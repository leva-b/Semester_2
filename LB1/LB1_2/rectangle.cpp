#include "rectangle.h"

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight, const QColor& color)
    : Polygon({topLeft, QPoint(bottomRight.x(), topLeft.y()), bottomRight, QPoint(topLeft.x(), bottomRight.y())}, color) {
    position = QPoint((topLeft.x()+bottomRight.x())/2,(topLeft.y()+bottomRight.y())/2);}

double Rectangle::area() const {
    double result = std::abs((vertices[1].x() - vertices[0].x())*(vertices[2].y() - vertices[1].y()));
    return result*scaleFactor;
}

void Rectangle::draw(QPainter& painter) {
    painter.setPen(QPen(color, 3));
    painter.setBrush(Qt::NoBrush);
    QPolygon polygon;
    for(const QPoint& vertice: vertices){
        polygon << (vertice - position)*scaleFactor + position;
    }
    painter.drawPolygon(polygon);
}

void Rectangle::scale(double factor, const QPoint& center){
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;
}
