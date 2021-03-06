#ifndef PART2WINDOW_H
#define PART2WINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QGraphicsScene>
#include <QPoint>
#include <QList>
#include <QtConcurrent/QtConcurrent>

#include "twoargnode.h"
#include "singleargnode.h"
#include "constantnode.h"
#include "variablenode.h"
#include "common/blockingqueue.h"

namespace Ui {
class Part2Window;
}

class Part2Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Part2Window(QWidget *parent = 0);
    ~Part2Window();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void pointsChanged(int val);
    void elitismChanged(int val);
    void nextGenSlot(int gen, QString equation, double fitness);

    void start();
    void stop();
    void updateLoop();

signals:
    void nextGenSignal(int gen, QString equation, double fitness);
    
private:
    void createWorkers();
    void workerFunction();
    void setWidgetsEnabled(bool enabled);
    void createTrigTree();
    void createPolyTree();
    void generatePop();
    QSP<MathNode> tournamentSelect(QList<QSP<MathNode> > &popList);

    Ui::Part2Window *ui;
    QSP<MathNode> target;
    QSP<QGraphicsScene> scene;
    QList<QGraphicsLineItem*> bestLines;
    QList<QPointF> bestPoints;
    QList<QSP<MathNode> > pop;
    BlockingQueue<int> work;
    BlockingQueue<QSP<MathNode> > nextGen;

    bool stopNow;
    int gen;
};

#endif // PART2WINDOW_H
