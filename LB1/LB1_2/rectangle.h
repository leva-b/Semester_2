#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QWidget>
#include <QPainter>
#include "polygon.h"

class Rectangle : public Polygon {
    Q_OBJECT

public:
    Rectangle(const QPoint& topLeft, const QPoint& bottomRight, const QColor& color = Qt::blue);
    void draw(QPainter& painter) override;
    void scale(double factor, const QPoint& center) override;
private:
    double area() const override;



};

#endif // RECTANGLE_H
