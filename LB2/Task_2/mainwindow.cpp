#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include "disk.h"
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->add);
    buttonGroup->addButton(ui->edit);
    ui->add->setChecked(true);
    // Заполнение ячеек
    QStringList headers;
    headers << "№" << "Type" << "Name" << "Author" << "Price" << "Notation";
    ui->tableWidget->verticalHeader()->setVisible(false); // Скрыть заголовок строк
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setColumnWidth(0, 25);  // (№)
    ui->tableWidget->setColumnWidth(1, 160); // (Type)
    ui->tableWidget->setColumnWidth(2, 160); // (Name)
    ui->tableWidget->setColumnWidth(3, 160); // (Author)
    ui->tableWidget->setColumnWidth(4, 80);  // (Price)
    ui->tableWidget->setColumnWidth(5, 145);

    ui->comboBox->addItem("Film", Disk::film);
    ui->comboBox->addItem("Video", Disk::video);
    ui->comboBox->addItem("Game", Disk::game);
    ui->comboBox->addItem("Music", Disk::music);
    ui->comboBox->addItem("Application", Disk::application);
    ui->comboBox->addItem("Software", Disk::software);

    ui->comboBoxSort->addItem("author");
    ui->comboBoxSort->addItem("name");

    ui->comboBoxSearch->addItem("author");
    ui->comboBoxSearch->addItem("name");

    connect(ui->delete_2, &QPushButton::clicked, this, &MainWindow::delete_row);
    connect(ui->showAll, &QPushButton::clicked, this, &MainWindow::show_all);
    connect(ui->openFile, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui->add_edit, &QPushButton::clicked, this, &MainWindow::add_edit);
    connect(ui->saveFile, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(ui->sortBy, &QPushButton::clicked, this, &MainWindow::sort_disks);
    connect(ui->searchBy, &QPushButton::clicked, this, &MainWindow::search_disks);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveFile(){
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        try{
            throw std::runtime_error("Cannot open file for reading: " + file.errorString().toStdString());
        }catch(const std::runtime_error& e){
            QMessageBox::warning(nullptr, "Error", "Cannot open file for reading:");
            return;
        }
    }
    QTextStream out(&file);
    for(Disk& disk: _disks){

        out << disk.GetType() << '\n' << disk.GetName() << '\n' << disk.GetAuthor() << '\n' << QString::number(disk.GetPrice()) << '\n' << disk.GetRemark() << '\n'; // Записываем новые данные
    }
    file.close();
}

void MainWindow::add_edit(){
    int currentRow = ui->tableWidget->currentRow();
    QString selectedType = ui->comboBox->currentText();
    float price = ui->priceEdit->value();
    QString name = ui->nameEdit->text();
    QString remark = ui->remarkEdit->toPlainText();
    QString author = ui->authorEdit->text();
    bool action = ui->add->isChecked() ? true : false;

    if(name.isEmpty() || remark.isEmpty() || name.isEmpty() || remark.isEmpty() || author.isEmpty()){
        QMessageBox::warning(this, "Error", "Please enter details");
        return;
    }
    if(action){
        _disks.push_back(Disk(selectedType, name, author, price/*.toFloat()*/, remark));
    }else{
        if (currentRow == -1) {
            QMessageBox::warning(this, "Error", "Please select a row");
            return;
        }

        // Получение элемента из текущей строки
        QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
        if (!item) {
            QMessageBox::warning(this, "Error", "No valid item found in the selected row");
            return;
        }

        int row = item->text().toInt();
        _disks[row] = Disk(selectedType, name, author, price, remark);
    }
    ui->priceEdit->clear();
    ui->nameEdit->clear();
    ui->remarkEdit->clear();
    ui->authorEdit->clear();

    updateTable();
}

void MainWindow::openFile() {
    filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    // Проверяем, что файл не пустой и имеет расширение .txt
    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        try{
            _disks = Disk::readDisksFromFile(filePath);
            qDebug() << "reading ";
        }catch(const std::runtime_error& e){
            QMessageBox::warning(nullptr, "Error", "Cannot open file for reading:");
        }
        updateTable();
    } else {
        QMessageBox::warning(this, "Invalid File", "Please select a valid .txt file.");
    }
}

void MainWindow::delete_row() {
    int currentRow = ui->tableWidget->currentRow();

    // Проверка на то, что строка выбрана
    if (currentRow == -1) {
        QMessageBox::warning(this, "Error", "Please select a row");
        return;
    }

    // Получение элемента из текущей строки
    QTableWidgetItem *item = ui->tableWidget->item(currentRow, 0);
    if (!item) {
        QMessageBox::warning(this, "Error", "No valid item found in the selected row");
        return;
    }

    int row = item->text(   ).toInt();

    _disks.remove(row-1);
    if (!_newDisks.isEmpty()){
        _newDisks.remove(currentRow);
        for (int i = currentRow; i < _newDisks.size(); ++i) {
            _newDisks[i].second--;
        }
        updateNewTable();
    }else{
        updateTable();
    }
}

void MainWindow::show_all(){
    updateTable();
    _newDisks.clear();
}

void MainWindow::updateTable() {
    ui->tableWidget->setRowCount(_disks.size());
    qDebug() << "reading ";
    for (long long  i = 0; i < _disks.size(); ++i) {
        Disk curDisk = _disks[i];

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(i+ 1)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(curDisk.GetType()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(curDisk.GetName()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(curDisk.GetAuthor()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(curDisk.GetPrice())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(curDisk.GetRemark()));
        for (int j = 1; j <= 5; ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (item) { // Проверяем, существует ли элемент
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }
}

void MainWindow::sort_disks() {

    QMap<QString, QVector<Disk>> groupedDisks;

    for (const Disk& disk : _disks) {
        groupedDisks[disk.GetType()].append(disk);
    }


    for (auto& group : groupedDisks) {
        if (ui->comboBoxSort->currentText() == "name") {
            std::sort(group.begin(), group.end(), Disk::compare_by_name);
        } else if (ui->comboBoxSort->currentText() == "author") {
            std::sort(group.begin(), group.end(), Disk::compare_by_author);
        }
    }

    // Очищаем оригинальный вектор дисков и добавляем отсортированные группы обратно
    _disks.clear();
    for (const auto& group : groupedDisks) {
        _disks.append(group);
    }

    updateTable();
}
void MainWindow::search_disks() {
    QString searchText = ui->searchLineEdit->text();
    QString searchCriterion = ui->comboBoxSearch->currentText();

    QVector<std::pair<Disk, int>> filteredDisks;

    for (int i = 0; i < _disks.size(); ++i) {
        const Disk& disk = _disks[i];
        bool matches = false;

        if (searchCriterion == "author") {
            matches = disk.GetAuthor().contains(searchText, Qt::CaseInsensitive);
        } else if (searchCriterion == "name") {
            matches = disk.GetName().contains(searchText, Qt::CaseInsensitive);
        }

        if (matches) {
            filteredDisks.append(qMakePair(disk, i)); // Сохраняем пару (диск, индекс)
        }
    }

    // Обновляем таблицу с отфильтрованными дисками
    _newDisks = filteredDisks;
    updateNewTable();
}

void MainWindow::updateNewTable() {
    ui->tableWidget->setRowCount(_newDisks.size());

    for (long long i = 0; i < _newDisks.size(); ++i) {
        const Disk& curDisk = _newDisks[i].first; // Получаем диск
        int originalIndex = _newDisks[i].second; // Получаем оригинальный индекс

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(originalIndex + 1)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(curDisk.GetType()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(curDisk.GetName()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(curDisk.GetAuthor()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(curDisk.GetPrice())));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(curDisk.GetRemark()));

        for (int j = 1; j <= 5; ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (item) { // Проверяем, существует ли элемент
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }
}
