#include "circle.h"
#include <QPainter>
#include <QPainterPath>

Circle::Circle(const QPoint& start, const QPoint& end, const QColor& color) {
    radius = std::hypot(start.x() - end.x(), start.y() - end.y());
    m_lineColor= color;
    this->originalColor = color;
    startPoint = start;
    endPoint = end;
    m_position = startPoint;
}

double Circle::area() const{
    return radius*m_scaleFactor*m_scaleFactor*radius*pi;
}

double Circle::perimeter() const{
    return 2*pi*radius*m_scaleFactor;
}

void Circle::draw(QPainter& painter){
    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(m_fillColor);
    painter.drawEllipse(m_position.x() - radius*m_scaleFactor, m_position.y() - radius*m_scaleFactor, radius*2*m_scaleFactor, radius*2*m_scaleFactor);
}

void Circle::move(const QPoint& offset) {
    //startPoint = startPoint + offset;
    m_position = m_position + offset;
}
void Circle::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(m_scaleFactor * factor > 10)m_scaleFactor = 10;
    else if(m_scaleFactor * factor < 0.1)m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
}

bool Circle::contains(const QPoint &point) const {
    qDebug() << "point:" << point << "position:" << m_position;
    QPainterPath path;
    path.addEllipse(m_position, radius*m_scaleFactor, radius*m_scaleFactor);

    return path.contains(point);
}


QMenu* Circle::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    //menu->addAction("Изменение R", this, &Circle::change);
    return menu;
}

void Circle::change(){
    QStringList labels = {"R:"};
    QList<double> initialValues = {(double)radius};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*radius)) radius *= 10;
        else if(abs(newValues[0]) < abs(0.1*radius)) radius *= 0.1;
        else radius = newValues[0];

    }
}

QRect Circle::boundingRect() const {
    int r = radius * m_scaleFactor;
    QRect rect(-r, -r, 2*r, 2*r);
    QTransform transform;
    transform.translate(m_position.x(), m_position.y());
    transform.rotate(m_rotation * 180 / M_PI);
    return transform.mapRect(rect);
}
