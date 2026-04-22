#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QComboBox>
#include <QGestureEvent>
#include <QPushButton>
#include <QSpinBox>
#include "layermanager.h"

class Shap;

enum class ToolCategory {
    Selection,
    Transformation,
    Drawing
};

enum class ShapeType {
    Rectangle,
    Triangle,
    Circle,
    Ellipse,
    Hexagon,
    Rhomb,
    Square,
    Star5,
    Star6,
    Star8
};

class Canvas: public QWidget
{
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
public:

    Canvas(QWidget *parent = nullptr);
    static bool isPointInWindow(const QWidget *widget, const QPoint &point) {
        QRect windowRect = widget->rect();
        return !windowRect.contains(point);
    }

    void setActiveLayer(int index);
    void setLineWidth(int width);
    void setLineColor(const QColor &color);
    void setFillColor(const QColor &color);

    void deleteSelectedShapes();
    void copySelected();
    void pasteShapes();

    const QList<Shap*>& selectedShapes() const { return m_selectedShapes; }
    LayerManager* layerManager() const { return m_layerManager; }
private:

    ToolCategory m_currentCategory = ToolCategory::Selection;
    ShapeType m_currentShapeType = ShapeType::Rectangle;

    void clearSelection();
    void addToSelection(Shap *shape, bool clearOthers = false);
    void toggleSelection(Shap *shape);
    void moveSelectedBy(const QPoint &delta);
    void rotateSelected(double angleDelta);
    void scaleSelected(double factor);
    void updateCurrentShapePreview();

    void handleSelectionPress(QMouseEvent *event);
    void handleTransformationPress(QMouseEvent *event);
    void startDrawing(const QPoint &pos);
    void finishDrawing();

    ShapeType shapeTypeFromString(const QString &type) const;
    Shap* createShape(ShapeType type, const QPoint &start, const QPoint &end);

    bool m_drawing = false;
    QPoint m_startPoint, m_endPoint;
    QPoint m_dragStart;

    bool m_rubberBandActive = false;
    QRect m_selectionRubberBandRect;

    bool m_transforming = false;
    QPoint m_transformStartPoint;
    QList<double> m_selectedShapesStartScale;
    QList<double> m_selectedShapesStartRot;

    QList<QPoint> m_selectedShapesStartPos;
    QComboBox *m_shapeCombo = nullptr;

    LayerManager *m_layerManager = nullptr;
    QList<Shap*> m_selectedShapes;
    QList<Shap*> m_clipboard;

    QSpinBox *m_lineWidthSpin = nullptr;
    QPushButton *m_lineColorBtn = nullptr;
    QPushButton *m_fillColorBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;

    QColor m_currentLineColor;
    QColor m_currentFillColor;
    int m_currentLineWidth;
    int m_activeLayerIndex = 0;
private slots:
    void chooseLineColor();
    void chooseFillColor();
    void onLayersChanged();

};

#endif // CANVAS_H
