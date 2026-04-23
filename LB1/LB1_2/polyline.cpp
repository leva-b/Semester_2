#include "polyline.h"
#include <QPainter>
#include <QPainterPath>
#include <QLineF>
#include <cmath>

Polyline::Polyline(const QVector<QPoint>& points, const QColor& color)
    : Shap(color), m_points(points)
{
    if (!m_points.isEmpty())
        m_position = m_points.first();
    originalColor = color;
}

void Polyline::draw(QPainter& painter)
{
    if (m_points.size() < 2) return;

    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(Qt::NoBrush);

    QPolygon polygon;
    for (const QPoint& pt : m_points) {
        QPoint transformed = (pt - m_position) * m_scaleFactor + m_position;
        polygon << transformed;
    }
    painter.drawPolyline(polygon);
}

QRect Polyline::boundingRect() const
{
    if (m_points.isEmpty()) return QRect();
    QPolygon scaled;
    for (const QPoint& pt : m_points) {
        scaled << ((pt - m_position) * m_scaleFactor + m_position);
    }
    return scaled.boundingRect();
}

bool Polyline::contains(const QPoint& point) const
{
    if (m_points.size() < 2) return false;
    QPainterPath path;
    path.moveTo(m_points.first());
    for (int i = 1; i < m_points.size(); ++i)
        path.lineTo(m_points[i]);
    QTransform transform;
    transform.translate(m_position.x(), m_position.y());
    transform.scale(m_scaleFactor, m_scaleFactor);
    transform.rotate(m_rotation);
    transform.translate(-m_position.x(), -m_position.y());
    QPainterPath transformed = transform.map(path);
    QPainterPathStroker stroker;
    stroker.setWidth(m_lineWidth + 5); // допуск
    QPainterPath stroked = stroker.createStroke(transformed);
    return stroked.contains(point);
}

Shap* Polyline::clone() const
{
    Polyline* cloned = new Polyline(m_points, m_lineColor);
    cloned->setLineWidth(m_lineWidth);
    cloned->setFillColor(m_fillColor);
    cloned->setScaleFactor(m_scaleFactor);
    cloned->rotate(m_rotation);
    cloned->move(m_position - cloned->center());
    return cloned;
}

void Polyline::move(const QPoint& offset)
{
    for (QPoint& pt : m_points) pt += offset;
    m_position += offset;
}

void Polyline::rotate(double angle)
{}

void Polyline::scale(double factor, const QPoint& center)
{
    if (m_scaleFactor * factor > 10) m_scaleFactor = 10;
    else if (m_scaleFactor * factor < 0.1) m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
    for (QPoint& pt : m_points) {
        QPoint delta = pt - center;
        pt = center + delta * factor;
    }
}

double Polyline::perimeter() const
{
    double len = 0.0;
    for (int i = 1; i < m_points.size(); ++i)
        len += QLineF(m_points[i-1], m_points[i]).length();
    return len * m_scaleFactor;
}

void Polyline::addPoint(const QPoint& point)
{
    m_points.append(point);
    if (m_points.size() == 1)
        m_position = point;
}

void Polyline::setPoints(const QVector<QPoint>& points)
{
    m_points = points;
    if (!points.isEmpty())
        m_position = points.first();
}
