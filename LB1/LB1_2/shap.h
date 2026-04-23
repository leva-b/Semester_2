#ifndef SHAP_H
#define SHAP_H
#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QLineEdit>
#include <QDebug>
#include <QMenu>
#include <parameterdialog.h>

class Shap
{
    QLineEdit* CMx = nullptr;
    QLineEdit* CMy = nullptr;
    void changeScale();

    void changeRotation();
    void changeCM();
protected:
    bool selectedShape = false;
    Shap(const QPoint& center);
    Shap() = default;
    static double pi;

    QColor m_lineColor;
    QColor m_fillColor;
    int m_lineWidth = 2;          // default
    bool m_selected = false;
    QPoint m_position;
    double m_rotation = 0.0;
    double m_scaleFactor = 1.0;

    QPoint m_startPoint;  // used during creation
    QPoint m_endPoint;

    QColor originalColor;

    QPoint startPoint;
    QPoint endPoint;

    virtual double area() const = 0;
    virtual double perimeter() const = 0;


public:
    virtual QString typeName() const = 0;
    Shap(const Shap&) = default;
    Shap& operator=(const Shap&) = default;
    explicit Shap(const QColor &color){
        m_lineColor = color;
    }
    virtual ~Shap() = default;

    virtual QRect boundingRect() const = 0;
    virtual Shap* clone() const = 0;
    virtual QMenu* createContextMenu(QWidget *parent);
    virtual bool onContextMenuAction(const QString& action);
    virtual void rotate(double angle) = 0;
    virtual void scale(double factor, const QPoint& center) = 0;

    int lineWidth() const { return m_lineWidth; }
    void setLineWidth(int width) { m_lineWidth = width; }
    QColor lineColor() const { return m_lineColor; }
    void setLineColor(const QColor &color) { m_lineColor = color; }
    QColor fillColor() const { return m_fillColor; }
    void setFillColor(const QColor &color) { m_fillColor = color; }

    bool isSelected() const { return m_selected; }
    void setSelected(bool sel) { m_selected = sel; }
    double scaleFactor() const { return m_scaleFactor; }
    double rotation() const { return m_rotation; }


    void updatePositionFromText();

    void setParameter(double param) {m_rotation = param;}
    void setScaleFactor(double scaleFactor){m_scaleFactor = scaleFactor;}
    void updatePosition(const QPoint& newPosition) {m_position = newPosition;}

    void showInformation(QPainter& painter, int height);
    //virtual void showMoreInformation() override = 0;

    virtual void move(const QPoint& offset) = 0;
    void changeSelection();

    QColor getColor() const{
        return m_lineColor;
    }
    void setColor(const QColor& color){
        this->m_lineColor = color;
    }
    void setOriginalColor(const QColor &originalColor) {
        this->originalColor = originalColor;
    }

    QColor getOriginalColor() const {
        return originalColor;
    }

    virtual QPoint center() const{
        return m_position;
    }
    virtual bool contains(const QPoint &point) const = 0;

    virtual void draw(QPainter& painter) = 0;
    void drawShape(QPainter& painter);

};



#endif // SHAP_H
