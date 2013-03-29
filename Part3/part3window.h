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

namespace Ui {
class Part3Window;
}

class Part3Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Part3Window(QWidget *parent = 0);
    ~Part3Window();

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

private:
    void setWidgetsEnabled(bool enabled);
    Ui::Part3Window *ui;
    QSP<QGraphicsScene> scene;
    QSP<Simulation> graphicSim;
    QSP<AntNode> bestBehaviour;
    bool running;
    bool runningGP;

};

#endif // PART3WINDOW_H
