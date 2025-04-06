#ifndef POLYGON_H
#define POLYGON_H
#include "shap.h"

class Polygon: public Shap
{
    Q_OBJECT

    bool contains(const QPoint &point) const override;
public:
    Polygon(const QVector<QPoint>& vertices, const QColor& color = Qt::blue);
protected:
    QVector<QPoint> vertices;
    void move(const QPoint& offset) override;
    double perimeter() const override;
    void rotate(double angle) override;
    void scale(double factor, const QPoint& center) override;
    void draw(QPainter& painter) override = 0;
    double TriangleArea(const QPoint& p1, const QPoint& p2,const QPoint& p3)const;
};

#endif // POLYGON_H
