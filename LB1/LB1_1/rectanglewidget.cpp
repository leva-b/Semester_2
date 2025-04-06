#include "rectanglewidget.h"
#include <QPainter>
#include <QTimer>
#include <QGridLayout>

const int FPS = 60;
const int SPEED = 2; // Скорость перемещения

RectangleWidget::RectangleWidget(QWidget *parent)
    : QWidget(parent), rectangle(50, 50, 100, 60), dx(0), dy(0) {

    setFocusPolicy(Qt::StrongFocus); // Позволяем виджету получать фокус ввода

    // Создаем таймер для плавного обновления позиции
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RectangleWidget::updatePosition);
    timer->start(1000 / FPS);

    // Создаем кнопки
    QPushButton *upButton = new QPushButton("Вверх", this);
    QPushButton *downButton = new QPushButton("Вниз", this);

    upButton->setFixedSize(50, 30);
    downButton->setFixedSize(50, 30);

    connect(upButton, &QPushButton::pressed, this, &RectangleWidget::moveUp);
    connect(downButton, &QPushButton::pressed, this, &RectangleWidget::moveDown);

    connect(upButton, &QPushButton::released, this, &RectangleWidget::stop);
    connect(downButton, &QPushButton::released, this, &RectangleWidget::stop);

    // Создаём сеточный макет
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    upButton->setFixedSize(300, 70);
    downButton->setFixedSize(300, 70);
    buttonLayout->addWidget(upButton);
    buttonLayout->addWidget(downButton);

    buttonLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

 void RectangleWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    int width = this->width();
    int height = this->height();
    painter.fillRect(0, height / 1.7, width, height - height / 1.7, Qt::blue);
     // Замените на нужный цвет
    painter.setBrush(Qt::yellow);
    painter.drawRect(rectangle); // Рисуем прямоугольник на основе его положения
}

void RectangleWidget::updatePosition() {
    rectangle.moveLeft(rectangle.left() + dx);
    rectangle.moveTop(rectangle.top() + dy);

    if (rectangle.top() < 0) {
        rectangle.moveTop(0);
    }
    if (rectangle.bottom() > height()) {
        rectangle.moveTop(height() - rectangle.height());
    }

}


void RectangleWidget::moveUp() {
    dy = -SPEED;
}

void RectangleWidget::moveDown() {
    dy = SPEED;
}

void RectangleWidget::stop() {
    dy = 0;
}
