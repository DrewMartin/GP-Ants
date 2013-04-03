#ifndef PART1WINDOW_H
#define PART1WINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QPoint>
#include <QSharedPointer>
#include <QList>

#include "common/constants.h"
#include "common/food.h"
#include "smartant.h"
#include "common/cell.h"
#include "common/pheremone.h"

namespace Ui {
class Part1Window;
}

class Part1Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Part1Window(QWidget *parent = 0);
    ~Part1Window();


private slots:
    void foodCountChanged(int val);
    void pheremoneDecayRateChanged(int val);
    void antCountChanged(int val);
    void pheremoneRadiusChanged(int val);

    void mouseClick(QPoint point);
    void reset();
    void clear();
    void start();
    void stop();
    void timerSlot();
    
private:
    void setWidgestEnabled(bool enabled);
    void updateLoop();

    Ui::Part1Window *ui;
    QSP<QGraphicsScene> scene;

    QGraphicsEllipseItem *anthill;

    QList<QSP<Food> > food;
    QList<QSP<SmartAnt> > ants;
    QList<QList<Cell> > cells;
    bool running;
    bool foodCleared;
    int turn;
};

#endif // PART1WINDOW_H
