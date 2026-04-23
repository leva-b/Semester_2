#include "removeshapecommand.h"
#include "canvas.h"
#include "layermanager.h"

RemoveShapeCommand::RemoveShapeCommand(Canvas* canvas, Shap* shape, int layerIndex, QUndoCommand* parent)
    : Command(canvas, parent), m_shape(shape), m_layerIndex(layerIndex), m_ownsShape(false)
{
    setText(QObject::tr("Remove %1").arg(shape->typeName()));
}

RemoveShapeCommand::~RemoveShapeCommand()
{
    if (m_ownsShape)
        delete m_shape;
}

void RemoveShapeCommand::undo()
{
    Layer* layer = m_canvas->layerManager()->layer(m_layerIndex);
    if (layer) {
        layer->addShape(m_shape);
        m_ownsShape = false;
    }
    m_canvas->update();
}

void RemoveShapeCommand::redo()
{
    Layer* layer = m_canvas->layerManager()->layer(m_layerIndex);
    if (layer) {
        layer->removeShape(m_shape);
        m_ownsShape = true;
    }
    m_canvas->update();
}
