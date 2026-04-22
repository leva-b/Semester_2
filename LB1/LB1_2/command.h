#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>

class Canvas;

class Command : public QUndoCommand
{
public:
    explicit Command(Canvas* canvas, QUndoCommand* parent = nullptr);
    virtual ~Command() = default;

    virtual void undo() override = 0;
    virtual void redo() override = 0;

protected:
    Canvas* m_canvas;
};

#endif // COMMAND_H
