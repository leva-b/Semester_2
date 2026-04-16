#include "layer.h"

Layer::Layer(const QString &name) : m_name(name) {}
Layer::~Layer() { qDeleteAll(m_shapes); }

QString Layer::name() const { return m_name; }
void Layer::setName(const QString &name) { m_name = name; }

QList<Shap*> Layer::shapes() const { return m_shapes; }
void Layer::addShape(Shap *shape) { if (!m_shapes.contains(shape)) m_shapes.append(shape); }
void Layer::removeShape(Shap *shape) { m_shapes.removeAll(shape); }
bool Layer::contains(Shap *shape) const { return m_shapes.contains(shape); }
void Layer::clear() { qDeleteAll(m_shapes); m_shapes.clear(); }
