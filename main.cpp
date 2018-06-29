#include "motomandriver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MotomanDriver w;
    w.show();

    return a.exec();
}
