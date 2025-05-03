#include "mainwindow.h"
#include "ui_mainwindow.h"

#define er QMessageBox::warning(this,"Error","Incorrect input")

Bitset<32>& MainWindow::selectedBitset() {
    return ui->radioButton->isChecked() ? value_1 : value_2;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    ui->result->clear();
    ui->bits_1->setPlainText(QString::fromStdString(value_1.to_string()));
    ui->bits_2->setPlainText(QString::fromStdString(value_2.to_string()));
}

bool MainWindow::get_ch()
{
    std::string s=ui->lineEdit->text().toStdString();

    if(s=="")
        return false;

    for(size_t i=0; i < s.size();i++)
    {
        if(!(s[i]>='0'&&s[i]<='9'))
            return false;
    }

    try
    {
        ch=stoi(s);
    }
    catch(...)
    {
        return false;
    }
    return true;
}


void MainWindow::on_pushButton_clicked()
{
    ui->result->setPlainText(selectedBitset().all()==1? "All 1" : "Not all 1");
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->result->setPlainText(selectedBitset().any()==1? "Have 1" : "Have not 1");
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->result->setPlainText(QString::fromStdString(std::to_string(selectedBitset().count())));
}


void MainWindow::on_pushButton_4_clicked()
{

    if(ui->lineEdit->text()=="")
    {
        selectedBitset().flip();
        update();
        return;
    }
    if(!get_ch() || ch < 0 || (size_t)ch>=selectedBitset().size())
    {
        er;
        return;
    }

    selectedBitset().flip(ch);
    update();
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->result->setPlainText(selectedBitset().none()==1? "No 1" : "Have 1");
}


void MainWindow::on_pushButton_9_clicked()
{
    if(!get_ch() || ch < 0 || (size_t)ch >= selectedBitset().size())
    {
        er;
        return;
    }

    ui->result->setPlainText(selectedBitset().test(ch)==1? "1" : "0");
}


void MainWindow::on_pushButton_6_clicked()
{
    if(ui->lineEdit->text()=="")
    {
        selectedBitset().reset();
        update();
        return;
    }
    if(!get_ch() || ch < 0 || (size_t)ch>=selectedBitset().size())
    {
        er;
        return;
    }

    selectedBitset().reset(ch);
    update();
}


void MainWindow::on_pushButton_7_clicked()
{
    if(ui->lineEdit->text()=="")
    {
        selectedBitset().set();
        update();
        return;
    }
    if(!get_ch()||ch < 0|| (size_t)ch >= selectedBitset().size())
    {
        er;
        return;
    }

    selectedBitset()    .set(ch);
    update();
}


void MainWindow::on_pushButton_8_clicked()
{
    ui->result->setPlainText(QString::fromStdString(std::to_string(value_1.size())));
}


void MainWindow::on_pushButton_11_clicked(){
    ui->result->setPlainText(QString::fromStdString((value_1 & value_2).to_string()));
}

void MainWindow::on_pushButton_12_clicked(){
    ui->result->setPlainText(QString::fromStdString((value_1 | value_2).to_string()));
}

void MainWindow::on_pushButton_13_clicked(){
    ui->result->setPlainText(QString::fromStdString((value_1 ^ value_2).to_string()));
}

void MainWindow::on_pushButton_14_clicked(){
    ui->result->setPlainText(QString::fromStdString((~value_1).to_string()));
}


void MainWindow::on_pushButton_10_clicked()
{
    ui->result->setPlainText(QString::number(value_1.to_ulong()));
}

