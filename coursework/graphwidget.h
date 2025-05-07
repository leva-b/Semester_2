#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QVBoxLayout>
#include "mazefromfileparser.h"

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void updateGraph( MazeFromFileParser* parser);

public slots:
    void findShortestPath();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    struct Node {
        QPoint pos;
        int id;
    };

    struct Edge {
        int from, to;
        int weight;
    };
    QPushButton *findPathButton; // Добавляем кнопку
    QVBoxLayout *mainLayout;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<int> shortestPath;
    int startNode = -1;
    int endNode = -1;
    int startY;

//    void drawPath(QPainter &painter);
    Node* findNodeAt(const QPoint &pos);
    bool isAddingEdge = false;
    bool isSelectingStart = false;
    bool isSelectingEnd = false;
    Node *firstNode = nullptr;
    void setupUI();
};

#endif // GRAPHWIDGET_H
