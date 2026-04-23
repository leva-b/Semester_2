#ifndef HEXAGON_H
#define HEXAGON_H
#include <QPoint>
#include <QColor>
#include <QPainter>
#include "polygon.h"
class Hexagon: public Polygon
{
public:
    QString typeName() const override { return "Hexagon"; }
    explicit Hexagon(const QPoint& start, const QPoint& end, const QColor& color);
    void draw(QPainter& painter) override;
    double area() const override;
    QMenu* createContextMenu(QWidget *parent)override;
    Shap* clone() const override { return new Hexagon(*this); }
private:
    void change();
    int radius;
    QVector<QPoint> calculateVertices(const QPoint& start, const QPoint& end);
    QVector<QPoint> calculateVertices(int R);
    void scale(double factor, const QPoint& center) override;
    Hexagon(const Hexagon&) = default;
};

#endif // HEXAGON_H
