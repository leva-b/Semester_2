#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QComboBox>
#include <QGestureEvent>
class Shap;

class Canvas: public QWidget
{
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
public:
    Canvas(QWidget *parent = nullptr);
    static bool isPointInWindow(const QWidget *widget, const QPoint &point) {
        QRect windowRect = widget->rect();
        return !windowRect.contains(point);
    }
    void setColor(const QColor &newColor);
    void clear();
private:
    //void openParameterDialog(Shap* shape);
    bool isRotating = false;
    bool drawing;
    QPoint startPoint;
    QPoint endPoint;
    QColor color;
    Shap* selectedShape;
    QComboBox *shapeComboBox;
    QList<Shap*> shapes;

    QLineEdit* CMx;
    QLineEdit* CMy;

    //void updateShapePositionFromText();
private slots:
    void chooseColor();
};

#endif // CANVAS_H
