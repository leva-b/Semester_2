#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "factory.h"
#include <QTimer>

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

    Factory WriteStruct();
    void WriteAndSaveText();
    void ReadText();

    void WriteAndSaveArray();
    void ReadArray();

    void WriteAndSaveStructTXT();
    void ReadStructTXT();

    void WriteAndSaveStructBIN();
    void ReadStructBIN();
    QString toQString(Factory factory);

    void WriteAndSaveSentences();
    void ReadSentences();

public slots:
    void menu();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    const QChar separator = '#';
    Factory factory;

    const QString File = "ЛевчишинЛаб20.txt";
    const QString File_0 = "ЛевчишинЛаб20_0.txt";
    const QString File_1 = "ЛевчишинЛаб20_1.txt";
    const QString File_2 = "ЛевчишинЛаб20_2.txt";
    const QString File_3 = "ЛевчишинЛаб20_3.txt";
};
#endif // MAINWINDOW_H
