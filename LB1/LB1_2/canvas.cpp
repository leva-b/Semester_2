#include "canvas.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QGestureEvent>
#include "rectangle.h"
#include "triangle.h"
#include "hexagon.h"
#include "rhomb.h"
#include "square.h"
#include "circle.h"
#include "ellipse.h"
#include "stars.h"
#include <QMenu>
#include <QToolBar>
#include <QActionGroup>
#include "command/addshapecommand.h"
#include "command/moveshapecommand.h"
#include "command/removeshapecommand.h"
#include "command/scaleshapecommand.h"
#include "command/propertychangecmd.h"
#include "command/rotateshapecommand.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_undoStack(new QUndoStack(this))
    , m_drawing(false)
    , m_layerManager(new LayerManager(this))
    , m_currentLineColor(Qt::black)
    , m_currentFillColor(Qt::transparent)
    , m_currentLineWidth(2)
{
    connect(m_layerManager, &LayerManager::layersChanged, this, &Canvas::onLayersChanged);

    // Создание UI
    m_shapeCombo = new QComboBox(this);
    m_shapeCombo->addItems({"Rectangle", "Triangle", "Circle", "Ellipse", "Polyline",
                            "Hexagon", "Rhomb", "Square", "5-star", "6-star", "8-star"});
    m_shapeCombo->setEnabled(false);

    connect(m_shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        if(idx >= 0){
            m_currentShapeType = shapeTypeFromString(m_shapeCombo->currentText());
        }
    });

    m_lineWidthSpin = new QSpinBox(this);
    m_lineWidthSpin->setRange(1, 20);
    m_lineWidthSpin->setValue(m_currentLineWidth);
    connect(m_lineWidthSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &Canvas::setLineWidth);

    m_lineColorBtn = new QPushButton("Line Color", this);
    connect(m_lineColorBtn, &QPushButton::clicked, this, &Canvas::chooseLineColor);

    m_fillColorBtn = new QPushButton("Fill Color", this);
    connect(m_fillColorBtn, &QPushButton::clicked, this, &Canvas::chooseFillColor);

    m_deleteBtn = new QPushButton("Delete", this);
    connect(m_deleteBtn, &QPushButton::clicked, this, &Canvas::deleteSelectedShapes);

    QPushButton *copyBtn = new QPushButton("Copy", this);
    QPushButton *pasteBtn = new QPushButton("Paste", this);
    connect(copyBtn, &QPushButton::clicked, this, &Canvas::copySelected);
    connect(pasteBtn, &QPushButton::clicked, this, &Canvas::pasteShapes);

    // Панель инструментов
    QToolBar *toolBar = new QToolBar(this);
    QActionGroup *categoryGroup = new QActionGroup(this);
    QAction *selectAction = toolBar->addAction("Select");
    selectAction->setCheckable(true);
    selectAction->setChecked(true);
    QAction *transfAction = toolBar->addAction("Transform");
    transfAction->setCheckable(true);
    QAction *drawAction = toolBar->addAction("Draw");
    drawAction->setCheckable(true);
    categoryGroup->addAction(selectAction);
    categoryGroup->addAction(transfAction);
    categoryGroup->addAction(drawAction);

    connect(selectAction, &QAction::triggered, [this]() {
        m_currentCategory = ToolCategory::Selection;
        m_shapeCombo->setEnabled(false); // отключаем выбор фигур
    });

    connect(transfAction, &QAction::triggered, [this]() {
        m_currentCategory = ToolCategory::Transformation;
        m_shapeCombo->setEnabled(true);
    });

    connect(drawAction, &QAction::triggered, [this]() {
        m_currentCategory = ToolCategory::Drawing;
        m_shapeCombo->setEnabled(true);
    });


    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(toolBar);
    topLayout->addWidget(m_shapeCombo);
    topLayout->addWidget(new QLabel("Line width:"));
    topLayout->addWidget(m_lineWidthSpin);
    topLayout->addWidget(m_lineColorBtn);
    topLayout->addWidget(m_fillColorBtn);
    topLayout->addWidget(m_deleteBtn);
    topLayout->addWidget(copyBtn);
    topLayout->addWidget(pasteBtn);
    topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

void Canvas::setLineWidth(int width)
{
    if (m_selectedShapes.isEmpty()) {
        m_currentLineWidth = width;
        return;
    }

    QUndoCommand* macro = new QUndoCommand(tr("Change line width"));
    for (Shap* s : m_selectedShapes) {
        new PropertyChangeCommand<int>(
            this, s, width,
            [](Shap* sh, int w) { sh->setLineWidth(w); },
            [](const Shap* sh) { return sh->lineWidth(); },
            tr("Line Width"), macro
            );
    }
    m_undoStack->push(macro);
    m_currentLineWidth = width;
}

void Canvas::setLineColor(const QColor &color)
{
    if (m_selectedShapes.isEmpty()) {
        m_currentLineColor = color;
        return;
    }

    QUndoCommand* macro = new QUndoCommand(tr("Change line color"));
    for (Shap* s : m_selectedShapes) {
        new PropertyChangeCommand<QColor>(
            this, s, color,
            [](Shap* sh, const QColor& c) { sh->setLineColor(c); },
            [](const Shap* sh) { return sh->lineColor(); },
            tr("Line Color"), macro
            );
    }
    m_undoStack->push(macro);
    m_currentLineColor = color;
}

void Canvas::setFillColor(const QColor &color)
{
    if (m_selectedShapes.isEmpty()) {
        m_currentFillColor = color;
        return;
    }

    QUndoCommand* macro = new QUndoCommand(tr("Change fill color"));
    for (Shap* s : m_selectedShapes) {
        new PropertyChangeCommand<QColor>(
            this, s, color,
            [](Shap* sh, const QColor& c) { sh->setFillColor(c); },
            [](const Shap* sh) { return sh->fillColor(); },
            tr("Fill Color"), macro
            );
    }
    m_undoStack->push(macro);
    m_currentFillColor = color;
}

void Canvas::chooseLineColor()
{
    QColor col = QColorDialog::getColor(Qt::black, this, "Line Color");
    if (col.isValid()) setLineColor(col);
}

void Canvas::chooseFillColor()
{
    QColor col = QColorDialog::getColor(Qt::transparent, this, "Fill Color");
    if (col.isValid()) setFillColor(col);
}

void Canvas::deleteSelectedShapes()
{
    if (m_selectedShapes.isEmpty())
        return;

    QUndoCommand* macro = new QUndoCommand(tr("Delete %1 shapes").arg(m_selectedShapes.size()));
    for (Shap* s : m_selectedShapes) {
        int layerIdx = findLayerIndex(s);
        new RemoveShapeCommand(this, s, layerIdx, macro);
    }
    m_undoStack->push(macro);
    m_selectedShapes.clear();
    update();
}

int Canvas::findLayerIndex(Shap* shape) const
{
    for (int i = 0; i < m_layerManager->layerCount(); ++i) {
        Layer* layer = m_layerManager->layer(i);
        if (layer && layer->contains(shape))
            return i;
    }
    return -1;
}

void Canvas::copySelected()
{
    m_clipboard.clear();
    for (Shap *s : m_selectedShapes)
        m_clipboard.append(s->clone());
}

void Canvas::pasteShapes()
{
    if (m_clipboard.isEmpty()) return;
    Layer *targetLayer = m_layerManager->layer(m_activeLayerIndex);
    if (!targetLayer) return;
    for (Shap *original : m_clipboard) {
        Shap* clone = original->clone();

        if(targetLayer)
            targetLayer->addShape(clone);
        clone->move(QPoint(10, 10));
    }
    update();
}


void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        switch (m_currentCategory) {
        case ToolCategory::Selection:
            handleSelectionPress(event);
            break;
        case ToolCategory::Transformation:
            handleTransformationPress(event);
            break;
        case ToolCategory::Drawing:
            if (m_currentShapeType == ShapeType::Polyline) {
                if (!m_buildingPolyline) {
                    m_tempPolyline = new Polyline({event->pos()}, m_currentLineColor);
                    m_buildingPolyline = true;
                } else {
                    m_tempPolyline->addPoint(event->pos());
                }
                update();
            } else {
                startDrawing(event->pos());
            }
            break;
        }
    }else if(event->button() == Qt::RightButton){
        if (m_buildingPolyline) {
            finishPolyline();
        } else {
            Shap *hit = m_layerManager->shapeAt(event->pos());
            if (hit) {
                QMenu *menu = hit->createContextMenu(this);
                QAction *selected = menu->exec(event->globalPos());
                if (selected) hit->onContextMenuAction(selected->text());
                delete menu;
                update();
            }
        }
    }
}

void Canvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_buildingPolyline) {
        finishPolyline();
    } else {
        QWidget::mouseDoubleClickEvent(event);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{

    if (m_drawing) {
        m_endPoint = event->pos();
        update();
        return;
    }
    if (m_buildingPolyline && m_tempPolyline) {
        m_lastCursorPos = event->pos();
        update();
    }
    if(m_rubberBandActive){
        m_selectionRubberBandRect.setBottomRight(event->pos());
        update();
        return;
    }
    if(m_currentCategory == ToolCategory::Transformation && m_transforming){
        QPoint delta = event->pos() - m_transformStartPoint;
        if(event->modifiers() & Qt::ControlModifier){
            double factor = 1.0 + delta.y() * 0.01;
            for(int i = 0; i < m_selectedShapes.size(); ++i){
                Shap *s = m_selectedShapes[i];
                s->scale(factor, s->center());
            }
        }else if(event->modifiers() & Qt::ShiftModifier){
            double angle = atan2(delta.y(), delta.x());
            for(Shap *s : m_selectedShapes)
                s->rotate(angle);
        }else{

            moveSelectedBy(delta);
        }
        m_transformStartPoint = event->pos();
        update();
        return;
    }
    if(m_currentCategory == ToolCategory::Selection && !m_selectedShapes.isEmpty() && (event->buttons() & Qt::LeftButton)){
        QPoint delta = event->pos() - m_dragStart;
        moveSelectedBy(delta);
        m_dragStart = event->pos();
        update();
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (m_drawing) {
        finishDrawing();
        m_drawing = false;
        update();
        return;
    }

    if (m_rubberBandActive) {
        QRect rect = m_selectionRubberBandRect.normalized();
        QList<Shap*> shapesInRect = m_layerManager->shapesInRect(rect);
        if (!(QGuiApplication::keyboardModifiers() & Qt::ControlModifier))
            clearSelection();
        for (Shap* s : shapesInRect)
            addToSelection(s, false);
        m_rubberBandActive = false;
        update();
        return;
    }

    if (m_moving && !m_selectedShapes.isEmpty() && !m_moveDelta.isNull()) {
        moveSelectedBy(-m_moveDelta);

        if (m_selectedShapes.size() == 1) {
            m_undoStack->push(new MoveShapeCommand(this, m_selectedShapes.first(), m_moveDelta));
        } else {
            QUndoCommand* macro = new QUndoCommand(tr("Move %1 shapes").arg(m_selectedShapes.size()));
            for (Shap* s : m_selectedShapes) {
                new MoveShapeCommand(this, s, m_moveDelta, macro);
            }
            m_undoStack->push(macro);
        }

        m_moving = false;
        m_moveDelta = QPoint(0, 0);
        update();
        return;
    }

    if (m_transforming && m_currentCategory == ToolCategory::Transformation) {
        QList<double> finalScales, finalAngles;
        QList<QPoint> finalCenters;
        for (Shap* s : m_selectedShapes) {
            finalScales.append(s->scaleFactor());
            finalAngles.append(s->rotation());
            finalCenters.append(s->center());
        }

        for (int i = 0; i < m_selectedShapes.size(); ++i) {
            Shap* s = m_selectedShapes[i];
            double curScale = s->scaleFactor();
            double initScale = m_initialScales[i];
            if (!qFuzzyCompare(curScale, initScale))
                s->scale(initScale / curScale, s->center());

            double curAngle = s->rotation();
            double initAngle = m_initialAngles[i];
            if (!qFuzzyCompare(curAngle, initAngle))
                s->rotate(initAngle - curAngle);
        }

        if (!m_moveDelta.isNull()) {
            moveSelectedBy(-m_moveDelta);
        }

        QUndoCommand* macro = new QUndoCommand(tr("Transform shapes"));
        for (int i = 0; i < m_selectedShapes.size(); ++i) {
            Shap* s = m_selectedShapes[i];
            double scaleFactor = finalScales[i] / m_initialScales[i];
            if (!qFuzzyCompare(scaleFactor, 1.0))
                new ScaleShapeCommand(this, s, scaleFactor, macro);
            double angleDelta = finalAngles[i] - m_initialAngles[i];
            if (!qFuzzyIsNull(angleDelta))
                new RotateShapeCommand(this, s, angleDelta, macro);
            QPoint moveDelta = finalCenters[i] - (finalCenters[i] - m_moveDelta); // = m_moveDelta
            if (!moveDelta.isNull())
                new MoveShapeCommand(this, s, moveDelta, macro);
        }

        if (macro->childCount() > 0)
            m_undoStack->push(macro);
        else
            delete macro;

        m_transforming = false;
        m_moving = false;
        m_moveDelta = QPoint(0, 0);
        update();
        return;
    }

    m_moving = false;
    m_transforming = false;
}

void Canvas::handleSelectionPress(QMouseEvent *event)
{
    QPoint pos = event->pos();
    Shap *hit = m_layerManager->shapeAt(pos);

    if (hit) {
        if (event->modifiers() & Qt::ControlModifier) {
            toggleSelection(hit);
        } else if (!m_selectedShapes.contains(hit)) {
            addToSelection(hit, true);
        }
        m_dragStart = pos;
        m_moveDelta = QPoint(0, 0);
        m_moving = true;
        m_transforming = false;
    } else {
        if (!(event->modifiers() & Qt::ControlModifier))
            clearSelection();
        m_rubberBandActive = true;
        m_selectionRubberBandRect = QRect(pos, pos);
        m_moving = false;
    }
}

void Canvas::handleTransformationPress(QMouseEvent *event)
{
    QPoint pos = event->pos();
    Shap *hit = m_layerManager->shapeAt(pos);

    if (hit) {
        m_dragStart = pos;
        m_moveDelta = QPoint(0, 0);
        m_moving = true;
    }

    if (!m_selectedShapes.isEmpty()) {
        m_transforming = true;
        m_transformStartPoint = pos;
        m_initialScales.clear();
        m_initialAngles.clear();
        for (Shap* s : m_selectedShapes) {
            m_initialScales.append(s->scaleFactor());
            m_initialAngles.append(s->rotation());
        }
    }
}

void Canvas::startDrawing(const QPoint &pos){
    if (m_currentShapeType == ShapeType::Polyline) {
        if (!m_buildingPolyline) {
            m_tempPolyline = new Polyline({pos}, m_currentLineColor);
            m_buildingPolyline = true;
        } else {
            m_tempPolyline->addPoint(pos);
        }
        update();
    } else {
        m_drawing = true;
        m_startPoint = pos;
        m_endPoint = pos;
        update();
    };
}

void Canvas::finishPolyline()
{
    if (m_tempPolyline && m_tempPolyline->points().size() >= 2) {
        m_tempPolyline->setLineWidth(m_currentLineWidth);
        m_tempPolyline->setFillColor(m_currentFillColor);
        Layer* target = m_layerManager->layer(m_activeLayerIndex);
        if (target) {
            m_undoStack->push(new AddShapeCommand(this, m_tempPolyline, m_activeLayerIndex));
        } else {
            delete m_tempPolyline;
        }
    } else {
        delete m_tempPolyline;
    }
    m_tempPolyline = nullptr;
    m_buildingPolyline = false;
    update();
}

void Canvas::finishDrawing(){
    Shap *shape = createShape(m_currentShapeType, m_startPoint, m_endPoint);
    if(shape){
        shape->setLineWidth(m_currentLineWidth);
        shape->setLineColor(m_currentLineColor);
        shape->setFillColor(m_currentFillColor);
        m_undoStack->push(new AddShapeCommand(this, shape, m_activeLayerIndex));
    }
}

void Canvas::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (Layer *layer : m_layerManager->layers()) {
        for (Shap *shape : layer->shapes()) {
            shape->drawShape(painter);
            if (shape->isSelected()) {
                QRect rect = shape->boundingRect();
                painter.save();
                painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
                painter.setBrush(Qt::NoBrush);
                painter.drawRect(rect);
                painter.restore();
            }
        }
    }

    if (m_buildingPolyline && m_tempPolyline) {
        m_tempPolyline->draw(painter);
        if (!m_tempPolyline->points().isEmpty()) {
            QPoint last = m_tempPolyline->points().last();
            QPoint cursor = mapFromGlobal(QCursor::pos());
            painter.setPen(QPen(m_currentLineColor, m_currentLineWidth, Qt::DashLine));
            painter.drawLine(last, cursor);
        }
    } else if (m_drawing) {
        Shap *temp = createShape(m_currentShapeType, m_startPoint, m_endPoint);
        if(temp){
            temp->setLineWidth(m_currentLineWidth);
            temp->setLineColor(m_currentLineColor);
            temp->setFillColor(m_currentFillColor);
            temp->drawShape(painter);
            delete temp;
        }

    }
    if(m_rubberBandActive){
        painter.save();
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.setBrush(QBrush(QColor(128, 128, 255, 64)));
        painter.drawRect(m_selectionRubberBandRect.normalized());
        painter.restore();
    }
    if (!m_selectedShapes.isEmpty()) {
        m_selectedShapes.first()->showInformation(painter, height());
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        deleteSelectedShapes();
        return;
    }
    int step = 1;
    QPoint delta;
    switch (event->key()) {
    case Qt::Key_Left:  delta = QPoint(-step, 0); break;
    case Qt::Key_Right: delta = QPoint( step, 0); break;
    case Qt::Key_Up:    delta = QPoint(0, -step); break;
    case Qt::Key_Down:  delta = QPoint(0,  step); break;
    default: QWidget::keyPressEvent(event); return;
    }
    moveSelectedBy(delta);
    update();
}

void Canvas::clearSelection()
{
    for (Shap *s : m_selectedShapes)
        s->setSelected(false);
    m_selectedShapes.clear();
    update();
}

void Canvas::addToSelection(Shap *shape, bool clearOthers)
{
    if (clearOthers) clearSelection();
    if (shape && !m_selectedShapes.contains(shape)) {
        shape->setSelected(true);
        m_selectedShapes.append(shape);
    }
    update();
}

void Canvas::toggleSelection(Shap *shape)
{
    if (m_selectedShapes.contains(shape)) {
        shape->setSelected(false);
        m_selectedShapes.removeAll(shape);
    } else {
        shape->setSelected(true);
        m_selectedShapes.append(shape);
    }
    update();
}

void Canvas::moveSelectedBy(const QPoint &delta)
{
    for (Shap *s : m_selectedShapes)
        s->move(delta);
}

void Canvas::rotateSelected(double angleDelta)
{
    for (Shap *s : m_selectedShapes)
        s->rotate(angleDelta);
}

void Canvas::scaleSelected(double factor)
{
    for (Shap *s : m_selectedShapes)
        s->scale(factor, s->center());
}

void Canvas::onLayersChanged()
{
    update();
}

ShapeType Canvas::shapeTypeFromString(const QString &type) const
{
    if(type == "Rectangle") return ShapeType::Rectangle;
    if(type == "Triangle") return ShapeType::Triangle;
    if (type == "Circle") return ShapeType::Circle;
    if (type == "Ellipse") return ShapeType::Ellipse;
    if (type == "Hexagon") return ShapeType::Hexagon;
    if (type == "Rhomb") return ShapeType::Rhomb;
    if (type == "Square") return ShapeType::Square;
    if (type == "5-star") return ShapeType::Star5;
    if (type == "6-star") return ShapeType::Star6;
    if (type == "8-star") return ShapeType::Star8;
    if (type == "Polyline") return ShapeType::Polyline;
    return ShapeType::Rectangle;
}

Shap* Canvas::createShape(ShapeType type, const QPoint &start, const QPoint &end){
    switch(type){
    case ShapeType::Rectangle:
        return new Rectangle(start, end, m_currentLineColor);
    case ShapeType::Triangle:
        return new Triangle(start, end, QPoint(start.x() - (end.x() - start.x()), end.y()), m_currentLineColor);
    case ShapeType::Circle:
        return new Circle(start, end, m_currentLineColor);
    case ShapeType::Ellipse:
        return new Ellipse(start, end, m_currentLineColor);
    case ShapeType::Hexagon:
        return new Hexagon(start, end, m_currentLineColor);
    case ShapeType::Rhomb:
        return new Rhomb(start, end, m_currentLineColor);
    case ShapeType::Square:
        return new Square(start, end, m_currentLineColor);
    case ShapeType::Star5:
        return new Stars(start, end, 5, m_currentLineColor);
    case ShapeType::Star6:
        return new Stars(start, end, 6, m_currentLineColor);
    case ShapeType::Star8:
        return new Stars(start, end, 8, m_currentLineColor);
    case ShapeType::Polyline:
        return new Polyline({start, end}, m_currentLineColor);
    default:
        return nullptr;
    }
}

void Canvas::setActiveLayer(int index){
    if(index >= 0 && index < m_layerManager->layerCount())
        m_activeLayerIndex = index;
}
