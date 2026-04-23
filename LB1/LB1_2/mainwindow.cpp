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
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    QAction* undoAction = m_canvas->undoStack()->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcut(QKeySequence::Undo); // Ctrl+Z
    QAction* redoAction = m_canvas->undoStack()->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcut(Qt::CTRL | Qt::Key_Y);

    connect(m_canvas->undoStack(), &QUndoStack::canRedoChanged, redoAction, &QAction::setEnabled);
    connect(m_canvas->undoStack(), &QUndoStack::canUndoChanged, undoAction, &QAction::setEnabled);

    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
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

    connect(m_canvas->layerManager(), &LayerManager::layersChanged, this, &MainWindow::refreshLayerList);
    connect(m_addLayerBtn, &QPushButton::clicked, this, &MainWindow::addLayer);
    connect(m_removeLayerBtn, &QPushButton::clicked, this, &MainWindow::removeLayer);
    connect(m_upBtn, &QPushButton::clicked, this, &MainWindow::moveLayerUp);
    connect(m_downBtn, &QPushButton::clicked, this, &MainWindow::moveLayerDown);
    connect(m_moveToLayerBtn, &QPushButton::clicked, this, &MainWindow::moveSelectedToCurrentLayer);
    connect(m_layerList, &QListWidget::currentRowChanged, this,  [this](int row) {
        if (row >= 0)
            m_canvas->setActiveLayer(row);
    });

    refreshLayerList();
    if (m_canvas->layerManager()->layerCount() > 0) {
        m_layerList->setCurrentRow(0);
        m_canvas->setActiveLayer(0);
    }
}

void MainWindow::refreshLayerList() {
    int oldRow = m_layerList->currentRow();
    m_layerList->clear();
    int count = m_canvas->layerManager()->layerCount();
    for (int i = 0; i < count; ++i)
        m_layerList->addItem(m_canvas->layerManager()->layer(i)->name());

    if (count > 0) {
        int newRow = qBound(0, oldRow, count - 1);
        m_layerList->setCurrentRow(newRow);
        m_canvas->setActiveLayer(newRow);
    } else {
        m_canvas->setActiveLayer(-1);
    }
}

void MainWindow::addLayer()
{
    m_canvas->layerManager()->addLayer();
}

void MainWindow::removeLayer()
{
    int idx = m_layerList->currentRow();
    if (idx >= 0 && m_canvas->layerManager()->layerCount() > 1)
        m_canvas->layerManager()->removeLayer(idx);
}

void MainWindow::moveLayerUp()
{
    int idx = m_layerList->currentRow();
    if (idx > 0) {
        m_canvas->layerManager()->moveLayerUp(idx);
        m_layerList->setCurrentRow(idx - 1);
    }
}

void MainWindow::moveLayerDown()
{
    int idx = m_layerList->currentRow();
    if (idx >= 0 && idx < m_canvas->layerManager()->layerCount() - 1) {
        m_canvas->layerManager()->moveLayerDown(idx);
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

