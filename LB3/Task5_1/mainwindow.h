#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include "node.h"

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
    void recursiveDirTraversal(const QString& path, Node* parentNode, int level);
    void printFileTree(Node* node, std::string prefix = "");


private:
    QListWidget* listWidget;
    Ui::MainWindow *ui;
    QPushButton* openParentFolder;
    QTextEdit* textEdit;
    Node* parentNode; // Указатель на корневой узел дерева
private slots:
    void itemDoubleClicked(QListWidgetItem* item);
    void openFolder();
};

#endif // MAINWINDOW_H
