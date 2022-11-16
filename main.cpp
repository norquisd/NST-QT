#include "nst.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NST w;
    w.show();
    return a.exec();
}
