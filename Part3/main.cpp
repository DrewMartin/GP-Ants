#include "part3window.h"
#include <QApplication>
#include <QTime>

#include "antnode.h"
Q_DECLARE_METATYPE(QSP<AntNode>)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QSP<AntNode> >();

    qsrand(QTime::currentTime().msec());
    Part3Window w;
    w.show();
    
    return a.exec();
}
