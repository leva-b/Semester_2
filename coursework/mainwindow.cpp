#include "mainwindow.h"
#include "mazewidget.h"
#include "graphwidget.h"
#include <QVBoxLayout>
#include  <QPushButton>
#include <ui_mainwindow.h>
#include <QPropertyAnimation>

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

    QWidget* mazeContainer = new QWidget(this);
    QWidget* graphContainer = new QWidget(this);

    // Настраиваем layout для контейнеров
    QVBoxLayout* mazeContainerLayout = new QVBoxLayout(mazeContainer);
    QVBoxLayout* graphContainerLayout = new QVBoxLayout(graphContainer);
    mazeContainerLayout->setContentsMargins(0, 0, 0, 0);
    graphContainerLayout->setContentsMargins(0, 0, 0, 0);

    // Создаем основные виджеты ОДИН РАЗ
    mazeWidget = new MazeWidget(mazeContainer, parser);
    graphWidget = new GraphWidget(graphContainer, parser);

    // Добавляем в контейнеры
    mazeContainerLayout->addWidget(mazeWidget);
    graphContainerLayout->addWidget(graphWidget);

    // Создаем scroll areas
    mazeScroll = new QScrollArea(this);
    graphScroll = new QScrollArea(this);
    mazeScroll->setWidget(mazeContainer);
    graphScroll->setWidget(graphContainer);
    mazeScroll->setWidgetResizable(true);
    graphScroll->setWidgetResizable(true);

    // Настройка эффектов прозрачности (применяем к контейнерам)
    QGraphicsOpacityEffect *mazeEffect = new QGraphicsOpacityEffect(mazeContainer);
    mazeEffect->setOpacity(1.0);
    mazeContainer->setGraphicsEffect(mazeEffect);

    QGraphicsOpacityEffect *graphEffect = new QGraphicsOpacityEffect(graphContainer);
    graphEffect->setOpacity(0.0);  // Начальная прозрачность для graphContainer
    graphContainer->setGraphicsEffect(graphEffect);

    // Принудительно обновляем виджеты
    mazeContainer->update();
    graphContainer->update();
    // Настройка stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(mazeScroll);
    stackedWidget->addWidget(graphScroll);
    stackedWidget->setCurrentWidget(mazeScroll);

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

    connect(mazeWidget, &MazeWidget::parserUpdated, this, &MainWindow::updateParser);

    connect(graphWidget, &GraphWidget::requestParser,
            this, &MainWindow::updateGraph);
}

void MainWindow::toggleViews() {
    if (animationGroup && animationGroup->state() == QAbstractAnimation::Running) {
        return;
    }

    // Получаем текущий и следующий контейнеры
    QWidget* currentContainer = qobject_cast<QScrollArea*>(stackedWidget->currentWidget())->widget();
    QWidget* nextContainer = qobject_cast<QScrollArea*>(
                                 isMazeView ? stackedWidget->widget(1) : stackedWidget->widget(0))->widget();

    // Гарантируем наличие эффектов
    if (!currentContainer->graphicsEffect()) {
        currentContainer->setGraphicsEffect(new QGraphicsOpacityEffect(currentContainer));
    }
    if (!nextContainer->graphicsEffect()) {
        nextContainer->setGraphicsEffect(new QGraphicsOpacityEffect(nextContainer));
    }

    // Получаем эффекты
    QGraphicsOpacityEffect *outEffect = qobject_cast<QGraphicsOpacityEffect*>(currentContainer->graphicsEffect());
    QGraphicsOpacityEffect *inEffect = qobject_cast<QGraphicsOpacityEffect*>(nextContainer->graphicsEffect());

    // Устанавливаем начальные значения
    outEffect->setOpacity(1.0);
    inEffect->setOpacity(0.0);

    // Создаем группу анимаций
    animationGroup = new QParallelAnimationGroup(this);

    // Анимация исчезновения текущего виджета
    QPropertyAnimation *outAnimation = new QPropertyAnimation(outEffect, "opacity", animationGroup);
    outAnimation->setDuration(1000);
    outAnimation->setStartValue(1.0);
    outAnimation->setEndValue(0.0);
    outAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Анимация появления следующего виджета
    QPropertyAnimation *inAnimation = new QPropertyAnimation(inEffect, "opacity", animationGroup);
    inAnimation->setDuration(1000);
    inAnimation->setStartValue(0.0);
    inAnimation->setEndValue(1.0);
    inAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    // Переключаем виджет при старте анимации
    connect(animationGroup, &QParallelAnimationGroup::stateChanged,
            [this](QAbstractAnimation::State newState, QAbstractAnimation::State) {
                if (newState == QAbstractAnimation::Running) {
                    stackedWidget->setCurrentWidget(isMazeView ? graphScroll : mazeScroll);
                }
            });

    // Удаляем группу после завершения
    connect(animationGroup, &QParallelAnimationGroup::finished, [this]() {
        animationGroup->deleteLater();
        animationGroup = nullptr;
    });

    // Добавляем анимации в группу
    animationGroup->addAnimation(outAnimation);
    animationGroup->addAnimation(inAnimation);

    // Запускаем анимацию
    animationGroup->start();
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

void MainWindow::updateParser(MazeFromFileParser* newParser) {
    // Удаляем старый парсер, если он существует
    if (parser) {
        delete parser;
    }

    // Устанавливаем новый парсер
    parser = newParser;

    graphWidget->setParser(parser);
}

MainWindow::~MainWindow(){
    delete ui;
    if (animationGroup) {
        animationGroup->stop();
        animationGroup->deleteLater();
    }
}
