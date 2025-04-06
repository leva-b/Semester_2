#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHeaderView>
#include "date.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void addDate();
    void editDate();
    void updateBirthday();

private:
    bool validateDate(const QString& dateStr, int& day, int& month, int& year);
    QString filePath = "";
    void updateDateInFile(int lineNumber, const Date& date);

    QTableWidget *tableWidget;
    QPushButton *openButton;
    QPushButton *addDateButton;
    QPushButton *editDateButton;
    QLineEdit *birthdayEdit;
    QVector<Date> dates;
    Date birthday = Date();
    void updateTable();
};

#endif // MAINWINDOW_H

