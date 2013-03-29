#include "part3window.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime::currentTime().msec());
    Part3Window w;
    w.show();
    
    return a.exec();
}
