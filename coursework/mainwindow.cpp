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
    mazeWidget = new MazeWidget(this);
    graphWidget = new GraphWidget(this);

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
    connect(toggleButton, &QPushButton::clicked, this, &MainWindow::toggleViews);

    // Основной layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(toggleButton);
    layout->addWidget(stackedWidget);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    resize(800, 600);
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
        graphWidget->updateGraphFromMaze(mazeWidget->getMazeData());
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

MainWindow::~MainWindow(){
    delete ui;
}
