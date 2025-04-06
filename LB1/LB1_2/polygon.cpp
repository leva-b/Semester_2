#include "polygon.h"
#include <QWidget>
#include <QPainter>


Polygon::Polygon(const QVector<QPoint>& vertices, const QColor& color)
    : Shap(color), vertices(vertices) {
    this->originalColor = color;
}

double Polygon::perimeter() const {
    double perimeter = 0;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) {
        const QPoint& p1 = vertices[i];
        const QPoint& p2 = vertices[(i + 1) % n];
        perimeter += std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
    }
    return perimeter*scaleFactor;
}

void Polygon::move(const QPoint& offset) {
    for (QPoint& vertex : vertices) {
        vertex += offset;
    }
    position += offset;
}

void Polygon::rotate(double angle) {
    rotation += angle;
}

void Polygon::scale(double factor, const QPoint& center) {
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;

    for (QPoint& vertex : vertices) {
        QPoint relative = vertex - center;
        vertex = center + relative * factor;
    }
}


bool Polygon::contains(const QPoint &point) const {
    QPolygon polygon;
    for (const QPoint& vertice: vertices) {
        QPoint scaledVertice = (vertice - position) * scaleFactor + position;
        polygon << scaledVertice;
    }

    QTransform transform;
    transform.translate(center().x(), center().y());
    transform.rotate(rotation);
    transform.translate(-center().x(), -center().y());

    QPolygon transformedPolygon = transform.map(polygon);

    return transformedPolygon.containsPoint(point, Qt::OddEvenFill);
}


double Polygon::TriangleArea(const QPoint& p1,const QPoint& p2,const QPoint& p3)const {
    double a, b, c;
    a = std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
    b = std::hypot(p2.x() - p3.x(), p2.y() - p3.y());
    c = std::hypot(p3.x() - p1.x(), p3.y() - p1.y());
    double p = (a+b+c)/2.0;
    double result = sqrt(p*(p - a)*(p - b)*(p - c));
    return result;
}
