#include "ellipse.h"
#include <QPainter>
#include <QPainterPath>

Ellipse::Ellipse(QPoint& start, QPoint& end, QColor& color){
    startPoint = start;
    endPoint = end;
    position = startPoint;
    R1 = startPoint.x() - endPoint.x();
    R2 = startPoint.y() - endPoint.y();
    this->color = color;
    this->originalColor = color;
}

double Ellipse::area() const {
    return pi*R1*R2*scaleFactor*scaleFactor;
}

double Ellipse::perimeter() const {
    int R1 = this->R1*scaleFactor;
    int R2 = this->R2*scaleFactor;
    return pi*(3*(std::abs(R1) + std::abs(R2)) - sqrt((3*std::abs(R1) + std::abs(R2))*(3*std::abs(R2) + std::abs(R1))));
}

void Ellipse::draw(QPainter& painter){
    painter.setPen(QPen(color,3));
    painter.drawEllipse(position.x() - R1*scaleFactor, position.y() - R2*scaleFactor, 2*R1*scaleFactor, 2*R2*scaleFactor);
    qDebug() << R1*scaleFactor;
    if (selectedShape && abs(R1*scaleFactor) > 30 && abs(R2*scaleFactor) > 30) {
        painter.setPen(QPen(Qt::blue, 2));
        QPoint endR1(position.x() + R1 * scaleFactor, position.y());
        painter.drawLine(position, endR1);
        painter.setPen(QPen(Qt::green,3));
        painter.drawText((position + endR1) / 2, "R1");

        painter.setPen(QPen(Qt::blue, 2));
        QPoint endR2(position.x(), position.y() + R2 * scaleFactor);
        painter.drawLine(position, endR2);

        painter.setPen(QPen(Qt::green,3));
        painter.drawText((position + endR2) / 2, "R2");
    }
}

void Ellipse::move(const QPoint& offset) {
    startPoint = startPoint + offset;
    position = position + offset;
}

void Ellipse::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;
}

bool Ellipse::contains(const QPoint &point) const {
    QPainterPath path;
    path.addEllipse(-R1, -R2, R1 * 2, R2 * 2);

    QTransform transform;
    transform.translate(position.x(), position.y());
    transform.scale(scaleFactor, scaleFactor);
    transform.rotate(rotation);

    // Применяем трансформацию к пути
    QPainterPath transformedPath = transform.map(path);

    return transformedPath.contains(point);
}

void Ellipse::rotate(double angle){
    rotation += angle;
}

QMenu* Ellipse::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    menu->addAction("Изменение R1, R2", this, &Ellipse::change);
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
