#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Open .txt File");
    window.resize(900, 600);
    window.show();
    return app.exec();
}
