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
    QString typeName() const override { return "Ellipse"; }
    void draw(QPainter& painter) override;
    Ellipse(const QPoint& start, const QPoint& end, const QColor& color);
    QMenu* createContextMenu(QWidget *parent) override;
    QRect boundingRect() const override;
    Shap* clone() const override { return new Ellipse(*this); }
    Ellipse(const Ellipse&) = default;

protected:
    double area() const override;
    double perimeter() const override;
    void move(const QPoint& offset) override;
    void rotate(double angle) override;
    void scale(double factor, const QPoint& center) override;
};

#endif // ELLIPSE_H
