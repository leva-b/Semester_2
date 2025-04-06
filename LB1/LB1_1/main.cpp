#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Ship ship;
    ship.resize(1000, 600);
    ship.show();
    return a.exec();
}


