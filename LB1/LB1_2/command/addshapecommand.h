#ifndef ADDSHAPECOMMAND_H
#define ADDSHAPECOMMAND_H

#include "command.h"
#include "shap.h"

class AddShapeCommand : public Command
{
public:
    AddShapeCommand(Canvas* canvas, Shap* shape, int layerIndex, QUndoCommand* parent = nullptr);
    ~AddShapeCommand();

    void undo() override;
    void redo() override;

private:
    Shap* m_shape;
    int m_layerIndex;
    bool m_ownsShape; // владеет ли команда объектом
};

#endif // ADDSHAPECOMMAND_H
