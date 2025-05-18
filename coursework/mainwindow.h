#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// #include "graph.h"
// #include "graphparser.h"
#include "mazefromfileparser.h"
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QScrollArea>

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

    MazeFromFileParser* getParser() const { return parser; }

private slots:
    void toggleViews();
    void updateGraph();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    MazeWidget *mazeWidget;
    GraphWidget *graphWidget;
    QPropertyAnimation *animation = nullptr;
    bool isMazeView = true;
    QScrollArea* mazeScrollArea;
    QScrollArea* graphScrollArea;
    MazeFromFileParser* parser;
};

#endif // MAINWINDOW_H
