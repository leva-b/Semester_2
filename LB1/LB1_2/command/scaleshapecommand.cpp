#include "scaleshapecommand.h"
#include "canvas.h"

ScaleShapeCommand::ScaleShapeCommand(Canvas* canvas, Shap* shape, double factor, QUndoCommand* parent)
    : Command(canvas, parent), m_shape(shape), m_factor(factor)
{
    setText(QObject::tr("Scale %1").arg(shape->typeName()));
}

void ScaleShapeCommand::undo()
{
    m_shape->scale(1.0 / m_factor, m_shape->center());
    m_canvas->update();
}

void ScaleShapeCommand::redo()
{
    m_shape->scale(m_factor, m_shape->center());
    m_canvas->update();
}

bool ScaleShapeCommand::mergeWith(const QUndoCommand* other)
{
    if (other->id() != id())
        return false;
    const ScaleShapeCommand* cmd = static_cast<const ScaleShapeCommand*>(other);
    if (cmd->m_shape != m_shape)
        return false;
    m_factor *= cmd->m_factor; // последовательное масштабирование перемножается
    return true;
}
