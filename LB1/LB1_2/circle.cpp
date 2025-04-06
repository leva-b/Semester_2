#include "circle.h"
#include <QPainter>
#include <QPainterPath>

Circle::Circle(QPoint& start, QPoint& end, QColor& color) {
    radius = std::hypot(start.x() - end.x(), start.y() - end.y());
    this->color = color;
    this->originalColor = color;
    startPoint = start;
    endPoint = end;
    position = startPoint;
}

double Circle::area() const{
    return radius*scaleFactor*scaleFactor*radius*pi;
}

double Circle::perimeter() const{
    return 2*pi*radius*scaleFactor;
}

void Circle::draw(QPainter& painter){
    painter.setPen(QPen(color,3));
    painter.drawEllipse(position.x() - radius*scaleFactor, position.y() - radius*scaleFactor, radius*2*scaleFactor, radius*2*scaleFactor);
}

void Circle::move(const QPoint& offset) {
    //startPoint = startPoint + offset;
    position = position + offset;
}
void Circle::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;
}

bool Circle::contains(const QPoint &point) const {
    qDebug() << "point:" << point << "position:" << position;
    QPainterPath path;
    path.addEllipse(position, radius*scaleFactor, radius*scaleFactor);

    return path.contains(point);
}


QMenu* Circle::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    menu->addAction("Изменение R", this, &Circle::change);
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
