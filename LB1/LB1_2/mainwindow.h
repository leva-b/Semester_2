#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rectangle.h>
#include "canvas.h"
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void refreshLayerList();
    void addLayer();
    void removeLayer();
    void moveLayerUp();
    void moveLayerDown();
    void moveSelectedToCurrentLayer();

private:
    Ui::MainWindow *ui;
    Canvas *m_canvas;
    QListWidget *m_layerList;
    QPushButton *m_addLayerBtn, *m_removeLayerBtn, *m_upBtn, *m_downBtn, *m_moveToLayerBtn;
};
#endif // MAINWINDOW_H
