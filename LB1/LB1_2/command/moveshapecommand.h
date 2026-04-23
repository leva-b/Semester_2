#ifndef MOVESHAPECOMMAND_H
#define MOVESHAPECOMMAND_H

#include "command.h"
#include <QPoint>

class Shap;

class MoveShapeCommand : public Command
{
public:
    MoveShapeCommand(Canvas* canvas, Shap* shape, const QPoint& delta, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override { return 1001; }
    bool mergeWith(const QUndoCommand* other) override;

private:
    Shap* m_shape;
    QPoint m_delta;
};

#endif // MOVESHAPECOMMAND_H
