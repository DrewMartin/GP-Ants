#ifndef PART3WINDOW_H
#define PART3WINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QSharedPointer>
#include <QGraphicsEllipseItem>

#include "common/constants.h"
#include "options.h"
#include "simulation.h"
#include "blockingqueue.h"
#include "blockingcounter.h"

namespace Ui {
class Part3Window;
}

class Part3Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Part3Window(QWidget *parent = 0);
    ~Part3Window();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void foodCountChanged(int val);
    void pheremoneDecayRateChanged(int val);
    void antCountChanged(int val);
    void pheremoneRadiusChanged(int val);
    void generationsChanged(int val);
    void elitismChanged(int val);

    void mouseClick(QPoint point);
    void reset();
    void clear();
    void start();
    void stop();
    void startDisplay();
    void timerSlot();
    void tryCustomBehaviour();

private:

    void setWidgetsEnabled(bool enabled);
    void generatePop();
    void workerFunction();
    void GPFunction();
    void updateLoop();
    QSP<AntNode> tournamentSelect(QList<QSP<AntNode> > &popList);

    Ui::Part3Window *ui;
    QSP<QGraphicsScene> scene;
    QSP<AntNode> bestBehaviour;
    QList<QSP<AntNode> > pop;
    BlockingQueue<int> work;
    BlockingQueue<QSP<AntNode> > nextGen;
    BlockingCounter counter;

    bool running;
    bool runningGP;
    int gen;


    // for running the sim on graphics
    QList<QSP<Food> > food;
    QList<QSP<GPAnt> > ants;
    QList<QList<Cell> > cells;
    bool foodCleared;
    int turn;

};

#endif // PART3WINDOW_H
