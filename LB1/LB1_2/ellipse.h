#ifndef ELLIPSE_H
#define ELLIPSE_H
#include <shap.h>

class Ellipse: public Shap
{
    int R1, R2;

    bool contains(const QPoint &point) const override;
    void changeRadius();
    void change();
public:
    void draw(QPainter& painter) override;
    Ellipse(QPoint& start, QPoint& end, QColor& color);
    QMenu* createContextMenu(QWidget *parent) override;

protected:
    double area() const override;
    double perimeter() const override;
    void move(const QPoint& offset) override;
    void rotate(double angle) override;
    void scale(double factor, const QPoint& center) override;
};

#endif // ELLIPSE_H
