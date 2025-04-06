#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QPoint>
#include <QColor>
#include "polygon.h"
class Triangle: public Polygon
{
public:
    explicit Triangle(const QPoint& point1, const QPoint& point2, const QPoint& point3, const QColor& color = Qt::blue);
    void draw(QPainter& painter) override;
    QMenu* createContextMenu(QWidget *parent)override;
private:
    void scale(double factor, const QPoint& center) override;
    double area() const override;
    void change();

};

#endif // TRIANGLE_H
