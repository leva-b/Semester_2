#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "disk.h"
#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QVector<Disk> _disks;
    QVector<std::pair<Disk, int>> _newDisks;
    QString filePath = "";

    void updateTable();
    void updateNewTable();
    void saveFile();

private slots:
    void search_disks();
    void sort_disks();
    void delete_row();
    void add_edit();
    void openFile();
    void show_all();
};
#endif // MAINWINDOW_H
