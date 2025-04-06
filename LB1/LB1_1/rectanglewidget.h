#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

#include <QWidget>
#include <QRect>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>
#include <QPainter>

class RectangleWidget : public QWidget {
    Q_OBJECT

public:
    explicit RectangleWidget(QWidget *parent = nullptr);

protected:
    QRect rectangle;
    void paintEvent(QPaintEvent *event) override;
    int dx;
    int dy;

protected slots:
    virtual void updatePosition();

private slots:
    void moveUp();
    void moveDown();
    void stop();

private:  
    QTimer *timer;
    static const int SPEED = 2;
};

#endif // RECTANGLEWIDGET_H
