#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new Canvas(this);
    canvas->resize(1000,500);
    setCentralWidget(canvas);
}

MainWindow::~MainWindow()
{
    delete ui;
}
