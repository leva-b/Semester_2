#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow a;
    a.resize(800, 600);
    a.show();
    return app.exec();
}
