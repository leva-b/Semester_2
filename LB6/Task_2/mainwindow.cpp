#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QFile>
#include <QMessageBox>
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(menu()));
    timer.start(1);
}


void MainWindow::WriteAndSaveText()
{
    QString str = QInputDialog::getText(nullptr, "Write and save a text", "Write a text please: ", QLineEdit::Normal);
    QFile file(File);
    QTextStream out(&file);
    qDebug() << str << '\n';
    qDebug() << "Полный путь к файлу:" << QFileInfo(file).absoluteFilePath();

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    for(int i=0;i<str.length();i++)
        out << str[i];

    file.close();
    QMessageBox::information(this,"File saved","Data saved succesfully");
}

void MainWindow::ReadText()
{
    QFile file(File);
    QTextStream in(&file);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QMessageBox::information(this, "File content",
                             "File content:\n\n" + in.readAll());

    file.close();
}

void MainWindow::WriteAndSaveArray()
{
    QFile file(File_0);
    size_t size = QInputDialog::getInt(nullptr, "Write size array", "Enter a number please: ", 1,1 , INT_MAX,1);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    for(size_t i = 0; i < size; i++){
        in << QInputDialog::getDouble(nullptr, "Enter an array", "Enter a value: ", 1,1 , INT_MAX,1) << separator;
    }
    QMessageBox::information(this, "Read from file", in.readAll());
}

void MainWindow::ReadArray()
{
    QFile file(File_0);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString data;
    QChar ch;
    while(!out.atEnd()){
        out >> ch;
        if(ch == separator) data += ' ';
        else data += ch;
    }
    file.close();
    qDebug() << data;
    QMessageBox::information(this, "Text in file", data);
}

Factory MainWindow::WriteStruct(){
    int workshopCount = QInputDialog::getInt(nullptr, "Enter a workshop count", "Enter a number please: ", 1, 1, 100,1);

    double productionVolume = QInputDialog::getDouble(nullptr, "Enter a product volume", "Enter a double number please: ", 1, 1, 100,1);
    QStringList items = {"Yes", "No"};
    bool isOperating = QInputDialog::getItem(
        nullptr,
        "Choice",
        "select a value:",
        items,
        0,    // индекс выбранного по умолчанию (0 = "Да")
        false) == "Yes"? true : false; // не редактируемый);
    QString factoryName;
    bool ok;
    do {
        factoryName = QInputDialog::getText(
            nullptr,
            "Write and save a factory name",
            "Write a text please (max 16 chars):",
            QLineEdit::Normal,
            "",
            &ok
            );
        if (ok && factoryName.length() > Factory::MAX_NAME_SIZE) {
            QMessageBox::warning(nullptr, "Error", "Maximum 16 characters allowed!");
        }
    } while (ok && factoryName.length() > Factory::MAX_NAME_SIZE);
    double dailyProduction[Factory::PERFORMANCE_STATISTICS_SIZE];
    for (int i = 0; i < Factory::PERFORMANCE_STATISTICS_SIZE; i++) {
        dailyProduction[i] = QInputDialog::getDouble(
            nullptr,
            QString("Day %1 Production").arg(i+1),
            QString("Enter production for day %1:").arg(i+1),
            0.0, 0.0, 10000.0, 2
            );
    }
    QChar category;
    bool categoryValid = false;
    do {
        QString input = QInputDialog::getText(
            nullptr,
            "Factory Category",
            "Enter category (A-Z, single character):",
            QLineEdit::Normal,
            ""
            );
        if (input.length() == 1 && input[0].isLetter()) {
            category = input[0].toUpper();
            categoryValid = true;
        } else {
            QMessageBox::warning(nullptr, "Error", "Please enter a single letter (A-Z)!");
        }
    } while (!categoryValid);

    for (int i = 0; i < Factory::MAX_NAME_SIZE; i++) {
        factory.factoryName[i] = (i < factoryName.length()) ? factoryName[i] : QChar();
    }

    // Копирование остальных данных
    for (int i = 0; i < Factory::PERFORMANCE_STATISTICS_SIZE; i++) {
        factory.dailyProduction[i] = dailyProduction[i];
    }

    factory.productionVolume = productionVolume;
    factory.workshopCount = workshopCount;
    factory.category = category;
    factory.isOperating = isOperating;

    return factory;
}

void MainWindow::WriteAndSaveStructTXT()
{
    QFile file(File_1);
    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    Factory tmp = WriteStruct();
    for(int i = 0; i < Factory::PERFORMANCE_STATISTICS_SIZE; i++)
        in << tmp.dailyProduction[i] << ' ';
    in << '|';
    for(int i = 0; i < Factory::MAX_NAME_SIZE && tmp.factoryName[i] != '\0'; i++)
        in << tmp.factoryName[i] << ' ';
    in << '|';
    in << tmp.productionVolume << '|' << tmp.workshopCount << '|' << tmp.category << '|' << tmp.isOperating << '\n';
    file.close();
}

void MainWindow::ReadStructTXT()
{
    QFile file(File_1);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QTextStream in(&file);
    QString data;
    while(!in.atEnd()){
        data += in.readLine() + '\n';
    }
    file.close();
    QMessageBox::information(this, "Text in file", data);
}

void MainWindow::WriteAndSaveStructBIN()
{
    QFile file(File_2);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    Factory factory = WriteStruct();
    file.write(reinterpret_cast<char*>(&factory), sizeof(Factory));
    file.close();
}

void MainWindow::ReadStructBIN(){
    QFile file(File_2);/*
    QDataStream output(&file);*/
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    Factory factory;
    QString data;
    while (file.read(reinterpret_cast<char*>(&factory), sizeof(Factory))) {
        // Выводим данные
        data += toQString(factory) + '\n';
    }

    QMessageBox::information(this, "Structs in file", data);
    file.close();
}

QString MainWindow::toQString(Factory factory){
    QString str;
    for(int i = 0; i< Factory::PERFORMANCE_STATISTICS_SIZE; i++){
        str += QString::number(factory.dailyProduction[i]) + ' ';
    }
    str += "; ";
    for(int i = 0; i < Factory::MAX_NAME_SIZE; i++){
        str += factory.factoryName[i];
    }
    str += " ; " + QString::number(factory.productionVolume) + QString::number(factory.workshopCount);
    str += " ; " + QString::number(factory.productionVolume) + QString::number(factory.workshopCount);
    str += " ; " + QString(factory.category);
    str += " ; " + QString(factory.isOperating ? "Yes" : "No");
    return str;
}

void MainWindow::WriteAndSaveSentences()
{
    QString str = QInputDialog::getText(nullptr, "Write and save a text", "Write a text please: ", QLineEdit::Normal);
    QFile file(File_3);
    QTextStream out(&file);
    qDebug() << str << '\n';
    qDebug() << "Полный путь к файлу:" << QFileInfo(file).absoluteFilePath();

    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    out << str << '\n';
    file.close();
    QMessageBox::information(this,"File saved","Data saved succesfully");
}

void MainWindow::ReadSentences()
{
    QFile file(File_3);
    QTextStream in(&file);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error",
                              "Cannot open file for writing: " + file.errorString());
        return;
    }
    QMessageBox::information(this, "File content",
                             "File content:\n\n" + in.readAll());

    file.close();
}


void MainWindow::menu()
{
    bool u=true;
    int pb;

    while(u)
    {
        pb = QInputDialog::getInt(nullptr, "Choose function", "1. Write and save text\n2. Read text from file\n"
                                                              "3. Write and save array of double\n4. Read array from file\n"
                                                              "5. Write and save struct in txt file\n6. Read struct for txt file\n"
                                                              "7. Write and save struct in bin file\n8. Read struct for bin file\n"
                                                              "9. Write and save new sentence\n10.Read all sentences from file\n"
                                                              "11. Exit\n\nWrite number of function:", 0, 1, 11, 1);
        switch(pb)
        {
        case 1:
            WriteAndSaveText();
            break;
        case 2:
            ReadText();
            break;
        case 3:
            WriteAndSaveArray();
            break;
        case 4:
            ReadArray();
            break;
        case 5:
            WriteAndSaveStructTXT();
            break;
        case 6:
            ReadStructTXT();
            break;
        case 7:
            WriteAndSaveStructBIN();
            break;
        case 8:
            ReadStructBIN();
            break;
        case 9:
            WriteAndSaveSentences();
            break;
        case 10:
            ReadSentences();
            break;
        case 11:
            this->~MainWindow();
            return;
        default:
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
