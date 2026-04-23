#ifndef REMOVESHAPECOMMAND_H
#define REMOVESHAPECOMMAND_H

#include "command.h"
#include "shap.h"

class RemoveShapeCommand : public Command
{
public:
    RemoveShapeCommand(Canvas* canvas, Shap* shape, int layerIndex, QUndoCommand* parent = nullptr);
    ~RemoveShapeCommand();

    void undo() override;
    void redo() override;

private:
    Shap* m_shape;
    int m_layerIndex;
    bool m_ownsShape;
};

#endif // REMOVESHAPECOMMAND_H
