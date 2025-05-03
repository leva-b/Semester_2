#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include <QTreeWidget>

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
    void recursiveDirTraversal(const QString& path, QTreeWidgetItem* parentItem);
    void openFolder();
    void showFileSystem();
    void openFile(QTreeWidgetItem *item);
    QTreeWidget* treeWidget;

private:
    Ui::MainWindow *ui;
    QString path;
    QString pathToFile;
    QPushButton* openParentFolder;
};
#endif // MAINWINDOW_H
