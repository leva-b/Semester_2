#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QWidget>
#include <QPainter>
#include "polygon.h"

class Rectangle : public Polygon {

public:
    QString typeName() const override { return "Rectangle"; }
    Rectangle(const QPoint& topLeft, const QPoint& bottomRight, const QColor& color = Qt::blue);
    void draw(QPainter& painter) override;
    void scale(double factor, const QPoint& center) override;
    Shap* clone() const override;

    Rectangle(const Rectangle&) = default;
private:
    double area() const override;


};

#endif // RECTANGLE_H
