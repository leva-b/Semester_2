#include "mainwindow.h"
#include "mazewidget.h"
#include "graphwidget.h"
#include <QVBoxLayout>
#include  <QPushButton>
#include <ui_mainwindow.h>

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Создаем виджеты
    std::vector<std::vector<char>> maze = {
        {'1', '1', '1', '1', '1', '1', '1', '1', '1'},
        {'0', '0', '0', '0', '0', '0', '0', '1', '1'},
        {'1', '0', '1', '1', '1', '0', '1', '1', '1'},
        {'1', '0', '0', '0', '1', '0', '0', '1', '1'},
        {'1', '1', '1', '0', '1', '1', '0', '1', '1'},
        {'1', '0', '0', '0', '0', '0', '0', '1', '1'},
        {'1', '0', '1', '1', '1', '1', '0', '1', '1'},
        {'1', '0', '1', '1', '1', '1', '1', '1', '1'},
        {'1', '0', '1', '1', '1', '1', '1', '1', '1'}
    };
    parser = new MazeFromFileParser(maze, '1', '0');

    // Создание виджетов с передачей парсера
    mazeWidget = new MazeWidget(this, parser);
    graphWidget = new GraphWidget(this, parser);

    // Настройка эффектов прозрачности
    QGraphicsOpacityEffect *mazeEffect = new QGraphicsOpacityEffect(mazeWidget);
    mazeWidget->setGraphicsEffect(mazeEffect);
    mazeEffect->setOpacity(1.0);

    QGraphicsOpacityEffect *graphEffect = new QGraphicsOpacityEffect(graphWidget);
    graphWidget->setGraphicsEffect(graphEffect);
    graphEffect->setOpacity(0.0);

    // Настройка stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(mazeWidget);
    stackedWidget->addWidget(graphWidget);
    stackedWidget->setCurrentWidget(mazeWidget);

    // Кнопка переключения
    QPushButton *toggleButton = new QPushButton("Switch View", this);
    // Основной layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(toggleButton);
    layout->addWidget(stackedWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    resize(800, 600);
    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleViews);


    connect(graphWidget, &GraphWidget::requestParser,
            this, &MainWindow::updateGraph);
}

void MainWindow::toggleViews() {
    if (animation && animation->state() == QPropertyAnimation::Running) {
        return;
    }

    QGraphicsOpacityEffect *outEffect, *inEffect;
    QWidget *outWidget, *inWidget;

    if (isMazeView) {
        outWidget = mazeWidget;
        inWidget = graphWidget;
        //graphWidget->updateGraphClicked(mazeWidget->getConstParser());
    } else {
        outWidget = graphWidget;
        inWidget = mazeWidget;
    }

    outEffect = qobject_cast<QGraphicsOpacityEffect*>(outWidget->graphicsEffect());
    inEffect = qobject_cast<QGraphicsOpacityEffect*>(inWidget->graphicsEffect());

    // Настройка анимации для текущего виджета (исчезание)
    animation = new QPropertyAnimation(outEffect, "opacity");
    animation->setDuration(500);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    // Настройка анимации для нового виджета (появление)
    QPropertyAnimation *inAnimation = new QPropertyAnimation(inEffect, "opacity");
    inAnimation->setDuration(500);
    inAnimation->setStartValue(0.0);
    inAnimation->setEndValue(1.0);
    inAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Связываем анимации
    connect(animation, &QPropertyAnimation::finished, [this, inWidget, inAnimation]() {
        stackedWidget->setCurrentWidget(inWidget);
        inAnimation->start();
    });

    // Запускаем анимацию
    animation->start();
    isMazeView = !isMazeView;
}

void MainWindow::updateGraph() {
    qDebug() << "updateGraph called";
    qDebug() << "graphWidget:" << graphWidget;
    qDebug() << "parser:" << parser;
    if (!graphWidget) {
        qDebug() << "graphWidget is null";
        return;
    }
    if (!parser) {
        qDebug() << "parser is null";
        return;
    }

    try {
        graphWidget->updateGraphFromMaze();
        qDebug() << "Graph updated successfully";
    } catch (const std::exception& e) {
        qDebug() << "Exception in updateGraphFromMaze:" << e.what();
    }
    qDebug() << "updateGraph : graph" << graphWidget << parser;
    if(graphWidget && parser) {
        graphWidget->updateGraphFromMaze();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}
