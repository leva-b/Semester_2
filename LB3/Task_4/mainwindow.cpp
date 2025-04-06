#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    diskCountInput = new QLineEdit(this);
    diskCountInput->setPlaceholderText("Enter the number of disks");
    diskCountInput->setText("7"); // Начальное значение
    diskCountInput->setFixedWidth(100);
    diskCountInput->setMaxLength(2);
    updateTextColor(diskCountInput);

    resetButton = new QPushButton("Reset", this);

    nextStep = new QPushButton("Next Step");
    executeAll = new QPushButton("Execute All");

    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("[0-9]*"), this);
    diskCountInput->setValidator(validator);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setGeometry(10, 50, 200, 40);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(diskCountInput);
    layout->addSpacing(20);
    layout->addWidget(resetButton);
    layout->addSpacing(20);
    layout->addWidget(nextStep);
    layout->addSpacing(20);
    layout->addWidget(executeAll);
    layout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addItem(layout);
    mainLayout->addStretch();
    QWidget *widget = new QWidget(this);
    widget->setLayout(mainLayout);
    setCentralWidget(widget);



    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);
    connect(diskCountInput, &QLineEdit::returnPressed, this, &MainWindow::applyDiskCount);
    connect(executeAll, &QPushButton::clicked, this, [this](){
        executeAllSteps();
    });
    connect(nextStep, &QPushButton::clicked, this, [this]() {
        executeOneStep();
    });
    initializeTowers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeTowers()
{
    towers.clear();
    towers.resize(3);
    for (int i = diskCount; i >= 1; --i) {
        towers[0].push(i);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    drawTowers(painter);

    // Отрисовка перемещаемого диска
    if (isDragging && draggedDisk != -1) {
        int diskSize = (draggedDisk * (170 / diskCount));
        painter.setBrush(Qt::blue);
        painter.drawRect(dragStartPos.x() - diskSize / 2, dragStartPos.y() - diskHeight / 2, diskSize, diskHeight);
    }
}

void MainWindow::drawTowers(QPainter &painter)
{
    int towerWidth = width() / 4;
    int baseY = height() - 50;
    int diskHeight = 22; // Уменьшаем высоту диска
    int maxDiskSize = 170; // Максимальная ширина диска

    painter.setPen(Qt::black);
    painter.setBrush(Qt::gray);

    for (int i = 0; i < 3; ++i) {
        int towerX = (i + 1) * towerWidth;
        painter.drawRect(towerX - 5, baseY - 440, 10, 440); // Высота башни

        QStack<int> tower = towers[i];
        int y = baseY;

        for (int j = 0; j < tower.size(); ++j) {
            int diskSize = (tower[j] * (maxDiskSize / diskCount)); // Уменьшаем размер диска
            painter.setBrush(Qt::blue);
            painter.drawRect(towerX - diskSize / 2, y - diskHeight, diskSize, diskHeight);
            y -= diskHeight;
        }
    }
}

void MainWindow::moveDisk(int from, int to)
{
    if (!towers[from].isEmpty()) {
        int disk = towers[from].pop();
        towers[to].push(disk);
    }
}

void MainWindow::applyDiskCount()
{
    bool ok;
    moves.clear();
    textEdit->clear();
    textEdit->append("Step number " + QString::number(currentDisk) + " of " + QString::number(moves.size()));
    int count = diskCountInput->text().toInt(&ok);
    if (ok && count > 0 && count <= 20) { // Ограничение на количество дисков
        diskCount = count;
        initializeTowers();
        update();
    } else {
        QMessageBox::warning(this, "Error", "Enter a number from 1 to 20");
    }
}

void MainWindow::resetGame()
{
    currentDisk = 0;
    textEdit->clear();
    textEdit->append("Step number " + QString::number(currentDisk) + " of " + QString::number(moves.size()));
    initializeTowers();
    update();
}

void MainWindow::updateTextColor(QLineEdit *lineEdit) {
    QPalette palette = lineEdit->palette();
    QColor textColor;

    // Проверяем, светлая или темная тема
    if (palette.color(QPalette::Window).lightness() < 128) {
        textColor = Qt::white;
    } else {
        textColor = Qt::black;
    }

    lineEdit->setStyleSheet(QString("color: %1;").arg(textColor.name()));
}

void MainWindow::executeAllSteps(){
    if (moves.empty()) {
        currentDisk = 0; // Очищаем предыдущие перемещения
        generateMoves(diskCount, 1, 3, 2);
    }

    while (currentDisk < (unsigned int)moves.size()) {
        // Извлекаем следующее перемещение
        auto [fromPeg, toPeg] = moves[currentDisk];
        moveDisk(fromPeg - 1, toPeg - 1);
        //QApplication::processEvents();
        update();
        currentDisk++;
    }
     // Сброс
    textEdit->clear();
    textEdit->append("Step number " + QString::number(moves.size()) + " of " + QString::number(moves.size()));
}

void MainWindow::executeOneStep() {
    if (moves.empty()) {
        currentDisk = 0; // Очищаем предыдущие перемещения
        generateMoves(diskCount, 1, 3, 2);
    }

    if (currentDisk < (unsigned int)moves.size()) {
        // Извлекаем следующее перемещение
        auto [fromPeg, toPeg] = moves[currentDisk];
        moveDisk(fromPeg - 1, toPeg - 1); // Перемещаем диск
        update();
        currentDisk++;
    }
    textEdit->clear();
    textEdit->append("Step number " + QString::number(currentDisk) + " of " + QString::number(moves.size()));
}

void MainWindow::generateMoves(int n, int fromPeg, int toPeg, int unusedPeg) {
    if (n == 1) {
        moves.emplace_back(fromPeg, toPeg);
    } else {
        generateMoves(n - 1, fromPeg, unusedPeg, toPeg);
        moves.emplace_back(fromPeg, toPeg);
        generateMoves(n - 1, unusedPeg, toPeg, fromPeg);
    }
}
