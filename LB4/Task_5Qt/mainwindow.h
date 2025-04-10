#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "vector.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void addRowClicked();
    void removeRowClicked();
    void addItemClicked();
    void removeItemClicked();
    void pushBackItemClicked();

private:

    void initDefaultData();
    void updateTables();
    Vector<Pair<Vector<int>, Vector<Pair<int,double>>>> data;
    QTableWidget *tableWidget_first;
    QTableWidget *tableWidget_second;

    QPushButton *addRow;
    QPushButton *removeRow;
    QPushButton *addItem;
    QPushButton *removeItem;
    QPushButton *pushBackItem;
    QPushButton *pushBackRow;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
