#include "part2window.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    Part2Window w;
    w.show();
    
    return a.exec();
}
