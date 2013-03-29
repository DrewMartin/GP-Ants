#include "part1window.h"
#include "ui_part1window.h"
#include <QDebug>
#include <QTime>
#include <QTimer>

Part1Window::Part1Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Part1Window)
{
    running = false;
    ui->setupUi(this);
    ui->graphicsView->scale(SCALE, SCALE);

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

    ui->foodCountSlider->setMaximum(MAX_FOOD);
    ui->foodCountSlider->setValue(MAX_FOOD/2);

    foodCountChanged(ui->foodCountSlider->value());
    pheremoneDecayRateChanged(ui->decaySlider->value());
    antCountChanged(ui->antCountSlider->value());
    pheremoneRadiusChanged(ui->pheremoneRadiusSlider->value());

    connect(ui->foodCountSlider, SIGNAL(valueChanged(int)), SLOT(foodCountChanged(int)));
    connect(ui->decaySlider, SIGNAL(valueChanged(int)), SLOT(pheremoneDecayRateChanged(int)));
    connect(ui->antCountSlider, SIGNAL(valueChanged(int)), SLOT(antCountChanged(int)));
    connect(ui->pheremoneRadiusSlider, SIGNAL(valueChanged(int)), SLOT(pheremoneRadiusChanged(int)));

    connect(ui->graphicsView, SIGNAL(clicked(QPoint)), SLOT(mouseClick(QPoint)));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(reset()));
    connect(ui->clearButton, SIGNAL(clicked()), SLOT(clear()));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(start()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(stop()));
}

Part1Window::~Part1Window()
{
    delete ui;
}

void Part1Window::foodCountChanged(int val)
{
    ui->foodCountLabel->setText(QString::number(val));
}

void Part1Window::pheremoneDecayRateChanged(int val)
{
    ui->decayLabel->setText(QString("0.%1").arg(val, 2, 10, QChar('0')));
}

void Part1Window::pheremoneRadiusChanged(int val)
{
    ui->pheremoneRadiusLabel->setText(QString::number(val));
}

void Part1Window::antCountChanged(int val)
{
    ui->antCountLabel->setText(QString::number(val));
}

void Part1Window::clear()
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
    anthill = new QGraphicsEllipseItem(LOC_HELPER(loc, ANTHILL_RAD));
    anthill->setPen(QPen(Qt::magenta));
    anthill->setBrush(QBrush(Qt::magenta));
    scene->addItem(anthill);
}

void Part1Window::reset()
{
    ants.clear();
    for (int i = 0; i < food.length(); i++)
        food.at(i)->reset();

    for (int row = 0; row < MAX_X; row++)
        for(int col = 0; col < MAX_Y; col++)
            cells[row][col].reset();

}

void Part1Window::start()
{
    setWidgestEnabled(false);
    ants.clear();
    pheremones.clear();
    QSP<SmartAnt> ant;
    QPoint p;

    double decay = ui->decaySlider->value() / 100.0;
    Cell::setDecay(decay);
    for (int i = 0; i < ui->antCountSlider->value(); i++) {
        p = QPoint(MAX_X/2, MAX_Y/2);
        ant = QSP<SmartAnt>(new SmartAnt(p));
        ants.append(ant);
        scene->addItem(ant->getGraphicsItem());
    }
    turn = 0;
    foodCleared = false;
    updateLoop();
}

void Part1Window::stop()
{
    setWidgestEnabled(true);
}

void Part1Window::timerSlot()
{
    updateLoop();
}

void Part1Window::mouseClick(QPoint point)
{
    if (running)
        return;

    point = QPoint(point.x()/SCALE, point.y()/SCALE);

    ui->graphicsView->pos();
    int amount = ui->foodCountSlider->value();
    QSP<Food> f = QSP<Food>(new Food(point, amount));
    QGraphicsItem *item = f->getGraphicsItem();

    if (!scene->collidingItems(item).length()) {
        food.append(f);
        scene->addItem(item);


        CIRCLE_LOOP_HELPER(point.x(), point.y(), FOOD_RAD)
                cells[row][col].setFood(f);
    }
}

void Part1Window::setWidgestEnabled(bool enabled)
{
    ui->resetButton->setEnabled(enabled);
    ui->clearButton->setEnabled(enabled);
    ui->startButton->setEnabled(enabled);
    ui->stopButton->setEnabled(!enabled);

    ui->foodCountSlider->setEnabled(enabled);
    ui->pheremoneRadiusSlider->setEnabled(enabled);
    ui->decaySlider->setEnabled(enabled);
    ui->antCountSlider->setEnabled(enabled);
    this->running = !enabled;

}

void Part1Window::updateLoop()
{
    if (!running)
        return;

    QTime timer;
    timer.start();
    int pheremoneRadius = ui->pheremoneRadiusSlider->value();
    int row, col, i;
    QPoint loc, newLoc;
    QImage background(MAX_X, MAX_Y, QImage::Format_ARGB32_Premultiplied);
    background.fill(Qt::white);
    turn++;

    for (row = 0; row < MAX_X; row++)
        for(col = 0; col < MAX_Y; col++)
            cells[row][col].update();

    for (i = 0; i < ants.length(); i++) {
        if (ants.at(i)->update(cells) && pheremoneRadius > 0) {
            loc = ants.at(i)->getLocation();
            CIRCLE_LOOP_HELPER(loc.x(), loc.y(), pheremoneRadius) {
                newLoc = QPoint(col, row);
                cells[row][col].addPheremone(50 - sqDist*50/sqRad);
            }
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
