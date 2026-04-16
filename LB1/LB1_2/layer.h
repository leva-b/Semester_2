#ifndef LAYER_H
#define LAYER_H

#include <QList>
#include <QString>
#include "shap.h"

class Layer
{
public:
    explicit Layer(const QString &name = "Layer");
    ~Layer();

    QString name() const;
    void setName(const QString &name);

    QList<Shap*> shapes() const;
    void addShape(Shap *shape);
    void removeShape(Shap *shape);
    bool contains(Shap *shape) const;

    void clear();

private:
    QString m_name;
    QList<Shap*> m_shapes;
};

#endif // LAYER_H
