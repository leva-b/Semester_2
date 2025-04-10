#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QSpinBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDefaultData();

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // === Первая таблица ===
    tableWidget_first = new QTableWidget(this);

    tableWidget_first->setEditTriggers(QAbstractItemView::NoEditTriggers); // Запрет редактирования
    tableWidget_first->setSelectionBehavior(QAbstractItemView::SelectItems);

    tableWidget_first->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget_first->verticalHeader()->setVisible(true);

    mainLayout->addWidget(tableWidget_first, 4);

    // === Кнопки ===
    QHBoxLayout *pairButtonsLayout = new QHBoxLayout();
    addRow = new QPushButton("Add pair", this);
    pushBackRow = new QPushButton("Push back row", this);
    removeRow = new QPushButton("Remove pair", this);
    pairButtonsLayout->addWidget(addRow);
    pairButtonsLayout->addWidget(removeRow);
    pairButtonsLayout->addWidget(pushBackRow);
    //QHBoxLayout *itemButtonsLayout = new QHBoxLayout();
    addItem = new QPushButton("Add item", this);
    pushBackItem = new QPushButton("Push back item", this);
    removeItem = new QPushButton("Remove item", this);
    pairButtonsLayout->addWidget(addItem);
    pairButtonsLayout->addWidget(removeItem);
    pairButtonsLayout->addWidget(pushBackItem);
    mainLayout->addLayout(pairButtonsLayout);



    // === Вторая таблица===
    tableWidget_second = new QTableWidget(this);
    //mainLayout->addLayout(itemButtonsLayout);

    tableWidget_second->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget_second->setSelectionBehavior(QAbstractItemView::SelectItems);

    tableWidget_second->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget_second->verticalHeader()->setVisible(true);
    mainLayout->addWidget(tableWidget_second, 4);
    updateTables();

    connect(addRow, &QPushButton::clicked, this, &MainWindow::addRowClicked);
    connect(removeRow, &QPushButton::clicked, this, &MainWindow::removeRowClicked);
    connect(addItem, &QPushButton::clicked, this, &MainWindow::addItemClicked);
    connect(removeItem, &QPushButton::clicked, this, &MainWindow::removeItemClicked);
    connect(pushBackRow, &QPushButton::clicked, this, /*&MainWindow::pushBackRowClicked*/[this](){
        data.emplace_back(Vector<int>(), Vector<Pair<int,double>>());
        updateTables();
    });
    connect(pushBackItem, &QPushButton::clicked, this, &MainWindow::pushBackItemClicked);
}


void MainWindow::pushBackItemClicked() {
    if (tableWidget_first->currentRow() != -1) {
        // Для первой таблицы (int)
        bool ok;
        int value = QInputDialog::getInt(this, "Add Item",
                                         "Enter integer value:",
                                         0, // default value
                                         -2147483647, 2147483647, // min/max
                                         1, // step
                                         &ok);
        if (ok) {
            data[tableWidget_first->currentRow()].first.push_back(std::move(value));
            updateTables();
        }
    }
    else if (tableWidget_second->currentRow() != -1) {
        // Для второй таблицы (Pair<int, double>)
        QDialog dialog(this);
        dialog.setWindowTitle("Add Pair");

        QFormLayout form(&dialog);

        QSpinBox *intSpin = new QSpinBox(&dialog);
        intSpin->setRange(-2147483647, 2147483647);
        QDoubleSpinBox *doubleSpin = new QDoubleSpinBox(&dialog);
        doubleSpin->setRange(-1.7e308, 1.7e308);
        doubleSpin->setDecimals(4);

        form.addRow("Integer:", intSpin);
        form.addRow("Double:", doubleSpin);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);

        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            data[tableWidget_second->currentRow()].second.push_back(
                Pair<int, double>(intSpin->value(), doubleSpin->value()));
            updateTables();
        }
    }
    else {
        QMessageBox::warning(this, "Error", "Select a row please");
    }
}

void MainWindow::addItemClicked() {
    if (tableWidget_first->currentItem() != nullptr) {
        bool ok;
        int value = QInputDialog::getInt(this, "Add Item",
                                         "Enter integer value:",
                                         0, -2147483647, 2147483647, 1, &ok);
        if (ok) {
            data[tableWidget_first->currentItem()->row()].first.insert(
                data[tableWidget_first->currentItem()->row()].first.begin() +
                    tableWidget_first->currentItem()->column(), std::move(value));
            updateTables();
        }
    }else if (tableWidget_second->currentItem() != nullptr) {
        QDialog dialog(this);
        dialog.setWindowTitle("Add Pair");

        QFormLayout form(&dialog);

        QSpinBox *intSpin = new QSpinBox(&dialog);
        intSpin->setRange(-2147483647, 2147483647);
        QDoubleSpinBox *doubleSpin = new QDoubleSpinBox(&dialog);
        doubleSpin->setRange(-1.7e308, 1.7e308);
        doubleSpin->setDecimals(4);

        form.addRow("Integer:", intSpin);
        form.addRow("Double:", doubleSpin);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);

        connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            data[tableWidget_second->currentItem()->row()].second.emplace(
                data[tableWidget_second->currentItem()->row()].second.begin() +
                    tableWidget_second->currentItem()->column(),
                intSpin->value(), doubleSpin->value());
            updateTables();
        }
    }else {
        QMessageBox::warning(this, "Error", "Select an item please");
    }
}

void MainWindow::addRowClicked(){
    qDebug() << tableWidget_first->currentRow() << ' ' << tableWidget_second->currentRow();
    if(tableWidget_first->currentRow() != -1){
        data.emplace(data.begin() + tableWidget_first->currentRow(), Vector<int>(), Vector<Pair<int,double>>());
    }else if(tableWidget_second->currentRow() != -1){
        data.emplace(data.begin() + tableWidget_second->currentRow(), Vector<int>(), Vector<Pair<int,double>>());
    }else{
        data.emplace_back(Vector<int>(), Vector<Pair<int,double>>());
    }

    updateTables();
}

void MainWindow::removeRowClicked(){
    if(tableWidget_first->currentRow() != -1){
        data.erase(data.begin() + tableWidget_first->currentRow());
    }else if(tableWidget_second->currentRow() != -1){
        data.erase(data.begin() + tableWidget_second->currentRow());
    }else{
        QMessageBox::warning(nullptr, "Error", "Select a row please");
        return;
    }

    updateTables();
}

void MainWindow::removeItemClicked(){
    if(tableWidget_first->currentItem() != nullptr){
        (data[tableWidget_first->currentItem()->row()].first)
        .erase((data[tableWidget_first->currentItem()->row()].first).begin()
                    + tableWidget_first->currentItem()->column());
        updateTables();

    }else if(tableWidget_second->currentItem() != nullptr){
        (data[tableWidget_second->currentItem()->row()].second)
        .erase((data[tableWidget_second->currentItem()->row()].second).begin()
                     + tableWidget_second->currentItem()->column());
        updateTables();
    }else{
         QMessageBox::warning(nullptr, "Error", "Select an item please");
    }
}

void MainWindow::initDefaultData()
{

    // Создаем тестовые данные
    Vector<int> firstVec1;
    firstVec1.push_back(10);
    firstVec1.push_back(20);
    firstVec1.push_back(30);
    Vector<Pair<int, double>> secondVec1;
    secondVec1.emplace_back(1, 1.5);
    secondVec1.emplace_back(2, 2.5);
    data.emplace_back(firstVec1, secondVec1);
    qDebug() << "method";
    Vector<int> firstVec2;
    firstVec2.push_back(40);
    firstVec2.push_back(50);
    Vector<Pair<int, double>> secondVec2;
    secondVec2.emplace_back(3, 3.5);

    data.emplace_back(std::move(firstVec2), std::move(secondVec2));

}

void MainWindow::updateTables() {
    // Очищаем таблицы
    tableWidget_first->clear();
    tableWidget_second->clear();

    if (this->data.empty()) return;

    // === Первая таблица ===
    // Находим максимальную длину для вектора (чтобы задать кол-во столбцов)
    size_t maxFirstCols = 0;
    for (size_t i = 0; i < this->data.size(); ++i) {
        if (this->data[i].first.size() > maxFirstCols)
            maxFirstCols = this->data[i].first.size();
    }

    tableWidget_first->setRowCount(this->data.size());
    tableWidget_first->setColumnCount(maxFirstCols);

    // Устанавливаем заголовки строк (индексы пар)
    QStringList rowLabels;
    for (size_t i = 0; i < this->data.size(); ++i)
        rowLabels << QString::number(i);
    tableWidget_first->setVerticalHeaderLabels(rowLabels);
    //tableWidget_second->setVerticalHeaderLabels(rowLabels);

    // Устанавливаем заголовки столбцов
    QStringList colLabels;
    for (size_t i = 0; i < maxFirstCols; ++i)
        colLabels << QString::number(i);
    tableWidget_first->setHorizontalHeaderLabels(colLabels);

    // Заполняем таблицу
    for (size_t row = 0; row < this->data.size(); ++row) {
        Vector<int>& vec = this->data[row].first;
        for (size_t col = 0; col < vec.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(vec[col]));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            tableWidget_first->setItem(row, col, item);
        }
        // Заполняем пустые ячейки, если векторы разной длины
        for (size_t col = vec.size(); col < maxFirstCols; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem("-");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            tableWidget_first->setItem(row, col, item);
        }
    }

    // === Вторая таблица ===

    size_t maxSecondCols = 0;
    for (size_t i = 0; i < this->data.size(); ++i) {
        if (this->data[i].second.size() > maxSecondCols)
            maxSecondCols = this->data[i].second.size();
    }

    tableWidget_second->setRowCount(this->data.size());
    tableWidget_second->setColumnCount(maxSecondCols);

    QStringList rowLabelsSecond;
    for (size_t i = 0; i < this->data.size(); ++i)
        rowLabelsSecond << QString::number(i);
    tableWidget_second->setVerticalHeaderLabels(rowLabelsSecond);

    for (size_t row = 0; row < this->data.size(); ++row) {
        Vector<Pair<int, double>>& pairs = this->data[row].second;
        for (size_t col = 0; col < pairs.size(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(pairs[col].first) + QString('/') + QString::number(pairs[col].second));
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            tableWidget_second->setItem(row, col, item);
        }
        // Заполняем пустые ячейки, если векторы разной длины
        for (size_t col = pairs.size(); col < maxSecondCols; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem("-");
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            tableWidget_second->setItem(row, col, item);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
