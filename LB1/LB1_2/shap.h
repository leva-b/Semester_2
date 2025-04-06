#ifndef SHAP_H
#define SHAP_H
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QDebug>
#include <QMenu>
#include <parameterdialog.h>

class Shap: public QWidget
{
    Q_OBJECT


    QLineEdit* CMx = nullptr;
    QLineEdit* CMy = nullptr;
    void changeScale();

    void changeRotation();
    void changeCM();
protected:
    bool selectedShape = false;
    Shap(const QPoint& center);
    Shap() = default;
    Shap(const QColor& color) {
        this->color = color;
    }

    static double pi;

    QColor color;
    QColor originalColor;
    QPoint position;
    double rotation = 0;
    double scaleFactor = 1;

    QPoint startPoint;
    QPoint endPoint;

    virtual double area() const = 0;
    virtual double perimeter() const = 0;


public:
    virtual QMenu* createContextMenu(QWidget *parent);

    virtual void rotate(double angle) = 0;
    virtual void scale(double factor, const QPoint& center) = 0;
    void updatePositionFromText();

    void setParameter(double param) {rotation = param;}

    void updatePosition(const QPoint& newPosition) {position = newPosition;     update();}

    void showInformation(QPainter& painter, int height);
    //virtual void showMoreInformation() override = 0;

    virtual void move(const QPoint& offset) = 0;
    void changeSelection();

    QColor getColor() const{
        return color;
    }
    void setColor(const QColor& color){
        this->color = color;
    }
    void setOriginalColor(const QColor &originalColor) {
        this->originalColor = originalColor;
    }

    QColor getOriginalColor() const {
        return originalColor;
    }

    virtual QPoint center() const{
        return position;
    }
    virtual bool contains(const QPoint &point) const = 0;

    virtual void draw(QPainter& painter) = 0;
    void drawShape(QPainter& painter);

};



#endif // SHAP_H
