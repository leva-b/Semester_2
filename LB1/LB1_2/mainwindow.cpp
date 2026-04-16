#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDockWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_canvas = new Canvas(this);
    setCentralWidget(m_canvas);

    // Layer dock
    QDockWidget *layerDock = new QDockWidget("Layers", this);
    QWidget *layerPanel = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(layerPanel);
    m_layerList = new QListWidget;
    layout->addWidget(m_layerList);

    m_addLayerBtn = new QPushButton("Add Layer");
    m_removeLayerBtn = new QPushButton("Remove Layer");
    m_upBtn = new QPushButton("Move Up");
    m_downBtn = new QPushButton("Move Down");
    m_moveToLayerBtn = new QPushButton("Move Selected to This Layer");

    layout->addWidget(m_addLayerBtn);
    layout->addWidget(m_removeLayerBtn);
    layout->addWidget(m_upBtn);
    layout->addWidget(m_downBtn);
    layout->addWidget(m_moveToLayerBtn);

    layerPanel->setLayout(layout);
    layerDock->setWidget(layerPanel);
    addDockWidget(Qt::RightDockWidgetArea, layerDock);

    connect(m_addLayerBtn, &QPushButton::clicked, this, &MainWindow::addLayer);
    connect(m_removeLayerBtn, &QPushButton::clicked, this, &MainWindow::removeLayer);
    connect(m_upBtn, &QPushButton::clicked, this, &MainWindow::moveLayerUp);
    connect(m_downBtn, &QPushButton::clicked, this, &MainWindow::moveLayerDown);
    connect(m_moveToLayerBtn, &QPushButton::clicked, this, &MainWindow::moveSelectedToCurrentLayer);
    connect(m_layerList, &QListWidget::currentRowChanged, this, &MainWindow::refreshLayerList);

    refreshLayerList();
}

void MainWindow::refreshLayerList()
{
    m_layerList->clear();
    for (int i = 0; i < m_canvas->layerManager()->layerCount(); ++i)
        m_layerList->addItem(m_canvas->layerManager()->layer(i)->name());
}

void MainWindow::addLayer()
{
    m_canvas->layerManager()->addLayer("New Layer");
    refreshLayerList();
}

void MainWindow::removeLayer()
{
    int idx = m_layerList->currentRow();
    if (idx >= 0)
        m_canvas->layerManager()->removeLayer(idx);
    refreshLayerList();
}

void MainWindow::moveLayerUp()
{
    int idx = m_layerList->currentRow();
    if (idx > 0) {
        m_canvas->layerManager()->moveLayerUp(idx);
        refreshLayerList();
        m_layerList->setCurrentRow(idx - 1);
    }
}

void MainWindow::moveLayerDown()
{
    int idx = m_layerList->currentRow();
    if (idx >= 0 && idx < m_canvas->layerManager()->layerCount() - 1) {
        m_canvas->layerManager()->moveLayerDown(idx);
        refreshLayerList();
        m_layerList->setCurrentRow(idx + 1);
    }
}

void MainWindow::moveSelectedToCurrentLayer()
{
    int targetIdx = m_layerList->currentRow();
    if (targetIdx < 0) return;
    // we need to get selected shapes from canvas – add a getter
    const auto &selected = m_canvas->selectedShapes(); // you must add this getter in Canvas
    for (Shap *s : selected)
        m_canvas->layerManager()->moveShapeToLayer(s, targetIdx);
    m_canvas->update();
}
