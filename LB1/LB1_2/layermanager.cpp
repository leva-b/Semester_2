#include "layermanager.h"
#include <QTransform>
#include <QPoint>
#include <algorithm>

LayerManager::LayerManager(QObject *parent) : QObject(parent)
{
    // default layer
    addLayer("Default");
}

LayerManager::~LayerManager()
{
    qDeleteAll(m_layers);
}

void LayerManager::addLayer(const QString &name)
{
    m_layers.append(new Layer(name));
    emit layersChanged();
}

void LayerManager::removeLayer(int index)
{
    if (index < 0 || index >= m_layers.size()) return;
    Layer *l = m_layers.takeAt(index);
    // move shapes from removed layer to default layer (index 0)
    if (!m_layers.isEmpty()) {
        for (Shap *shape : l->shapes())
            m_layers.first()->addShape(shape);
    }
    delete l;
    emit layersChanged();
}

void LayerManager::moveLayerUp(int index)
{
    if (index <= 0 || index >= m_layers.size()) return;
    std::swap(m_layers[index], m_layers[index - 1]);
    emit layersChanged();
}

void LayerManager::moveLayerDown(int index)
{
    if (index < 0 || index >= m_layers.size() - 1) return;
    std::swap(m_layers[index], m_layers[index + 1]);
    emit layersChanged();
}

Layer* LayerManager::layer(int index) const
{
    return (index >= 0 && index < m_layers.size()) ? m_layers[index] : nullptr;
}

int LayerManager::layerCount() const { return m_layers.size(); }

QList<Layer*> LayerManager::layers() const { return m_layers; }

QList<Shap*> LayerManager::getAllShapes() const
{
    QList<Shap*> all;
    for (Layer *l : m_layers)
        all.append(l->shapes());
    return all;
}

Shap* LayerManager::shapeAt(const QPoint &pos, const QTransform &viewTransform) const
{
    // iterate layers from topmost (last) to bottom
    for (int i = m_layers.size() - 1; i >= 0; --i) {
        for (Shap *shape : m_layers[i]->shapes()) {
            if (shape->contains(pos))
                return shape;
        }
    }
    return nullptr;
}

void LayerManager::addShapeToLayer(Shap *shape, int layerIndex)
{
    if (layerIndex < 0 || layerIndex >= m_layers.size()) return;
    m_layers[layerIndex]->addShape(shape);
    emit layersChanged();
}

void LayerManager::removeShape(Shap *shape)
{
    for (Layer *l : m_layers) {
        if (l->contains(shape)) {
            l->removeShape(shape);
            break;
        }
    }
    emit layersChanged();
}

void LayerManager::moveShapeToLayer(Shap *shape, int targetLayerIndex)
{
    if (targetLayerIndex < 0 || targetLayerIndex >= m_layers.size()) return;
    for (Layer *l : m_layers) {
        if (l->contains(shape)) {
            l->removeShape(shape);
            break;
        }
    }
    m_layers[targetLayerIndex]->addShape(shape);
    emit layersChanged();
}

QList<Shap*> LayerManager::shapesInRect(const QRect &rect) const
{
    QList<Shap*> result;
    for (Layer *layer : m_layers) {
        for (Shap *shape : layer->shapes()) {
            QRect shapeRect = shape->boundingRect();
            if (rect.intersects(shapeRect))
                result.append(shape);
        }
    }
    return result;
}
