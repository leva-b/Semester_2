#include "flag.h"
#include <QPainter>
Flag::Flag() {

}
void Flag::paintEvent(QPaintEvent*){
    QPainter painter;
    painter.begin(this);
    painter.drawPolygon({QPointF(120, 20), QPointF(220, 130), QPointF(120, 200)});
    painter.end();
}
void Flag::raisingLoweringFlag(){

}
