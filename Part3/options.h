#ifndef OPTIONS_H
#define OPTIONS_H

#include <QPair>
#include <QPoint>
#include <QList>

class Options
{
public:
    static Options* getInstance();

    double getPheromoneDecayRate();
    int getAntCount();
    int getPheromoneRadius();
    int getMaxGenerations();
    int getElitism();
    const QList<QPair<QPoint, int> > &getFood();

    void setPheromoneDecayRate(double val);
    void setAntCount(int val);
    void setPheromoneRadius(int val);
    void setMaxGenerations(int val);
    void setElitism(int val);
    void addFood(QPoint loc, int amount);
    void clearFood();

private:
    Options();

    static Options *instance;

    double pheromoneDecayRate;
    int antCount;
    int pheromoneRadius;
    int maxGenerations;
    int elitism;
    QList<QPair<QPoint, int> > foodLocations;
};

#endif // OPTIONS_H
