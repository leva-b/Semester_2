#ifndef SHIP_H
#define SHIP_H
#include <QWidget>
#include <rectanglewidget.h>
class Ship: public RectangleWidget
{
    int perfectCur;
    void paintEvent(QPaintEvent *event) override;
    void updatePosition() override;
public:
    Ship(QWidget*event = nullptr);

};

#endif // SHIP_H
