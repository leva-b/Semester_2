#ifndef ROTATESHAPECOMMAND_H
#define ROTATESHAPECOMMAND_H

#include "command.h"
#include "shap.h"

class RotateShapeCommand : public Command
{
public:
    RotateShapeCommand(Canvas* canvas, Shap* shape, double angle, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override { return 1003; }
    bool mergeWith(const QUndoCommand* other) override;

private:
    Shap* m_shape;
    double m_angle;
};

#endif // ROTATESHAPECOMMAND_H
