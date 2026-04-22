#include "command.h"
#include "canvas.h"

Command::Command(Canvas* canvas, QUndoCommand* parent)
    : QUndoCommand(parent), m_canvas(canvas)
{}
