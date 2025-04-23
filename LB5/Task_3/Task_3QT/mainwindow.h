#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bitset.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum BitsetSelector { FIRST_BITSET, SECOND_BITSET };
    BitsetSelector currentBitset = FIRST_BITSET;
    Bitset<32>& selectedBitset();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    bool get_ch();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();

private:
    Ui::MainWindow *ui;
    Bitset<32> value_1;
    Bitset<32> value_2;
    int ch;
};
#endif // MAINWINDOW_H
