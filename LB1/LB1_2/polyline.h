#ifndef POLYLINE_H
#define POLYLINE_H

#include "shap.h"
#include <QVector>

class Polyline : public Shap
{
public:
    Polyline(const QVector<QPoint>& points, const QColor& color = Qt::black);
    Polyline(const Polyline&) = default;

    QString typeName() const override { return "Polyline"; }
    void draw(QPainter& painter) override;
    QRect boundingRect() const override;
    bool contains(const QPoint& point) const override;
    Shap* clone() const override;

    void move(const QPoint& offset) override;
    void rotate(double angle) override;
    void scale(double factor, const QPoint& center) override;

    // Методы для интерактивного построения
    void addPoint(const QPoint& point);
    void setPoints(const QVector<QPoint>& points);
    QVector<QPoint> points() const { return m_points; }

protected:
    double area() const override { return 0.0; }
    double perimeter() const override;

private:
    QVector<QPoint> m_points;
    QRect computeBoundingRect() const;
};

#endif // POLYLINE_H
