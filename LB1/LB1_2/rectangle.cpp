#include "rectangle.h"

Rectangle::Rectangle(const QPoint& topLeft, const QPoint& bottomRight, const QColor& color)
    : Polygon({topLeft, QPoint(bottomRight.x(), topLeft.y()), bottomRight, QPoint(topLeft.x(), bottomRight.y())}, color) {
    m_position = QPoint((topLeft.x()+bottomRight.x())/2,(topLeft.y()+bottomRight.y())/2);}

double Rectangle::area() const {
    double result = std::abs((vertices[1].x() - vertices[0].x())*(vertices[2].y() - vertices[1].y()));
    return result*m_scaleFactor;
}

void Rectangle::draw(QPainter& painter) {
    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(m_fillColor);
    QPolygon polygon;
    for(const QPoint& vertice: vertices){
        polygon << (vertice - m_position)*m_scaleFactor + m_position;
    }
    painter.drawPolygon(polygon);
}

void Rectangle::scale(double factor, const QPoint& center){
    if(m_scaleFactor * factor > 10)m_scaleFactor = 10;
    else if(m_scaleFactor * factor < 0.1)m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
}

Shap* Rectangle::clone() const {
    QPoint topLeft = vertices[0];
    QPoint bottomRight = vertices[2];
    Rectangle *cloned = new Rectangle(topLeft, bottomRight, m_lineColor);

    cloned->setLineWidth(m_lineWidth);
    cloned->setFillColor(m_fillColor);
    cloned->move(m_position - cloned->center());
    cloned->setScaleFactor(m_scaleFactor);
    cloned->rotate(m_rotation);
    return cloned;
}
