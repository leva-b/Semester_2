#ifndef STARS_H
#define STARS_H

#include "polygon.h"
class Stars: public Polygon
{
public:
    QMenu* createContextMenu(QWidget *parent)override;
    void change();
    Stars(const QPoint& startPosition, const QPoint& end, int rays, const QColor& color);
    void draw(QPainter &painter) override;
    double area() const override;
    void scale(double factor, const QPoint& center) override;


private:
    int numRays;
    double innerR;
    double outerR;

    QVector<QPoint> calculateVertices(const QPoint& start);
    QVector<QPoint> calculateVertices(const QPoint& start,const QPoint& end, int rays);
    void changeIR();
    void changeOR();
};

#endif // STARS_H
