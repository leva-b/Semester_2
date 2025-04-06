#include "ship.h"
#include "rectanglewidget.h"

Ship::Ship(QWidget *parent): RectangleWidget(parent) {}

void Ship::updatePosition(){
    rectangle.moveLeft(rectangle.left() + dx);
    rectangle.moveTop(rectangle.top() + dy);

    if (rectangle.top() < height()/3) {
        rectangle.moveTop(height()/3);
    }
    if (rectangle.top() > height()*2/3) {
        rectangle.moveTop(height()*2/3);
    }


    update();
}

void Ship::paintEvent(QPaintEvent* parent){
    RectangleWidget::paintEvent(parent);
    QPainter painter(this);



    painter.setPen(QPen(QColor(139, 69, 19), 2));
    painter.setBrush(QBrush(QColor(139, 69, 19)));

    int width = this->width();
    int height = this->height();

    QPolygon polygon;
    polygon << QPoint(width/2,height*2/3)
            << QPoint(width/2 + 200, height*2/3)
            << QPoint(width/2 + 200,height*2/3 + 100)
            << QPoint(width/2 - 80, height*2/3 + 100)
            << QPoint(width/2 - 140, height*2/3)
            << QPoint(width/2, height*2/3);


    painter.setPen(QPen(Qt::blue, 2));
    painter.setBrush(QColor("brown"));
    painter.drawPolygon(polygon);

    rectangle.moveLeft(width / 1.9);
    if(perfectCur != height)
        rectangle.moveTo(width / 1.9,  (rectangle.y()/perfectCur) * height);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(width / 1.9, height * 2/3, this->width() / 1.9, this->height() / 3);
    perfectCur = height;
}


