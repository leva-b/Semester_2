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
    explicit GraphWidget(QWidget *parent = nullptr, MazeFromFileParser* parser = nullptr);


signals:  // Секция сигналов
    void requestParser();

public slots:
    void findShortestPath();
    void deleteAllGraph();
    void updateGraphFromMaze();

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

    QPushButton *updateGraph;
    QPushButton *deleteGraph;
    QPushButton *findPathButton;

    Graph graph;
    QVBoxLayout *mainLayout;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<size_t> shortestPath;
    int startNode = -1, endNode = -1;
    int startY;
    QColor pathColor = QColor("#a02140");
    MazeFromFileParser* parser;
    QTimer* animationTimer;
    int currentAnimationStep = 0;
    std::vector<int> animatedPath;
    std::vector<QLineF> animatedEdges;

    Node* findNodeAt(const QPoint &pos, size_t& curID);
    Node* findNodeAt(const QPoint &pos);
    bool isAddingEdge = false;
    bool isSelectingStart = false;
    bool isSelectingEnd = false;
    Node *firstNode = nullptr;
    void shortenLine(QLineF& line, float offset);
};

#endif // GRAPHWIDGET_H
