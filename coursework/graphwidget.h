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
    QSize sizeHint() const override;
    void setParser(MazeFromFileParser* newParser);

signals:
    void requestParser();

public slots:
    void findShortestPath();
    void deleteAllGraph();
    void updateGraphFromMaze();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

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
    QPushButton *pathDisplayButton;

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

    double scaleFactor = 1.0;
    double minScale = 0.1;
    double maxScale = 2.0;
    const int minNodeSize = 5;
    const int maxNodeSize = 20;
    const int baseNodeSize = 20;
    QPointF scrollOffset;
    bool isAnimating = false;

    Node* findNodeAt(const QPoint &pos, size_t& curID);
    Node* findNodeAt(const QPoint &pos);
    bool isAddingEdge = false;
    bool isSelectingStart = false;
    bool isSelectingEnd = false;
    Node *firstNode = nullptr;
    void shortenLine(QLineF& line, float offset);
    void initializeMinMaxScale();
    void limitScrollOffset();
    void getGraphBounds(QRectF& bounds) const;
    void pathDisplay();
};

#endif // GRAPHWIDGET_H
