#ifndef SQUARE_H
#define SQUARE_H
#include <QColor>
#include <QPoint>
#include "rhomb.h"
class Square: public Rhomb
{
    //void calculateVertices(const QPoint& start) override;
    void change();
    int sideLength;
    void calculateVertices();
public:
    QMenu* createContextMenu(QWidget *parent)override;
    Square(const QPoint& start, const QPoint& end, const QColor& color);
    void calculateVertices(const QPoint& start, const QPoint& end);
    Square(const Square&) = default;
};

#endif // SQUARE_H
