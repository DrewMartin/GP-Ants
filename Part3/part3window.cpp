#include "part3window.h"
#include "ui_part3window.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

Part3Window::Part3Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Part3Window),
    graphicSim(new Simulation()),
    counter(ANT_POP_SIZE),
    running(false),
    runningGP(false),
    gen(0)
{
    ui->setupUi(this);
    generatePop();
    clear();
    ui->graphicsView->scale(SCALE, SCALE);

    ui->foodCountSlider->setMaximum(MAX_FOOD);
    ui->foodCountSlider->setValue(MAX_FOOD/2);

    foodCountChanged(ui->foodCountSlider->value());
    pheremoneDecayRateChanged(ui->decaySlider->value());
    antCountChanged(ui->antCountSlider->value());
    pheremoneRadiusChanged(ui->pheremoneRadiusSlider->value());
    generationsChanged(ui->generationSlider->value());
    elitismChanged(ui->elitismSlider->value());

    connect(ui->foodCountSlider, SIGNAL(valueChanged(int)), SLOT(foodCountChanged(int)));
    connect(ui->decaySlider, SIGNAL(valueChanged(int)), SLOT(pheremoneDecayRateChanged(int)));
    connect(ui->antCountSlider, SIGNAL(valueChanged(int)), SLOT(antCountChanged(int)));
    connect(ui->pheremoneRadiusSlider, SIGNAL(valueChanged(int)), SLOT(pheremoneRadiusChanged(int)));
    connect(ui->generationSlider, SIGNAL(valueChanged(int)), SLOT(generationsChanged(int)));
    connect(ui->elitismSlider, SIGNAL(valueChanged(int)), SLOT(elitismChanged(int)));

    connect(ui->graphicsView, SIGNAL(clicked(QPoint)), SLOT(mouseClick(QPoint)));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clear()));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
    connect(ui->startVisual, SIGNAL(clicked()), SLOT(startDisplay()));
    setWidgetsEnabled(true);
}

Part3Window::~Part3Window()
{
    delete ui;
}

void Part3Window::closeEvent(QCloseEvent *)
{
    stop();
}

void Part3Window::foodCountChanged(int val)
{
    ui->foodCountLabel->setText(QString::number(val));
}

void Part3Window::pheremoneDecayRateChanged(int val)
{
    ui->decayLabel->setText(QString("0.%1").arg(val, 2, 10, QChar('0')));
    Options::getInstance()->setPheromoneDecayRate(val / 100.0);
}

void Part3Window::pheremoneRadiusChanged(int val)
{
    ui->pheremoneRadiusLabel->setText(QString::number(val));
    Options::getInstance()->setPheromoneRadius(val);
}

void Part3Window::antCountChanged(int val)
{
    ui->antCountLabel->setText(QString::number(val));
    Options::getInstance()->setAntCount(val);
}

void Part3Window::generationsChanged(int val)
{
    ui->generationLabel->setText(QString::number(val));
    Options::getInstance()->setMaxGenerations(val);
}

void Part3Window::elitismChanged(int val)
{
    ui->elitismLabel->setText(QString::number(val));
    Options::getInstance()->setElitism(val);
}

void Part3Window::clear()
{
    graphicSim->preClear();
    Options::getInstance()->clearFood();
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X, MAX_Y));
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(scene.data());
    graphicSim->setScene(scene);
    graphicSim->setupSim();
}

void Part3Window::reset()
{
    graphicSim->reset();

}

void Part3Window::start()
{
    runningGP = true;
    setWidgetsEnabled(false);
    QtConcurrent::run(this, &Part3Window::GPFunction);
}

void Part3Window::stop()
{

    if (running) {
        graphicSim->stop();
    } else if (runningGP) {
        ui->stopButton->setEnabled(false);
        ui->stopButton->setText("Working");
        int threads = QThread::idealThreadCount() - 1;
        work.clear();
        for (int i = 0; i < threads; i++) {
            work.put(WORK_QUIT);
        }
        work.waitForEmpty();
        ui->stopButton->setText("Stop");
    }

    running = runningGP = false;
    setWidgetsEnabled(true);
}

void Part3Window::startDisplay()
{
    running = true;
    setWidgetsEnabled(false);
    graphicSim->setBehaviour(bestBehaviour);
    QtConcurrent::run(graphicSim.data(), &Simulation::run);
}

void Part3Window::mouseClick(QPoint point)
{
    if (running || runningGP)
        return;

    point = QPoint(point.x()/SCALE, point.y()/SCALE);

    ui->graphicsView->pos();
    int amount = ui->foodCountSlider->value();
    QSP<Food> f = QSP<Food>(new Food(point, amount));
    QGraphicsItem *item = f->getGraphicsItem();

    if (!scene->collidingItems(item).length()) {
        Options::getInstance()->addFood(point, amount);
        graphicSim->addFood(f);
    }

}

void Part3Window::setWidgetsEnabled(bool enabled)
{
    ui->resetButton->setEnabled(enabled);
    ui->clearButton->setEnabled(enabled);
    ui->startButton->setEnabled(enabled);
    ui->startVisual->setEnabled(enabled && !bestBehaviour.isNull());
    ui->stopButton->setEnabled(!enabled);

    ui->foodCountSlider->setEnabled(enabled);
    ui->pheremoneRadiusSlider->setEnabled(enabled);
    ui->decaySlider->setEnabled(enabled);
    ui->antCountSlider->setEnabled(enabled);
    ui->generationSlider->setEnabled(enabled);
    ui->elitismSlider->setEnabled(enabled);
}

void Part3Window::generatePop()
{
    pop.clear();

    QSP<AntNode> tree;
    QString hash;
    QSet<QString> popAlready;
    popAlready.reserve(ANT_POP_SIZE);
    int each = ANT_POP_SIZE /((MAX_HEIGHT - 1)*2);
    for(int i = 2; i <= MAX_HEIGHT; i++) {
        for (int j = 0; j < each; j++) {
            while (true) {
                tree = AntNode::generateFullTree(i);
                hash = tree->toString();
                if (!popAlready.contains(hash)) {
                    popAlready.insert(hash);
                    pop.append(tree);
                    break;
                }
            }

            while (true) {
                tree = AntNode::generateGrowTree(i);
                hash = tree->toString();
                if (!popAlready.contains(hash)) {
                    popAlready.insert(hash);
                    pop.append(tree);
                    break;
                }
            }
        }
    }
}

#define BEST_CHANCE 90

QSP<AntNode> Part3Window::tournamentSelect(QList<QSP<AntNode> > &popList)
{
    QList<QSP<AntNode> > tournament;
    QSP<AntNode> selection, best;

    for (int i = 0; i < TOURNAMENT_SIZE; i++) {
        selection = popList.at(RAND_INT(popList.length()));
        tournament += selection;
        if (best.isNull() || best->getScore() < selection->getScore())
            best = selection;
    }

    if (!best.isNull() && RAND_INT(100) < BEST_CHANCE)
        return best;

    return tournament.at(RAND_INT(TOURNAMENT_SIZE));
}

void Part3Window::workerFunction()
{
    int task;
    QSP<Node> tempNode;
    QSP<AntNode> n1, n2;
    Simulation *sim;
    QTime timer;
    while (true) {
        task = work.take();
        if (task == WORK_QUIT)
            return;
        if (task == WORK_GEN) {
            if (RAND_INT(100) < XOVER_PROB) {
                while (true) {
                    n1 = tournamentSelect(pop);
                    n2 = tournamentSelect(pop);
                    tempNode = AntNode::crossover(n1, n2);
                    if (!tempNode.isNull()) {
                        n1 = tempNode.staticCast<AntNode>();
                        nextGen.put(n1);
                        break;
                    }
                }
            } else {
                while (true) {
                    n1 = tournamentSelect(pop);
                    tempNode = AntNode::mutate(n1);
                    if (!tempNode.isNull()) {
                        n1 = tempNode.staticCast<AntNode>();
                        nextGen.put(n1);
                        break;
                    }
                }
            }
        } else {
            if (pop.at(task)->isScored()) {
                counter.increment();
            } else {
                sim = new Simulation(MAX_TURNS);
                sim->setBehaviour(pop.at(task));
                sim->run();
                pop.at(task)->setScore(sim->getScore());
                counter.increment();
                delete sim;
            }
        }
    }
}

bool comparator(QSP<AntNode> &n1, QSP<AntNode> &n2) {
    return n1->getScore() > n2->getScore();
}

void Part3Window::GPFunction()
{
    QList<QSP<AntNode> > nextPop;
    QTime timer;

    int maxGen = gen + Options::getInstance()->getMaxGenerations();
    int i;
    double keepAmount = Options::getInstance()->getElitism() / 100.0;

    work.clear();
    int threads = QThread::idealThreadCount() - 1;
    for (i = 0; i < threads; i++) {
        QtConcurrent::run(this, &Part3Window::workerFunction);
    }

    while (runningGP && gen < maxGen) {
        timer.start();
        nextGen.clear();

        for (i = 0; i < ANT_POP_SIZE; i++) {
            work.put(i);
        }
        counter.wait();

        gen++;
        qSort(pop.begin(), pop.end(), comparator);

        if (bestBehaviour.isNull() || pop.at(0)->getScore() > bestBehaviour->getScore())
            bestBehaviour = pop.at(0);
        ui->currGeneration->setText(QString::number(gen));
        ui->bestFitness->setText(QString::number(bestBehaviour->getScore()));

        nextPop.clear();
        if (keepAmount > 0.0)
            nextPop = pop.mid(0, pop.length() * keepAmount);
        work.clear();
        nextGen.clear();
        for(i = 0; i < ANT_POP_SIZE - nextPop.length(); i++) {
            work.put(WORK_GEN);
        }
        nextGen.waitOnSize(ANT_POP_SIZE - nextPop.length());
        nextPop.append(nextGen.takeList());
        pop = nextPop;
        qDebug() << "Took" << timer.elapsed() << "for gen" << gen;
    }
    stop();
}
