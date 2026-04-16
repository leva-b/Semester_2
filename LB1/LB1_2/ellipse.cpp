#include "ellipse.h"
#include <QPainter>
#include <QPainterPath>

Ellipse::Ellipse(const QPoint& start, const QPoint& end, const QColor& color){
    startPoint = start;
    endPoint = end;
    m_position = startPoint;
    R1 = startPoint.x() - endPoint.x();
    R2 = startPoint.y() - endPoint.y();
    m_lineColor = color;
    this->originalColor = color;
}

double Ellipse::area() const {
    return pi*R1*R2*m_scaleFactor*m_scaleFactor;
}

double Ellipse::perimeter() const {
    int R1 = this->R1*m_scaleFactor;
    int R2 = this->R2*m_scaleFactor;
    return pi*(3*(std::abs(R1) + std::abs(R2)) - sqrt((3*std::abs(R1) + std::abs(R2))*(3*std::abs(R2) + std::abs(R1))));
}

void Ellipse::draw(QPainter& painter){
    painter.setPen(QPen(m_lineColor, m_lineWidth));
    painter.setBrush(m_fillColor);
    painter.drawEllipse(m_position.x() - R1*m_scaleFactor, m_position.y() - R2*m_scaleFactor, 2*R1*m_scaleFactor, 2*R2*m_scaleFactor);
    //qDebug() << R1*m_scaleFactor;
    if (selectedShape && abs(R1*m_scaleFactor) > 30 && abs(R2*m_scaleFactor) > 30) {
        painter.setPen(QPen(Qt::blue, 2));
        QPoint endR1(m_position.x() + R1 * m_scaleFactor, m_position.y());
        painter.drawLine(m_position, endR1);
        painter.setPen(QPen(Qt::green,3));
        painter.drawText((m_position + endR1) / 2, "R1");

        painter.setPen(QPen(Qt::blue, 2));
        QPoint endR2(m_position.x(), m_position.y() + R2 * m_scaleFactor);
        painter.drawLine(m_position, endR2);

        painter.setPen(QPen(Qt::green,3));
        painter.drawText((m_position + endR2) / 2, "R2");
    }
}

void Ellipse::move(const QPoint& offset) {
    startPoint = startPoint + offset;
    m_position = m_position + offset;
}

void Ellipse::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(m_scaleFactor * factor > 10)m_scaleFactor = 10;
    else if(m_scaleFactor * factor < 0.1)m_scaleFactor = 0.1;
    else m_scaleFactor *= factor;
}

bool Ellipse::contains(const QPoint &point) const {
    QPainterPath path;
    path.addEllipse(-R1, -R2, R1 * 2, R2 * 2);

    QTransform transform;
    transform.translate(m_position.x(), m_position.y());
    transform.scale(m_scaleFactor, m_scaleFactor);
    transform.rotate(m_rotation);

    // Применяем трансформацию к пути
    QPainterPath transformedPath = transform.map(path);

    return transformedPath.contains(point);
}

void Ellipse::rotate(double angle){
    m_rotation += angle;
}

QMenu* Ellipse::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    //menu->addAction("Изменение R1, R2", this, &Ellipse::change);
    return menu;
}

void Ellipse::change(){
    QStringList labels = {"R1:", "R2:"};
    QList<double> initialValues = {(double)R1, (double)R2};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*R1)) R1 *= 10;
        else if(abs(newValues[0]) < abs(0.1*R1)) R1 *= 0.1;
        else R1 = newValues[0];

        if(abs(newValues[1]) > abs(10*R2)) R2 *= 10;
        else if(abs(newValues[1]) < abs(0.1*R2)) R2 *= 0.1;
        else R2 = newValues[1];
    }
}

QRect Ellipse::boundingRect() const {
    double a = std::abs(R1) * m_scaleFactor;
    double b = std::abs(R2) * m_scaleFactor;
    double angleRad = m_rotation * M_PI / 180.0;
    double cosTheta = std::cos(angleRad);
    double sinTheta = std::sin(angleRad);

    double dx = std::sqrt(a*a*cosTheta*cosTheta + b*b*sinTheta*sinTheta);
    double dy = std::sqrt(a*a*sinTheta*sinTheta + b*b*cosTheta*cosTheta);

    int left = static_cast<int>(m_position.x() - dx);
    int top = static_cast<int>(m_position.y() - dy);
    int width = static_cast<int>(2*dx);
    int height = static_cast<int>(2*dy);
    return QRect(left, top, width, height);
}
