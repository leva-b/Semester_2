#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDesktopServices>
#include <QUrl>
#include <QListWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    openParentFolder = new QPushButton("Open folder", this);
    connect(openParentFolder, &QPushButton::clicked, this, &MainWindow::openFolder);

    // Создаем QListWidget для отображения файлов
    listWidget = new QListWidget(this);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::itemDoubleClicked);

    // Устанавливаем компоновщик
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(openParentFolder);
    layout->addWidget(listWidget); // Изменено на QListWidget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    delete ui;
    delete parentNode;
}

void MainWindow::openFolder() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;

    // Создаем корневой узел
    parentNode = new Node(path.split("/").last().toStdString(), Node::_nodeType::FOLDER);
    if (!parentNode) {
        qWarning() << "Failed to create parent node.";
        return;
    }

    listWidget->clear(); // Очищаем список перед добавлением новых элементов
    recursiveDirTraversal(path, parentNode, 0); // Запускаем рекурсивный обход с уровнем 0
}

void MainWindow::recursiveDirTraversal(const QString& path, Node* parentNode, int level) {
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        return;
    }

    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo& entry : entries) {
        Node* curNode = new Node(entry.fileName().toStdString(), entry.isDir() ? Node::_nodeType::FOLDER : Node::_nodeType::FILE);
        if (!curNode) {
            qWarning() << "Failed to create node for:" << entry.fileName();
            continue;
        }

        parentNode->addChild(curNode);

        // Добавляем отступы в зависимости от уровня вложенности
        QString indentation(level * 4, ' '); // 4 пробела на каждый уровень
        QString displayName = indentation + entry.fileName();

        // Добавляем элементы в QListWidget
        QListWidgetItem* item = new QListWidgetItem(displayName);
        item->setData(Qt::UserRole, entry.absoluteFilePath()); // Сохраняем путь к файлу
        listWidget->addItem(item);

        if (entry.isDir()) {
            recursiveDirTraversal(entry.absoluteFilePath(), curNode, level + 1); // Увеличиваем уровень вложенности
        }
    }
}
void MainWindow::itemDoubleClicked(QListWidgetItem* item) {
    QString filePath = item->data(Qt::UserRole).toString(); // Получаем полный путь
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath)); // Открываем файл
}
