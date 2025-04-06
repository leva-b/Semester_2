#include "shap.h"
#include "canvas.h"
double Shap::pi = 3.1415926535897932;

Shap::Shap(const QPoint& center): position(center){}

void Shap::changeCM(){
    QStringList labels = {"Центр масс X:", "Центр масс Y:"};
    QList<double> initialValues = {(double)position.x(), (double)position.y()};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        QPoint point(newValues[0], newValues[1]);
        QPoint different = point - position;
        this->move(different);
        position = point;
    }
}

void Shap::changeScale(){
    QStringList labels = {"Масштаб:"};
    QList<double> initialValues = {scaleFactor};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();

        if(newValues[0] > 10)newValues[0] = 10;
        else if(newValues[0] < 0.1)newValues[0] = 0.1;
        scaleFactor = newValues[0];
    }
}

void Shap::changeRotation(){
    QStringList labels = {"Угол поворота:"};
    QList<double> initialValues = {-rotation};

    ParameterDialog dialog(labels, initialValues);
    if (dialog.exec() == QDialog::Accepted) {
        QList<double> newValues = dialog.values();
        if(newValues[0] > 360)newValues[0] = fmod(newValues[0], 360);
        else if(newValues[0] < 0.1)newValues[0] = fmod(newValues[0], -360);
        rotation = -newValues[0];
    }
}

QMenu* Shap::createContextMenu(QWidget *parent) {
    QMenu *menu = new QMenu(parent);
    menu->addAction("Масштаб", this, &Shap::changeScale);
    menu->addAction("Угол поворота", this, &Shap::changeRotation);
    menu->addAction("Центр масс", this, &Shap::changeCM);
    return menu;
}

void Shap::updatePositionFromText() {
    bool okX, okY;
    int x = CMx->text().toInt(&okX);
    int y = CMy->text().toInt(&okY);
    if (okX && okY) {
        position = QPoint(x, y);
        update();
    }
}

void Shap::showInformation(QPainter& painter, int height) {

    painter.setPen(Qt::red);
    QRect rect(20, height - 43, 100, 20);
    painter.drawText(rect, Qt::AlignLeft, QString("P: ") + QString::number(this->perimeter(), 'f', 2));

    QRect rect2(20, height - 22, 100, 20);
    painter.drawText(rect2, Qt::AlignLeft, QString("S: ") + QString::number(this->area(), 'f', 2));

    QRect rect3(142, height - 22, 100, 20);
    painter.drawText(rect3, Qt::AlignLeft, QString("Угол: ") + QString::number(-this->rotation, 'f', 2));

    QRect rect4(118, height - 43, 100, 20);
    painter.drawText(rect4, Qt::AlignLeft, QString("Маcштаб: ") + QString::number(this->scaleFactor, 'f', 2));


    QRect rect5(250, height - 22, 100, 20);
    painter.drawText(rect5, Qt::AlignLeft, QString("ЦМ X: ") + QString::number(this->position.x(), 'f', 2));

    QRect rect6(250, height - 43, 100, 20);
    painter.drawText(rect6, Qt::AlignLeft, QString("ЦМ Y: ") + QString::number(this->position.y(), 'f', 2));
    //qDebug() << "Rotate: " << this->rotation;
}

void Shap::changeSelection() {
    selectedShape = !selectedShape;
    if (selectedShape) {
        color = Qt::green;
    } else {
        color = originalColor;
    }
}

void Shap::drawShape(QPainter& painter){
    painter.save();
    painter.translate(position); // Перемещаем начало координат в центр фигуры
    painter.rotate(rotation);
    painter.translate(-position);
    this->draw(painter);
    painter.restore();
}
