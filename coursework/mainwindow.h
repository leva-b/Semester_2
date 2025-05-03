#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// #include "graph.h"
// #include "graphparser.h"
// #include "graphfromfileparser.h"
#include <QStackedWidget>
#include <QPropertyAnimation>

class MazeWidget;
class GraphWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleViews();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    MazeWidget *mazeWidget;
    GraphWidget *graphWidget;
    QPropertyAnimation *animation = nullptr;
    bool isMazeView = true;
};

#endif // MAINWINDOW_H
