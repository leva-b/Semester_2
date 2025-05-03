#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userinputdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initWithStr();
    initWithCode();
    initWithPtr();
    initWithRef();

    input = new UserInputDialog();
    connect(input, &UserInputDialog::inputInit, this, &MainWindow::initUserInput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWithStr()
{
    strInit = {1910, 20000.25, 'V', true, {'C', 'a', 'r'}, {1910, 1911, 1911, 1910}};
    QString str = QString::number(strInit.workshopCount) + "|" + QString::number(strInit.productionVolume) + "|" + strInit.category + "|" + (strInit.isOperating ? "true" : "false") + "|" + QString(strInit.factoryName) + "|";

    for (int i = 0; i < 4; ++i)
    {
        str += QString::number(strInit.dailyProduction[i]) + (i == 3 ? "" : ", ");
    }
    ui->textBrowser->setText(str);
}

void MainWindow::initWithCode()
{
    codeInit.category = 'W';
    codeInit.productionVolume = 50000;
    codeInit.workshopCount = 1913;
    codeInit.isOperating = true;
    char codeInitfactoryName[] = "My Car";
    for (int i = 0; i < 7; ++i)
    {
        codeInit.factoryName[i] = codeInitfactoryName[i];
    }
    int codeInitdailyProduction[] = {1914,1914,1914,1914};
    for (int i = 0; i < 4; ++i)
    {
        codeInit.dailyProduction[i] = codeInitdailyProduction[i];
    }

    QString str = QString::number(codeInit.workshopCount) + "|" + QString::number(codeInit.productionVolume) + "|" + codeInit.category + "|" + (codeInit.isOperating ? "true" : "false") + "|" + QString(codeInit.factoryName) + "|";
    for (int i = 0; i < 4; ++i)
    {
        str += QString::number(codeInit.dailyProduction[i]) + (i == 3 ? "" : ", ");
    }
    ui->textBrowser_2->setText(str);
}

void MainWindow::initWithPtr()
{
    ptr->category = 'P';
    ptr->productionVolume = 1000300.5;
    ptr->workshopCount = 1924;
    ptr->isOperating = false;
    char ptrInitfactoryName[] = "Ptr Car";
    for (int i = 0; i < 8; ++i)
    {
        ptr->factoryName[i] = ptrInitfactoryName[i];
    }
    int ptrInitdailyProduction[] = {1924,1925,1925,1925};
    for (int i = 0; i < 4; ++i)
    {
        ptr->dailyProduction[i] = ptrInitdailyProduction[i];
    }

    QString str = QString::number(ptr->workshopCount) + "|" + QString::number(ptr->productionVolume) + "|" + ptr->category + "|" + (ptr->isOperating ? "true" : "false") + "|" + QString(ptr->factoryName) + "|";
    for (int i = 0; i < 4; ++i)
    {
        str += QString::number(ptr->dailyProduction[i]) + (i == 3 ? "" : ", ");
    }
    ui->textBrowser_3->setText(str);
}

void MainWindow::initWithRef()
{
    ref.category = 'R';
    ref.productionVolume = 230040.2675;
    ref.workshopCount = 1920;
    ref.isOperating = true;
    char refInitfactoryName[] = "Ref Car";
    for (int i = 0; i < 8; ++i)
    {
        ref.factoryName[i] = refInitfactoryName[i];
    }
    int refInitdailyProduction[] = {1922,1923,1922,1920};
    for (int i = 0; i < 4; ++i)
    {
        ref.dailyProduction[i] = refInitdailyProduction[i];
    }

    QString str = QString::number(ref.workshopCount) + "|" + QString::number(ref.productionVolume) + "|" + ref.category + "|" + (ref.isOperating ? "true" : "false") + "|" + QString(ref.factoryName)+ "|";
    for (int i = 0; i < 4; ++i)
    {
        str += QString::number(ref.dailyProduction[i]) + (i == 3 ? "" : ", ");
    }
    ui->textBrowser_4->setText(str);
}

void MainWindow::initUserInput(Factory userInit)
{
    QString str = QString::number(userInit.workshopCount) + "|" + QString::number(userInit.productionVolume) + "|" + userInit.category + "|" + (userInit.isOperating ? "true" : "false") + "|" + QString(userInit.factoryName) + "|";
    for (int i = 0; i < 4; ++i)
    {
        str += QString::number(userInit.dailyProduction[i]) + (i == 3 ? "" : ", ");
    }
    ui->textBrowser_5->setText(str);
}

void MainWindow::on_pushButton_clicked()
{
    input->setWindowModality(Qt::ApplicationModal);
    input->show();
}


