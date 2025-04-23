#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "../headers/myString.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLibrary>
#include <memory>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QPlainTextEdit *str_1;
    QPlainTextEdit *str_2;
    QPlainTextEdit *log;
    QLineEdit* inputN;
    QLineEdit* inputChar;

    void initializedButton(QVBoxLayout* layout);
    void handleStringFunction(const QString& funcName);

    std::unique_ptr<char[]> m_resultBuffer;
    QByteArray m_byteArray1;
    QByteArray m_byteArray2;

    QLibrary m_stringLib;
    typedef char* (*strcpy_func)(char*, const char*);
    strcpy_func m_strcpyFunc = nullptr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
