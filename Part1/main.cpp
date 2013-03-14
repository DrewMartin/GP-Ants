#include "part1window.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    Part1Window w;
    w.show();
    
    return a.exec();
}
