#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QList>
#include "layer.h"

class LayerManager : public QObject
{
    Q_OBJECT
public:
    explicit LayerManager(QObject *parent = nullptr);
    ~LayerManager();

    void addLayer(const QString &name = "New Layer");
    void removeLayer(int index);
    void moveLayerUp(int index);
    void moveLayerDown(int index);

    Layer* layer(int index) const;
    int layerCount() const;
    QList<Layer*> layers() const;

    QList<Shap*> getAllShapes() const;
    Shap* shapeAt(const QPoint &pos, const QTransform &viewTransform = QTransform()) const;
    QList<Shap*> shapesInRect(const QRect &rect) const;

    void addShapeToLayer(Shap *shape, int layerIndex);
    void removeShape(Shap *shape);
    void moveShapeToLayer(Shap *shape, int targetLayerIndex);

signals:
    void layersChanged();

private:
    QList<Layer*> m_layers;
};

#endif // LAYERMANAGER_H
