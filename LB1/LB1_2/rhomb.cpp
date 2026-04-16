#include "rhomb.h"
#include <QPainter>
Rhomb::Rhomb(const QPoint& start, const QPoint& end, const QColor& color):
    Polygon(calculateVertices(start, end), color){
    m_position = QPoint(start.x(), end.y());
}

void Rhomb::draw(QPainter& painter)
{
    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(m_fillColor);

    QPolygon polygon;
    for(int i = 0; i < 4; i++)
        polygon << (vertices[i]- m_position)*m_scaleFactor + m_position;
    painter.drawPolygon(polygon);
}


double Rhomb::area() const{
    double result = std::hypot(vertices[0].x() - vertices[2].x(), vertices[0].y() - vertices[2].y())
                   *std::hypot(vertices[1].x() - vertices[3].x(), vertices[1].y() - vertices[3].y())/2;
    return result*m_scaleFactor*m_scaleFactor;
}

QVector<QPoint> Rhomb::calculateVertices(const QPoint& start, const QPoint& end){
    return QVector<QPoint>{start, end, QPoint(start.x(), end.y() - start.y() + end.y()), QPoint(2*start.x() - end.x(), end.y())};
}

QVector<QPoint> Rhomb::calculateVertices(int d1, int d2){
    QVector<QPoint> vertices(4);
    vertices[0] = QPoint(m_position.x(), m_position.y() - d1 / 2);
    vertices[1] = QPoint(m_position.x() + d2 / 2, m_position.y());
    vertices[2] = QPoint(m_position.x(), m_position.y() + d1 / 2);
    vertices[3] = QPoint(m_position.x() - d2 / 2, m_position.y());

    return vertices;
}

void Rhomb::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(m_scaleFactor * factor > 10)m_scaleFactor = 10;
    else if(m_scaleFactor * factor < 0.1)m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
}

QMenu* Rhomb::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    //menu->addAction("Изменение d1, d2", this, &Rhomb::change);
    return menu;
}

void Rhomb::change(){
    QStringList labels = {"d1:", "d2:"};
    double d1 = (vertices[2].y() - vertices[0].y())*m_scaleFactor;
    double d2 = (vertices[1].x() - vertices[3].x())*m_scaleFactor;
    QList<double> initialValues = {(double)d1, (double)d2};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*d1*m_scaleFactor)) d1 *= 10;
        else if(abs(newValues[0]) < abs(0.1*d1*m_scaleFactor)) d1 *= 0.1;
        else d1 = newValues[0];

        if(abs(newValues[1]) > abs(10*d2*m_scaleFactor)) d2 *= 10;
        else if(abs(newValues[1]) < abs(0.1*d1*m_scaleFactor)) d2 *= 0.1;
        else d2 = newValues[1];
        vertices = calculateVertices(d1*m_scaleFactor,d2*m_scaleFactor);
    }
}

