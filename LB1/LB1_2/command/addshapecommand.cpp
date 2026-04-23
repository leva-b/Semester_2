#include "addshapecommand.h"
#include "canvas.h"
#include "layermanager.h"

AddShapeCommand::AddShapeCommand(Canvas* canvas, Shap* shape, int layerIndex, QUndoCommand* parent)
    : Command(canvas, parent), m_shape(shape), m_layerIndex(layerIndex), m_ownsShape(true)
{
    setText(QObject::tr("Add %1").arg(shape->typeName()));
}

AddShapeCommand::~AddShapeCommand()
{
    if (m_ownsShape)
        delete m_shape;
}

void AddShapeCommand::undo()
{
    Layer* layer = m_canvas->layerManager()->layer(m_layerIndex);
    if (layer) {
        layer->removeShape(m_shape);
        m_ownsShape = true;
    }
    m_canvas->update();
}

void AddShapeCommand::redo()
{
    Layer* layer = m_canvas->layerManager()->layer(m_layerIndex);
    if (layer) {
        layer->addShape(m_shape);
        m_ownsShape = false;
    }
    m_canvas->update();
}
