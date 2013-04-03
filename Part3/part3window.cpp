#include "part3window.h"
#include "ui_part3window.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>

Part3Window::Part3Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Part3Window),
    counter(ANT_POP_SIZE),
    running(false),
    runningGP(false),
    gen(0)
{
    ui->setupUi(this);
    generatePop();

    QPoint loc;
    QList<Cell> thisRow;
    for (int row = 0; row < MAX_Y; row++) {
        thisRow = QList<Cell>();
        for (int col = 0; col < MAX_X; col++) {
            loc = QPoint(col, row);
            thisRow.append(Cell(loc));
        }
        cells.append(thisRow);
    }

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
    connect(ui->behaviourButton, SIGNAL(clicked()), SLOT(tryCustomBehaviour()));
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
    for (int i = 0; i < MAX_X; i++) {
        for(int j = 0; j < MAX_Y; j++) {
            cells[j][i].clear();
            cells[j][i].setScent((sq(MAX_X)/2 - sqDistance(MAX_X/2, MAX_Y/2, j, i))/((double)sq(MAX_X)/2.0));
        }
    }

    ants.clear();
    food.clear();
    scene = QSharedPointer<QGraphicsScene>(new QGraphicsScene(0, 0, MAX_X, MAX_Y));
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(scene.data());

    QPoint loc(MAX_X/2, MAX_Y/2);
    CIRCLE_LOOP_HELPER(MAX_X/2, MAX_Y/2,ANTHILL_RAD)
            cells[row][col].setAnthill();
    QGraphicsEllipseItem *anthill = new QGraphicsEllipseItem(LOC_HELPER(loc, ANTHILL_RAD));
    anthill->setPen(QPen(Qt::magenta));
    anthill->setBrush(QBrush(Qt::magenta));
    scene->addItem(anthill);
    Options::getInstance()->clearFood();
    setWidgetsEnabled(true);
}

void Part3Window::reset()
{
    ants.clear();
    for (int i = 0; i < food.length(); i++)
        food.at(i)->reset();

    for (int row = 0; row < MAX_X; row++)
        for(int col = 0; col < MAX_Y; col++)
            cells[row][col].reset();

    QImage background(MAX_X, MAX_Y, QImage::Format_ARGB32_Premultiplied);
    background.fill(Qt::white);
    scene->setBackgroundBrush(background);

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
        running = false;
    } else if (runningGP) {
        runningGP = false;
        ui->stopButton->setEnabled(false);
        ui->stopButton->setText("Working");
        int threads = QThread::idealThreadCount() - 1;
        counter.force();
        nextGen.force();
        work.clear();
        for (int i = 0; i < threads; i++) {
            work.put(WORK_QUIT);
        }
        work.waitForEmpty();
        ui->stopButton->setText("Stop");
    }

    setWidgetsEnabled(true);
}

void Part3Window::startDisplay()
{
    running = true;
    setWidgetsEnabled(false);
    qDebug() << "Best behaviour" << bestBehaviour->toString();
    reset();
    QSP<GPAnt> ant;
    QPoint p;

    double decay = ui->decaySlider->value() / 100.0;
    Cell::setDecay(decay);
    for (int i = 0; i < ui->antCountSlider->value(); i++) {
        p = QPoint(MAX_X/2, MAX_Y/2);
        ant = QSP<GPAnt>(new GPAnt(p));
        ants.append(ant);
        scene->addItem(ant->getGraphicsItem());
    }
    turn = 0;
    foodCleared = false;
    updateLoop();
}

void Part3Window::timerSlot()
{
    updateLoop();
}

void Part3Window::tryCustomBehaviour()
{
    QString text = ui->behaviourEdit->toPlainText();
    try {
        bestBehaviour = AntNode::fromString(text);
        bestBehaviour->setScore(0);
        setWidgetsEnabled(true);
    } catch (...) {
        QMessageBox::warning(this, "Bad behaviour string", "The entered behaviour string is invalid");
    }
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
        food.append(f);
        scene->addItem(item);


        CIRCLE_LOOP_HELPER(point.x(), point.y(), FOOD_RAD)
                cells[row][col].setFood(f);
    }
    setWidgetsEnabled(true);
}

void Part3Window::setWidgetsEnabled(bool enabled)
{
    ui->resetButton->setEnabled(enabled);
    ui->clearButton->setEnabled(enabled);
    ui->startButton->setEnabled(enabled && Options::getInstance()->foodCount() > 0);
    ui->startVisual->setEnabled(enabled && !bestBehaviour.isNull());
    ui->stopButton->setEnabled(!enabled);

    ui->foodCountSlider->setEnabled(enabled);
    ui->pheremoneRadiusSlider->setEnabled(enabled);
    ui->decaySlider->setEnabled(enabled);
    ui->antCountSlider->setEnabled(enabled);
    ui->generationSlider->setEnabled(enabled);
    ui->elitismSlider->setEnabled(enabled);
    ui->behaviourButton->setEnabled(enabled);
}

void Part3Window::generatePop()
{
    pop.clear();

    QSP<AntNode> tree;
    QString hash;
    QSet<QString> popAlready;
    popAlready.reserve(ANT_POP_SIZE);
    for(int i = 2; i <= MAX_HEIGHT; i++) {
        for (int j = 0; j < ANT_POP_STEP; j++) {
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
                sim = new Simulation(pop.at(task), MAX_TURNS);
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
        counter.reset();
        for (i = 0; i < ANT_POP_SIZE; i++) {
            work.put(i);
        }
        counter.wait();
        if (!runningGP)
            return;

        gen++;
        qSort(pop.begin(), pop.end(), comparator);

        if (bestBehaviour.isNull() || pop.at(0)->getScore() > bestBehaviour->getScore()) {
            bestBehaviour = pop.at(0);
            qDebug() << "New best" << bestBehaviour->toString();
        }
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
        if (!runningGP)
            return;
        nextPop.append(nextGen.takeList());
        pop = nextPop;
        qDebug() << "Took" << timer.elapsed() << "ms for gen" << gen << "best score of" << bestBehaviour->getScore();
    }
    stop();
}

void Part3Window::updateLoop()
{
    if (!running)
        return;

    QTime timer;
    timer.start();
    int row, col, i;
    QSP<GPAnt> curr;
    QImage background(MAX_X, MAX_Y, QImage::Format_ARGB32_Premultiplied);
    background.fill(Qt::white);
    turn++;

    for (row = 0; row < MAX_X; row++)
        for(col = 0; col < MAX_Y; col++)
            cells[row][col].update();

    for (i = 0; i < ants.length(); i++) {
        curr = ants.at(i);
        bestBehaviour->eval(cells, curr);
        curr->moveForward();

        if (curr->antHasFood()) {
            if (cells[curr->getLocation().y()][curr->getLocation().x()].atAnthill())
                curr->dropFood();
        }
    }

    for (row = 0; row < MAX_X; row++)
        for(col = 0; col < MAX_Y; col++) {
            i = (cells[row][col].getPheremone()*255/MAX_PHEROMONE) / (1.0 - cells[row][col].getScent());
            background.setPixel(col, row, qRgba(0, i, 0, i));
        }

    if (!foodCleared) {
        foodCleared = true;
        for (i = 0; i < food.length(); i++) {
            if (food.at(i)->hasFood()) {
                foodCleared = false;
                break;
            }
        }
        if (foodCleared) {
            qDebug() << "All food eaten after" << turn << "turns";
        }
    }

    scene->setBackgroundBrush(background);

    QTimer::singleShot(qMax(0, 33 - timer.elapsed()), this, SLOT(timerSlot()));
}
