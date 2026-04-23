#ifndef CIRCLE_H
#define CIRCLE_H
#include <shap.h>
class Circle: public Shap
{
    double radius;
    bool contains(const QPoint &point) const override;
    void change();
public:
    QString typeName() const override { return "Circle"; }
    QMenu* createContextMenu(QWidget *parent) override;
    void draw(QPainter& painter) override;
    Circle(const QPoint& start, const QPoint& end, const QColor& color);
    double area() const override;
    double perimeter() const override;
    void move(const QPoint& offset) override;
    void rotate(double angle) override { m_rotation += angle;}
    void scale(double factor, const QPoint& center) override;
    QRect boundingRect() const override;
    Shap* clone() const override { return new Circle(*this); }
    Circle(const Circle&) = default;
};

#endif // CIRCLE_H
