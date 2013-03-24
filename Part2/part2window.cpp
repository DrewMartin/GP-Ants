#include "part2window.h"
#include "ui_part2window.h"
#include <QDebug>
#include <qmath.h>
#include <QGraphicsLineItem>
#include <QTime>
#include <QtConcurrent/QtConcurrent>

Part2Window::Part2Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Part2Window)
{
    ui->setupUi(this);

    pointsChanged(ui->pointsSlider->value());

    connect(ui->pointsSlider, SIGNAL(valueChanged(int)), SLOT(pointsChanged(int)));

    connect(ui->startButton, SIGNAL(clicked()), SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
    createWorkers();
}

Part2Window::~Part2Window()
{
    delete ui;
}

void Part2Window::pointsChanged(int val)
{
    ui->pointsLabel->setText(QString::number(val));
}

void Part2Window::start()
{
    setWidgetsEnabled(false);
    ui->generationText->setText("0");

    if (ui->trigRadio->isChecked())
        createTrigTree();
    else
        createPolyTree();

    ui->graphicsView->resetMatrix();
    bestLines.clear();
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene());

    int pointCount = ui->pointsSlider->value();
    double minY = -target->eval(MIN_X), maxY = minY;
    double prevX = MIN_X, prevY = minY;
    bestPoints.clear();
    bestPoints.append(QPointF(MIN_X, -minY));
    double step = (MAX_X - MIN_X) / pointCount;
    double nextPointAt = MIN_X + step * bestPoints.length();

    QBrush brush(Qt::blue);
    QPen pen(brush, 0);
    brush = QBrush(Qt::green);
    QPen bestPen(brush, 0);
    double res;
    for (double x = MIN_X + STEP_SIZE; x <= MAX_X; x += STEP_SIZE) {
        res = -target->eval(x);
        scene->addLine(prevX, prevY, x, res, pen);
        bestLines.append(scene->addLine(prevX, prevY, x, res, bestPen));
        prevX = x;
        prevY = res;
        if (res < minY)
            minY = res;
        if (res > maxY)
            maxY = res;
        if (bestPoints.length() < pointCount && nextPointAt <= x) {
            bestPoints.append(QPointF(x, -res));
            nextPointAt = MIN_X + step * bestPoints.length();
        }
    }

    minY *= 1.05;
    maxY *= 1.05;
    brush = QBrush(Qt::black);
    pen = QPen(brush, 0);
    scene->addLine(MIN_X, 0, MAX_X, 0, pen)->setZValue(-1);
    scene->addLine(0, minY, 0, maxY, pen)->setZValue(-1);

    scene->setSceneRect(MIN_X, minY, MAX_X - MIN_X, maxY - minY);
    ui->graphicsView->setScene(scene.data());

    ui->graphicsView->scale(ui->graphicsView->width()/(MAX_X - MIN_X), ui->graphicsView->height()/(maxY - minY));
    generatePop();
    stopNow = false;
    gen = 0;
    QtConcurrent::run(this, &Part2Window::updateLoop);
}

void Part2Window::stop()
{
    setWidgetsEnabled(true);
    stopNow = true;
}

void Part2Window::setWidgetsEnabled(bool enabled)
{
    ui->startButton->setEnabled(enabled);
    ui->stopButton->setEnabled(!enabled);
    ui->equationGroup->setEnabled(enabled);
}

void Part2Window::createTrigTree()
{
    QSP<MathNode> l(new VariableNode());
    QSP<MathNode> r(new ConstantNode(2));
    QSP<MathNode> c(new TwoArgNode(l, r, qPow)); // x^2
    l = QSP<MathNode>(new SingleArgNode(c, qSin)); // sin(x^2)
    r = QSP<MathNode>(new ConstantNode(3));
    r = QSP<MathNode>(new TwoArgNode(l, r, mult)); // 3 sin(x^2)

    c = QSP<MathNode>(new VariableNode());
    l = QSP<MathNode>(new SingleArgNode(c, qCos)); // cos(x)
    target = QSP<MathNode>(new TwoArgNode(l, r, add)); // cos(x) + 3 sin(x^2)
}

void Part2Window::createPolyTree()
{
    QSP<MathNode> l(new VariableNode());
    QSP<MathNode> r(new ConstantNode(3));
    r = QSP<MathNode>(new TwoArgNode(l, r, qPow)); // x ^ 3

    l = QSP<MathNode>(new ConstantNode(3));
    l = QSP<MathNode>(new TwoArgNode(l, r, mult)); // 3*x^3

    QSP<MathNode> l1(new VariableNode());
    r = QSP<MathNode>(new ConstantNode(2));
    r = QSP<MathNode>(new TwoArgNode(l1, r, qPow)); // x^2

    l1 = QSP<MathNode>(new ConstantNode(2));
    r = QSP<MathNode>(new TwoArgNode(l1, r, mult)); // 2 * x^2

    l = QSP<MathNode>(new TwoArgNode(l, r, add)); // 3 * x^3 + 2 * x^2

    l1 = QSP<MathNode>(new VariableNode());
    r = QSP<MathNode>(new ConstantNode(1));

    r = QSP<MathNode>(new TwoArgNode(l1, r, add)); // x + 1

    target = QSP<MathNode>(new TwoArgNode(l, r, add)); // 3x^3 + 2x^2 + x + 1
}

void Part2Window::generatePop()
{
    pop.clear();
    QSP<MathNode> tree;
    QString hash;
    QSet<QString> popAlready;
    popAlready.reserve(POP_SIZE);
    int each = POP_SIZE /((MAX_HEIGHT - 1)*2);
    for(int i = 2; i <= MAX_HEIGHT; i++) {
        for (int j = 0; j < each; j++) {
            while (true) {
                tree = MathNode::generateFullTree(i);
                hash = tree->toString();
                if (!popAlready.contains(hash)) {
                    popAlready.insert(hash);
                    tree->getScore(bestPoints);
                    pop.append(tree);
                    break;
                }
            }

            while (true) {
                tree = MathNode::generateGrowTree(i);
                hash = tree->toString();
                if (!popAlready.contains(hash)) {
                    popAlready.insert(hash);
                    tree->getScore(bestPoints);
                    pop.append(tree);
                    break;
                }
            }
        }
    }
}

#define BEST_CHANCE 90

QSP<MathNode> Part2Window::tournamentSelect(QList<QSP<MathNode> > &popList)
{
    QList<QSP<MathNode> > tournament;
    QSP<MathNode> selection, best;

    for (int i = 0; i < TOURNAMENT_SIZE; i++) {
        selection = popList.at(RAND_INT(popList.length()));
        tournament += selection;
        if (isnan(selection->getScore()) && (best.isNull() || best->getScore() > selection->getScore()))
            best = selection;
    }

    if (!best.isNull() && RAND_INT(100) < BEST_CHANCE)
        return best;

    return tournament.at(RAND_INT(TOURNAMENT_SIZE));
}

bool comparator(QSP<MathNode> &n1, QSP<MathNode> &n2) {
    if (isnan(n2->getScore()))
        return true;
    if (isnan(n1->getScore()))
        return false;
    return n1->getScore() < n2->getScore();
}

void Part2Window::updateLoop()
{
    QTime timer;
    QList<QSP<MathNode> > nextPop;
    QSP<MathNode> bestNode;
    QString hash, bestHash;

    double curr, x, prevX, prevY, i;

    while (!stopNow)
    {
        timer.restart();

        gen++;
        ui->generationText->setText(QString::number(gen));
        qSort(pop.begin(), pop.end(), comparator);

        bestNode = pop.at(0);
        hash = bestNode->toString();

        if (!bestNode.isNull() && hash != bestHash){
            bestHash = hash;
            ui->bestScoreText->setText(QString::number(bestNode->getScore(), 'f', 6));
            ui->bestEqnText->setText(bestNode->toString());

            prevX = MIN_X;
            prevY = -bestNode->eval(MIN_X);
            i = 0;
            for (x = MIN_X + STEP_SIZE; x <= MAX_X; x += STEP_SIZE, i++) {
                curr = -bestNode->eval(x);
                if (isnan(curr))
                    curr = 9999999999999.0;
                bestLines.at(i)->setLine(prevX, prevY, x, curr);
                prevX = x;
                prevY = curr;
            }
            if (bestNode->getScore() <= PRECISION) {
                stop();
                return;
            }
        }

        nextPop.clear();
        nextPop = pop.mid(0, pop.length()/10);
        work.clear();
        nextGen.clear();
        for (int i = 0; i < POP_SIZE - nextPop.length(); i++) {
            work.put(-1);
        }
        nextGen.waitOnSize(POP_SIZE - nextPop.length());
        nextPop.append(nextGen.takeList());

        pop = nextPop;
        qDebug () << "Time to update" << timer.elapsed();

    }
}

void Part2Window::createWorkers()
{
    int threads = QThread::idealThreadCount() - 1;
    for (int i = 0; i < threads; i++) {
        QtConcurrent::run(this, &Part2Window::workerFunction);
    }
}


void Part2Window::workerFunction()
{
    int task;
    QSP<Node<double, double> > tempNode;
    QSP<MathNode> n1, n2;
    while (true) {
        task = work.take();
        if (RAND_INT(100) < XOVER_PROB) {
            while (true) {
                n1 = tournamentSelect(pop);
                n2 = tournamentSelect(pop);
                tempNode = MathNode::crossover(n1, n2);
                if (!tempNode.isNull()) {
                    n1 = tempNode.staticCast<MathNode>();
                    n1->getScore(bestPoints);
                    nextGen.put(n1);
                    break;
                }
            }
        } else {
            while (true) {
                n1 = tournamentSelect(pop);
                tempNode = MathNode::mutate(n1);
                if (!tempNode.isNull()) {
                    n1 = tempNode.staticCast<MathNode>();
                    n1->getScore(bestPoints);
                    nextGen.put(n1);
                    break;
                }
            }
        }
    }
}
