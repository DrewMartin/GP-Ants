#include "part3window.h"
#include "ui_part3window.h"
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

Part3Window::Part3Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Part3Window),
    graphicSim(new Simulation()),
    running(false),
    runningGP(false)
{
    ui->setupUi(this);
    bestBehaviour = AntNode::generateGrowTree(5);
    graphicSim->setBehaviour(bestBehaviour);
    qDebug() << bestBehaviour->toString();
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
//    ants.clear();
//    for (int i = 0; i < food.length(); i++)
//        food.at(i)->reset();

//    for (int row = 0; row < MAX_X; row++)
//        for(int col = 0; col < MAX_Y; col++)
//            cells[row][col].reset();

}

void Part3Window::start()
{
    runningGP = true;
    setWidgetsEnabled(false);

}

void Part3Window::stop()
{
    if (running)
        graphicSim->stop();
    running = runningGP = false;
    setWidgetsEnabled(true);
}

void Part3Window::startDisplay()
{
    running = true;
    setWidgetsEnabled(false);
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
