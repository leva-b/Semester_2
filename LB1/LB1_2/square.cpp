#include "square.h"
#include <QPoint>
#include <QColor>

Square::Square(QPoint& start, QPoint& end, QColor& color):
    Rhomb(start, end, color){
    calculateVertices(start, end);
    position = start;
}

void Square::calculateVertices(const QPoint& start, const QPoint& end) {
    vertices.clear();
    //this->vertices.push_back(start);
    sideLength = std::max(abs(start.x() - end.x()), abs(start.y() - end.y()));

    int centerX = start.x();
    int centerY = start.y();

    vertices.push_back(QPoint(centerX - sideLength, centerY - sideLength));
    vertices.push_back(QPoint(centerX + sideLength, centerY - sideLength));
    vertices.push_back(QPoint(centerX + sideLength, centerY + sideLength));
    vertices.push_back(QPoint(centerX - sideLength, centerY + sideLength));
}

void Square::calculateVertices() {
    vertices.clear();

    int centerX = position.x();
    int centerY = position.y();

    vertices.push_back(QPoint(centerX - sideLength, centerY - sideLength));
    vertices.push_back(QPoint(centerX + sideLength, centerY - sideLength));
    vertices.push_back(QPoint(centerX + sideLength, centerY + sideLength));
    vertices.push_back(QPoint(centerX - sideLength, centerY + sideLength));
}

QMenu* Square::createContextMenu(QWidget *parent){
    QMenu* menu = Shap::createContextMenu(parent);
    menu->addAction("Длина стороны", this, &Square::change);
    return menu;
}

void Square::change(){
    QStringList labels = {"Длина стороны:"};
    QList<double> initialValues = {(double)2*sideLength};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(abs(newValues[0]/2) > abs(10*sideLength)) sideLength *= 10;
        else if(abs(newValues[0]/2) < abs(0.1*sideLength)) sideLength *= 0.1;
        else sideLength = newValues[0]/2;
        calculateVertices();
    }
}

