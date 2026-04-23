#include "moveshapecommand.h"
#include "canvas.h"
#include "shap.h"

MoveShapeCommand::MoveShapeCommand(Canvas* canvas, Shap* shape, const QPoint& delta, QUndoCommand* parent)
    : Command(canvas, parent), m_shape(shape), m_delta(delta)
{
    setText(QObject::tr("Move %1").arg(shape->typeName()));
}

void MoveShapeCommand::undo()
{
    m_shape->move(-m_delta);
    m_canvas->update();
}

void MoveShapeCommand::redo()
{
    m_shape->move(m_delta);
    m_canvas->update();
}

bool MoveShapeCommand::mergeWith(const QUndoCommand* other)
{
    if (other->id() != id())
        return false;
    const MoveShapeCommand* cmd = static_cast<const MoveShapeCommand*>(other);
    if (cmd->m_shape != m_shape)
        return false;
    m_delta += cmd->m_delta;
    return true;
}
