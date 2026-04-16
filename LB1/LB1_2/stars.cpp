#include "stars.h"
#include <QPainter>
#include "polygon.h"
#include <QPoint>

Stars::Stars(const QPoint& startPosition, const QPoint& end, int rays, const QColor& color)
    :Polygon(calculateVertices(startPosition, end, rays), color), numRays(rays)
{
    m_position = startPosition;
}

QVector<QPoint> Stars::calculateVertices(const QPoint& start, const QPoint& end,int rays)
{
    innerR = std::hypot(end.x() - start.x(), end.y() - start.y());
    outerR = innerR*2;
    QVector<QPoint> vertices;
    double angleStep = 2 * M_PI / rays;
    double offset = -M_PI / 2;
    for (int i = 0; i < rays; ++i) {
        // Внешняя точка
        double outerAngle = i * angleStep + offset;
        int outerX = static_cast<int>(start.x() + outerR * cos(outerAngle));
        int outerY = static_cast<int>(start.y() + outerR * sin(outerAngle));
        vertices << QPoint(outerX, outerY);

        // Внутренняя точка
        double innerAngle = outerAngle + angleStep / 2;
        int innerX = static_cast<int>(start.x() + innerR * cos(innerAngle));
        int innerY = static_cast<int>(start.y() + innerR * sin(innerAngle));
        vertices << QPoint(innerX, innerY);
    }
    return vertices;
}

QVector<QPoint> Stars::calculateVertices(const QPoint& start)
{
    QVector<QPoint> vertices;
    double angleStep = 2 * M_PI / numRays;
    double offset = -M_PI / 2;
    vertices.clear();
    for (int i = 0; i < numRays; ++i) {
        // Внешняя точка
        double outerAngle = i * angleStep + offset;
        int outerX = start.x() + outerR*m_scaleFactor* cos(outerAngle);
        int outerY = start.y() + outerR*m_scaleFactor * sin(outerAngle);
        vertices << QPoint(outerX, outerY);

        // Внутренняя точка
        double innerAngle = outerAngle + angleStep / 2;
        int innerX = start.x() + innerR*m_scaleFactor * cos(innerAngle);
        int innerY = start.y() + innerR*m_scaleFactor * sin(innerAngle);
        vertices << QPoint(innerX, innerY);
    }
    return vertices;
}

void Stars::draw(QPainter& painter){
    QPolygon polygon;
    for(const QPoint& vertice: vertices){
        polygon << (vertice - m_position)*m_scaleFactor + m_position;
    }
    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(m_fillColor);
    painter.drawPolygon(polygon);
}

double Stars::area()const{
    double result = 0;
    for(int i = 0; i < vertices.size(); i += 2){
        result += TriangleArea(vertices[i-1<0?vertices.size() - 1:i-1], vertices[i], vertices[i+1]);
        result += TriangleArea(vertices[i-1<0?vertices.size() - 1:i-1], m_position, vertices[i+1]);
    }
    return result*m_scaleFactor;
}

void Stars::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(m_scaleFactor * factor > 10)m_scaleFactor = 10;
    else if(m_scaleFactor * factor < 0.1)m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
    // vertices = calculateVertices(center);
}

QMenu* Stars::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    //menu->addAction("Внутренний радиус", this, &Stars::changeIR);
    //menu->addAction("Внешний радиус", this, &Stars::changeOR);
    return menu;
}

void Stars::changeIR(){
    QStringList labels = {"innerR:"};
    QList<double> initialValues = {(double)innerR};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*innerR)) innerR *= 10;
        else if(abs(newValues[0]) < abs(0.1*innerR)) innerR *= 0.1;
        else innerR = newValues[0];
        outerR = innerR * 2;
        vertices = calculateVertices(m_position);
    }
}

void Stars::changeOR(){
    QStringList labels = {"outerR:"};
    QList<double> initialValues = {(double)outerR};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*outerR)) outerR *= 10;
        else if(abs(newValues[0]) < abs(0.1*outerR)) outerR *= 0.1;
        else outerR = newValues[0];
        innerR = outerR/ 2;
        vertices = calculateVertices(m_position);
    }
}

