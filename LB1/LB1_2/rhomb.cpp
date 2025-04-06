#include "rhomb.h"
#include <QPainter>
Rhomb::Rhomb(const QPoint& start, const QPoint& end, const QColor& color):
    Polygon(calculateVertices(start, end), color){
    position = QPoint(start.x(), end.y());
}

void Rhomb::draw(QPainter& painter)
{

    painter.setPen(QPen(color, 3));
    painter.setBrush(Qt::NoBrush);
    QPolygon polygon;
    for(int i = 0; i < 4; i++)
        polygon << (vertices[i]- position)*scaleFactor + position;
    painter.drawPolygon(polygon);
}


double Rhomb::area() const{
    double result = std::hypot(vertices[0].x() - vertices[2].x(), vertices[0].y() - vertices[2].y())
                   *std::hypot(vertices[1].x() - vertices[3].x(), vertices[1].y() - vertices[3].y())/2;
    return result*scaleFactor*scaleFactor;
}

QVector<QPoint> Rhomb::calculateVertices(const QPoint& start, const QPoint& end){
    return QVector<QPoint>{start, end, QPoint(start.x(), end.y() - start.y() + end.y()), QPoint(2*start.x() - end.x(), end.y())};
}

QVector<QPoint> Rhomb::calculateVertices(int d1, int d2){
    QVector<QPoint> vertices(4);
    vertices[0] = QPoint(position.x(), position.y() - d1 / 2);
    vertices[1] = QPoint(position.x() + d2 / 2, position.y());
    vertices[2] = QPoint(position.x(), position.y() + d1 / 2);
    vertices[3] = QPoint(position.x() - d2 / 2, position.y());

    return vertices;
}

void Rhomb::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;
}

QMenu* Rhomb::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    menu->addAction("Изменение d1, d2", this, &Rhomb::change);
    return menu;
}

void Rhomb::change(){
    QStringList labels = {"d1:", "d2:"};
    double d1 = (vertices[2].y() - vertices[0].y())*scaleFactor;
    double d2 = (vertices[1].x() - vertices[3].x())*scaleFactor;
    QList<double> initialValues = {(double)d1, (double)d2};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]) > abs(10*d1*scaleFactor)) d1 *= 10;
        else if(abs(newValues[0]) < abs(0.1*d1*scaleFactor)) d1 *= 0.1;
        else d1 = newValues[0];

        if(abs(newValues[1]) > abs(10*d2*scaleFactor)) d2 *= 10;
        else if(abs(newValues[1]) < abs(0.1*d1*scaleFactor)) d2 *= 0.1;
        else d2 = newValues[1];
        vertices = calculateVertices(d1*scaleFactor,d2*scaleFactor);
    }
}
