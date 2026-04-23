#ifndef CANVAS_H
#define CANVAS_H
#include <QWidget>
#include <QComboBox>
#include <QGestureEvent>
#include <QPushButton>
#include <QSpinBox>
#include "layermanager.h"
#include <QUndoStack>
#include "polyline.h"
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
    Star8,
    Polyline
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

    QUndoStack* undoStack() const { return m_undoStack; }
    const QList<Shap*>& selectedShapes() const { return m_selectedShapes; }
    LayerManager* layerManager() const { return m_layerManager; }
private:
    QUndoStack* m_undoStack;
    ToolCategory m_currentCategory = ToolCategory::Selection;
    ShapeType m_currentShapeType = ShapeType::Rectangle;

    void finishPolyline();
    void mouseDoubleClickEvent(QMouseEvent *event)override;

    void clearSelection();
    void addToSelection(Shap *shape, bool clearOthers = false);
    void toggleSelection(Shap *shape);
    void moveSelectedBy(const QPoint &delta);
    void rotateSelected(double angleDelta);
    void scaleSelected(double factor);
    void updateCurrentShapePreview();

    int findLayerIndex(Shap* shape) const;
    void handleSelectionPress(QMouseEvent *event);
    void handleTransformationPress(QMouseEvent *event);
    void startDrawing(const QPoint &pos);
    void finishDrawing();

    ShapeType shapeTypeFromString(const QString &type) const;
    Shap* createShape(ShapeType type, const QPoint &start, const QPoint &end);

    QPoint m_lastCursorPos;
    bool m_drawing = false;
    QPoint m_lastMousePos;
    QPoint m_startPoint, m_endPoint;
    QPoint m_dragStart;
    QPoint m_moveDelta;
    QPoint m_lastDragPos;
    bool m_rubberBandActive = false;
    QRect m_selectionRubberBandRect;

    bool m_transforming = false;
    QList<double> m_initialScales;
    QList<double> m_initialAngles;
    QPoint m_transformStartPoint;
    QList<double> m_selectedShapesStartScale;
    QList<double> m_selectedShapesStartRot;
    bool m_moving = false;

    QList<QPoint> m_selectedShapesStartPos;
    QComboBox *m_shapeCombo = nullptr;

    LayerManager *m_layerManager = nullptr;
    QList<Shap*> m_selectedShapes;
    QList<Shap*> m_clipboard;

    QSpinBox *m_lineWidthSpin = nullptr;
    QPushButton *m_lineColorBtn = nullptr;
    QPushButton *m_fillColorBtn = nullptr;
    QPushButton *m_deleteBtn = nullptr;
    bool m_buildingPolyline = false;
    Polyline* m_tempPolyline = nullptr;

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
