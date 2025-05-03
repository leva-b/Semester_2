#include "mainwindow.h"/*
#include "ui_mainwindow.h"*/
#include <QApplication>
#include <QInputDialog>
#include "date.h"
#include <fstream>

MainWindow::~MainWindow() {
    delete tableWidget;
    delete openButton;
    delete addDateButton;
    delete editDateButton;
    delete birthdayEdit;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainlayout = new QHBoxLayout(centralWidget);
    //mainlayout->setContentsMargins(10, 10, 10, 10);  // Устанавливаем отступы

    // Создаем таблицу
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels({"Date", "Previous Day", "Next Day", "Week num", "Duration", "Days Till Birthday"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);  // Растягиваем таблицу
    mainlayout->addWidget(tableWidget, 4);  // Таблица занимает 3 части

    // Добавляем кнопки
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(0, 0, 0, 0);

    openButton = new QPushButton("Open .txt File", this);
    openButton->setFixedHeight(50);

    layout->addWidget(openButton);

    addDateButton = new QPushButton("Add Date", this);
    addDateButton->setFixedHeight(50);
    layout->addWidget(addDateButton);

    editDateButton = new QPushButton("Edit Date", this);
    editDateButton->setFixedHeight(50);
    layout->addWidget(editDateButton);

    birthdayEdit = new QLineEdit(this);
    birthdayEdit->setPlaceholderText("Enter your birthday (dd.mm.yyyy)");
    birthdayEdit->setFixedHeight(40);
    layout->addWidget(birthdayEdit);

    layout->addStretch();
    mainlayout->addLayout(layout, 1);

    openButton->setStyleSheet("background-color: #388E3C; color: white; border-radius: 5px; font-size: 16px;");
    addDateButton->setStyleSheet("background-color: #1976D2; color: white; border-radius: 5px; font-size: 16px;");
    editDateButton->setStyleSheet("background-color: #D32F2F; color: white; border-radius: 5px; font-size: 16px;");
    birthdayEdit->setStyleSheet("color: white; background-color: #333; border: 1px solid #555; border-radius: 5px; font-size: 16px");

    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(addDateButton, &QPushButton::clicked, this, &MainWindow::addDate);
    connect(editDateButton, &QPushButton::clicked, this, &MainWindow::editDate);
    connect(birthdayEdit, &QLineEdit::editingFinished, this, &MainWindow::updateBirthday);
}


void MainWindow::openFile() {
    filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    // Проверяем, что файл не пустой и имеет расширение .txt
    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        dates = Date::readDatesFromFile(filePath.toStdString());
        updateTable();
    } else {
        QMessageBox::warning(this, "Invalid File", "Please select a valid .txt file.");
    }
}


void MainWindow::updateTable() {
    tableWidget->setRowCount(dates.size());
    for (long long  i = 0; i < dates.size(); ++i) {
        Date currentDate = dates[i];
        Date nextDate = currentDate.NextDay();
        Date previousDate = currentDate.PreviousDay();
        int duration = (i < dates.size() - 1) ? currentDate.Duration(dates[i + 1]) : 0;
        int daysTillBirthday = currentDate.DaysTillYourBithday(birthday);

        // qDebug() << currentDate.getNumberOfDayInYear() << "currentDate\n";

        tableWidget->setItem(i, 0, new QTableWidgetItem(QString("%1.%2.%3").arg(currentDate.getDay(), 2, 10, QChar('0')).arg(currentDate.getMonth(), 2, 10, QChar('0')).arg(currentDate.getYear(), 4, 10, QChar('0'))));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1.%2.%3").arg(previousDate.getDay(), 2, 10, QChar('0')).arg(previousDate.getMonth(), 2, 10, QChar('0')).arg(previousDate.getYear(), 4, 10, QChar('0'))));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString("%1.%2.%3").arg(nextDate.getDay(), 2, 10, QChar('0')).arg(nextDate.getMonth(), 2, 10, QChar('0')).arg(nextDate.getYear(), 4, 10, QChar('0'))));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(currentDate.WeekNumber())));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(duration)));
        tableWidget->setItem(i, 5, new QTableWidgetItem(birthday.getNumberOfDayInYear() != -1 ? QString::number(daysTillBirthday): QString("-#-")));
        tableWidget->item(i, 0)->setFlags(tableWidget->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        tableWidget->item(i, 2)->setFlags(tableWidget->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        tableWidget->item(i, 4)->setFlags(tableWidget->item(i, 0)->flags() & ~Qt::ItemIsEditable);
        tableWidget->item(i, 5)->setFlags(tableWidget->item(i, 0)->flags() & ~Qt::ItemIsEditable);
    }
}

void MainWindow::addDate() {
    QString dateStr = QInputDialog::getText(this, "Add Date", "Enter date (dd.mm.yyyy):");
    if (!dateStr.isEmpty()) {
        int day, month, year;
        if (validateDate(dateStr, day, month, year)) {
            dates.push_back(Date(day, month, year));
            updateTable();
            if(!filePath.isEmpty()){
                updateDateInFile(dates.size() - 1, dates[dates.size() - 1]);
            }
        }
        updateTable();
    }
}

void MainWindow::editDate() {
    int row = tableWidget->currentRow();
    if (row >= 0 && row < (int)dates.size()) {
        QString dateStr = QInputDialog::getText(this, "Edit Date", "Enter new date (dd.mm.yyyy):", QLineEdit::Normal,
                                                QString("%1.%2.%3").arg(dates[row].getDay(), 2, 10, QChar('0'))
                                                    .arg(dates[row].getMonth(), 2, 10, QChar('0'))
                                                    .arg(dates[row].getYear(), 4, 10, QChar('0')));
        if (!dateStr.isEmpty()) {
            int day, month, year;
            if (validateDate(dateStr, day, month, year)) {
                dates[row] = Date(day, month, year);
                updateTable();
                if(!filePath.isEmpty()){
                    updateDateInFile(row, dates[row]);
                }
            }
        }
    }
}

void MainWindow::updateBirthday() {
    QString dateStr = birthdayEdit->text();

    if (!dateStr.isEmpty()) {
        int day, month, year;
        if (validateDate(dateStr, day, month, year)) {
            birthday = Date(day, month, year);
            updateTable();
        }
    }
     // Сохраняем дату рождения
    qDebug() << birthday.getNumberOfDayInYear() << "birthday-\n";
}



bool MainWindow::validateDate(const QString& dateStr, int& day, int& month, int& year) {
    // Проверяем формат даты
    QRegularExpression regex(R"(^(\d{2})\.(\d{2})\.(\d{4})$)");
    QRegularExpressionMatch match = regex.match(dateStr);
    if (match.hasMatch()) {
        try {
            day = match.captured(1).toInt();
            month = match.captured(2).toInt();
            year = match.captured(3).toInt();

            try{
                if(day > Date::daysInMonth(month,year) || month > 12 || month <= 0){
                    throw false;
                }else {
                    return true;
                }
            }catch(bool){
                QMessageBox::warning(nullptr, "Invalid Date", "The entered date is not valid.");
            }
        } catch (const std::exception &e) {
            QMessageBox::warning(nullptr, "Error", "An error occurred while processing the date: " + QString::fromStdString(e.what()));
        }
    } else {
        QMessageBox::warning(nullptr, "Invalid Format", "Please enter the date in the format dd.mm.yyyy.");
    }
    return false;
}

void MainWindow::updateDateInFile(int lineNumber, const Date& date) {
    std::fstream file(filePath.toStdString(), std::ios::in | std::ios::out);

    if (!file) {
        QMessageBox::warning(this, "Error", "Could not open file for updating.");
        return;
    }

    // Каждая дата занимает 11 символов (формат dd.mm.yyyy\n)
    std::streampos position = lineNumber * 11;
    file.seekp(position); // Перемещаем указатель записи на нужную строку

    file << QString("%1.%2.%3\n").arg(date.getDay(), 2, 10, QChar('0'))
                .arg(date.getMonth(), 2, 10, QChar('0'))
                .arg(date.getYear(), 4, 10, QChar('0')).toStdString();

    file.close();
}

