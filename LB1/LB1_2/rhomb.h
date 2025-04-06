#ifndef RHOMB_H
#define RHOMB_H
#include "polygon.h"

class Rhomb: public Polygon
{
public:
    Rhomb(const QPoint& start, const QPoint& end, const QColor& color = Qt::blue);
    void draw(QPainter& painter) override;
    QMenu* createContextMenu(QWidget *parent)override;
private:
    double area() const override;
    void change();
    QVector<QPoint> calculateVertices(int d1, int d2);
    QVector<QPoint> calculateVertices(const QPoint& start, const QPoint& end);
    void scale(double factor, const QPoint& center)override;
};

#endif // RHOMB_H
