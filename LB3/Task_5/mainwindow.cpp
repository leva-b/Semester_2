#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    openParentFolder = new QPushButton("Open folder");
    connect(openParentFolder, &QPushButton::clicked, this, &MainWindow::openFolder);

    treeWidget = new QTreeWidget(this); // Создаем QTreeWidget
    treeWidget->setHeaderLabel("File System"); // Устанавливаем заголовок

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(openParentFolder);
    layout->addWidget(treeWidget);


    // Подключаем двойное нажатие на элемент дерева для открытия файла
    connect(treeWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::openFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFolder() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return; // Проверяем, выбрана ли папка

    treeWidget->clear();

    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, path.split("/").last()); // Устанавливаем имя корневого элемента
    recursiveDirTraversal(path, rootItem);
    showFileSystem();
}

void MainWindow::showFileSystem() {

    treeWidget->expandAll(); // Раскрываем все элементы
}

void MainWindow::openFile(QTreeWidgetItem *item) {
    QString filePath = item->data(0, Qt::UserRole).toString(); // Получаем полный путь
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); // Открываем файл
}


void MainWindow::recursiveDirTraversal(const QString& path, QTreeWidgetItem* parentItem) {
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return;
    }

    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& entry : entries) {

        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
        item->setText(0, entry.fileName());
        item->setData(0, Qt::UserRole, entry.absoluteFilePath()); // Сохраняем полный путь в данных элемента

        if (entry.isDir()) {
            recursiveDirTraversal(entry.absoluteFilePath(), item);
        }
    }
}
