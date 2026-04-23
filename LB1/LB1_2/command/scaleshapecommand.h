#ifndef SCALESHAPECOMMAND_H
#define SCALESHAPECOMMAND_H
#include "command.h"
#include "shap.h"
// scaleshapecommand.h
class ScaleShapeCommand : public Command
{
public:
    ScaleShapeCommand(Canvas* canvas, Shap* shape, double factor, QUndoCommand* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override { return 1002; }
    bool mergeWith(const QUndoCommand* other) override;

private:
    Shap* m_shape;
    double m_factor;
};

#endif // SCALESHAPECOMMAND_H
