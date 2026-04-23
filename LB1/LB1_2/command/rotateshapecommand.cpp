#include "rotateshapecommand.h"
#include "canvas.h"

RotateShapeCommand::RotateShapeCommand(Canvas* canvas, Shap* shape, double angle, QUndoCommand* parent)
    : Command(canvas, parent), m_shape(shape), m_angle(angle)
{
    setText(QObject::tr("Rotate %1").arg(shape->typeName()));
}

void RotateShapeCommand::undo()
{
    m_shape->rotate(-m_angle);
    m_canvas->update();
}

void RotateShapeCommand::redo()
{
    m_shape->rotate(m_angle);
    m_canvas->update();
}

bool RotateShapeCommand::mergeWith(const QUndoCommand* other)
{
    if (other->id() != id())
        return false;
    const RotateShapeCommand* cmd = static_cast<const RotateShapeCommand*>(other);
    if (cmd->m_shape != m_shape)
        return false;
    m_angle += cmd->m_angle;
    return true;
}
