#include "triangle.h"
#include <QPainter>
#include <QPolygon>
#include <parameterdialog.h>

Triangle::Triangle(const QPoint& point1, const QPoint& point2, const QPoint& point3, const QColor& color):
    Polygon({point1, point2, point3}, color){
    if(point1.y() < point2.y())
        position = QPoint(point1.x(), (point2.y()-point1.y())*2/3+point1.y());
    else
        position = QPoint(point1.x(), (point1.y()-point2.y())/3 + point2.y());
}


double Triangle::area() const {
    double p = this->perimeter()/2/scaleFactor;
    double result = scaleFactor*scaleFactor*sqrt(p*(p - std::hypot(vertices[0].x() - vertices[1].x(),vertices[0].y() - vertices[1].y()))*
                                                     (p - std::hypot(vertices[1].x() - vertices[2].x(),vertices[1].y() - vertices[2].y()))*
                                                     (p - std::hypot(vertices[2].x() - vertices[0].x(),vertices[2].y() - vertices[0].y())));
    return result;
}

void Triangle::draw(QPainter& painter) {
    painter.setPen(QPen(color, 3));

    QPolygon polygon;
    for(const QPoint& vertice: vertices){
        polygon << (vertice - position) * scaleFactor + position;
    }
    painter.drawPolygon(polygon);

    if(this->selectedShape){
        painter.setPen(Qt::red);
        for (int i = 0; i < vertices.size(); ++i) {
            QPoint labeledPoint = (vertices[i] - position) * scaleFactor + position;
            if (i == 2) {
                painter.drawText(labeledPoint + QPoint(-15, -5), QString("V%1").arg(i + 1));
            } else {
                painter.drawText(labeledPoint + QPoint(5, -5), QString("V%1").arg(i + 1));
            }
        }
    }
}

void Triangle::scale(double factor, const QPoint& center){
    Q_UNUSED(center);
    if(scaleFactor * factor > 10)scaleFactor = 10;
    else if(scaleFactor * factor < 0.1)scaleFactor = 0.1;
    else scaleFactor *= factor;
}


QMenu* Triangle::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    menu->addAction("Изменение координат точки", this, &Triangle::change);
    return menu;
}

void Triangle::change() {
    QStringList labels = {"x:", "y:"};
    QList<QPoint> initialVertices = {vertices[0], vertices[1], vertices[2]}; // Получаем координаты вершин

    ParameterDialog dialog(labels, initialVertices);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        // Обновляем координаты для выбранной вершины
        int selectedIndex = dialog.pointSelector->currentIndex(); // Получаем индекс выбранной точки
        vertices[selectedIndex] = QPoint(newValues[0], newValues[1]);
        position = (vertices[0] + vertices[1] + vertices[2])/3;
    }
}
