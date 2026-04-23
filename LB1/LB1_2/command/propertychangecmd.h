#ifndef PROPERTYCHANGECMD_H
#define PROPERTYCHANGECMD_H

#include "command.h"
#include "shap.h"
#include <functional>
#include "canvas.h"

template<typename T>
class PropertyChangeCommand : public Command
{
public:
    using Setter = std::function<void(Shap*, const T&)>;
    using Getter = std::function<T(const Shap*)>;

    PropertyChangeCommand(Canvas* canvas, Shap* shape, const T& newValue,
                          Setter setter, Getter getter, const QString& text,
                          QUndoCommand* parent = nullptr)
        : Command(canvas, parent), m_shape(shape), m_newValue(newValue),
        m_setter(setter), m_getter(getter)
    {
        m_oldValue = m_getter(m_shape);
        setText(text);
    }

    void undo() override
    {
        m_setter(m_shape, m_oldValue);
        m_canvas->update();
    }

    void redo() override
    {
        m_setter(m_shape, m_newValue);
        m_canvas->update();
    }

private:
    Shap* m_shape;
    T m_oldValue;
    T m_newValue;
    Setter m_setter;
    Getter m_getter;
};

#endif // PROPERTYCHANGECMD_H
