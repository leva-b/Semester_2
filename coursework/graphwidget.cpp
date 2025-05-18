#include "graphwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <cmath>
#include <QTimer>
#include <QMessageBox>

GraphWidget::GraphWidget(QWidget *parent, MazeFromFileParser* parser) : QWidget(parent), parser(parser)
{
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        if ((size_t)currentAnimationStep < shortestPath.size() * 2 - 1) {
            currentAnimationStep++;
            update();
        } else {
            animationTimer->stop();
        }
    });

    mainLayout = new QVBoxLayout(this);
    updateGraph = new QPushButton("Update graph from maze", this);
    deleteGraph = new QPushButton("Delete the graph", this);
    findPathButton = new QPushButton("Find a way", this);
    connect(findPathButton, &QPushButton::clicked, this, &GraphWidget::findShortestPath);
    connect(deleteGraph, &QPushButton::clicked, this, &GraphWidget::deleteAllGraph);
    connect(updateGraph, &QPushButton::clicked, this, &GraphWidget::updateGraphFromMaze);
    // findPathButton->setStyleSheet("QPushButton {"
    //                               "background-color: #4CAF50;"
    //                               "color: white;"
    //                               "border: none;"
    //                               "padding: 8px;"
    //                               "font-size: 14px;"
    //                               "}");
    auto buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(deleteGraph);
    buttonLayout->addWidget(updateGraph);
    buttonLayout->addWidget(findPathButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
    startY = updateGraph->height();
}

void GraphWidget::updateGraphFromMaze() {

    if (!parser) {
        qDebug() << "Parser is null";
        return;
    }

    nodes.clear();
    edges.clear();
    shortestPath.clear();

    graph = parser->buildGraph();
    if (graph.getNumberVertices() == 0) {
        qDebug() << "Empty graph built from maze";
        return;
    }

    std::vector<std::pair<int, int>> verticesPositions = parser->getVerticesPositions();
    if (verticesPositions.empty()) {
        qDebug() << "No vertex positions in parser";
        return;
    }
    std::vector<std::vector<char>> maze = parser->getMazeConstData();
    int centerX = maze.size()/2;
    int centerY = maze[0].size()/2;
    int nodeId = 0;
    int spacing = 50;


    for (const auto &vertice: verticesPositions) {
         nodes.push_back({QPoint((vertice.second - centerX) * spacing, (vertice.first - centerY) * spacing ), nodeId++});
        //qDebug() << "x" << (vertice.second - centerX) * spacing << " y" << (vertice.first - centerY) * spacing << "node" << nodeId - 1;
    }

    for (size_t from = 0; from < graph.getNumberVertices(); ++from) {
        for (auto &edge: graph.getEdges(from)) {
            size_t to = edge.first;
            if(to <= from)continue;
            int weight = edge.second;
            edges.push_back({static_cast<int>(from), static_cast<int>(to), weight});

        }
    }

    update();
}


void GraphWidget::findShortestPath()
{
    bool ok;
    QStringList items;
    for (const auto& node : nodes) {
        items << QString::number(node.id);
    }

    QString start = QInputDialog::getItem(this, "Select Start Node", "Start Node:", items, 0, false, &ok);
    if (!ok) return;

    QString end = QInputDialog::getItem(this, "Select End Node", "End Node:", items, 0, false, &ok);
    if (!ok) return;

    startNode = start.toInt();
    endNode = end.toInt();
    graph.setStartEnd(startNode, endNode);

    try {

        if (!edges.size()) {
            throw std::runtime_error("Graph has no edges");
        }

        graph.dijkstra();
        shortestPath = graph.getPath();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
        shortestPath.clear();
        return;
    }catch(...){
        QMessageBox::critical(this, "Error", "path not found");
        shortestPath.clear();
        return;
    }

    // Подготовка данных для анимации
    currentAnimationStep = 0;
    animatedPath.clear();
    animatedEdges.clear();

    if (!shortestPath.empty()) {
        // Заполняем вершины и рёбра для анимации
        for (size_t i = 0; i < shortestPath.size(); ++i) {
            animatedPath.push_back(shortestPath[i]);

            if (i > 0) {
                int from = shortestPath[i-1];
                int to = shortestPath[i];
                if (from < (int)nodes.size() && to < (int)nodes.size()) {
                    animatedEdges.emplace_back(nodes[from].pos, nodes[to].pos);
                }
            }
        }

        // Запускаем таймер
        int animationDuration = 1000;
        int interval = animationDuration / (animatedPath.size() + animatedEdges.size());
        animationTimer->start(std::max(10, interval));
    }

    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.translate(width() / 2, (height() - startY)/2 + startY);

    // 1. Рисуем все обычные рёбра (тонкие и серые)
    painter.setPen(QPen(Qt::black, 2));
    for (const Edge &edge : edges) {
        if (edge.from < (int)nodes.size() && edge.to < (int)nodes.size()) {
            QPointF from = nodes[edge.from].pos;
            QPointF to = nodes[edge.to].pos;

            QLineF line(from, to);
            shortenLine(line, 20); // Укорачиваем с обоих концов

            painter.drawLine(line);

            // Вес ребра
            painter.setPen(QPen(Qt::green));
            painter.drawText((from + to) / 2, QString::number(edge.weight));
            painter.setPen(QPen(Qt::black, 2));
        }
    }

    // 2. Рисуем все узлы (неподсвеченные)
    QFont nodeFont = painter.font();
    nodeFont.setPointSize(12);
    nodeFont.setBold(true);
    painter.setFont(nodeFont);

    for (const Node &node : nodes) {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::gray));
        painter.drawEllipse(node.pos, 20, 20);

        painter.setPen(QPen(Qt::white));
        painter.drawText(QRectF(node.pos.x() - 15, node.pos.y() - 10, 30, 20),
                         Qt::AlignCenter, QString::number(node.id));
    }

    // 3. Анимация пути (только видимые элементы)
    if (!shortestPath.empty()) {

        int visibleElements = currentAnimationStep + 1;
        int visibleNodes = (visibleElements + 1) / 2;
        int visibleEdges = visibleElements / 2;


        // Подсвечиваем узлы
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(pathColor));

        for (size_t i = 0; i < (size_t)visibleNodes && i < shortestPath.size(); i++) {
            const Node& node = nodes[shortestPath[i]];
            painter.drawEllipse(node.pos, 20, 20);

            // Текст ID поверх подсветки
            painter.setPen(QPen(Qt::white));
            painter.drawText(QRectF(node.pos.x() - 15, node.pos.y() - 10, 30, 20),
                             Qt::AlignCenter, QString::number(node.id));
            painter.setPen(QPen(Qt::black, 2));
        }

        // Подсвечиваем рёбра
        painter.setPen(QPen(pathColor, 4));
        for (size_t i = 0; i < (size_t)visibleEdges && i+1 < shortestPath.size(); i++) {
            int from = shortestPath[i];
            int to = shortestPath[i+1];

            QLineF line(nodes[from].pos, nodes[to].pos);
            shortenLine(line, 20); // Укорачиваем с обоих концов

            painter.drawLine(line);
        }
    }
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    for (const Node &node : nodes) {
        painter.drawEllipse(node.pos, 20, 20);
    }
}

void GraphWidget::shortenLine(QLineF& line, float offset) {
    line.setLength(line.length() - offset);
    line.setPoints(line.p2(), line.p1()); // Разворачиваем линию
    line.setLength(line.length() - offset);
    line.setPoints(line.p2(), line.p1()); // Возвращаем ориентацию
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos() - QPoint(width() / 2, (height() - startY)/2 + startY);

        if (isAddingEdge) {
            Node *node = findNodeAt(pos);
            if (node && node != firstNode) {
                // Добавление ребра
                QPoint diff = firstNode->pos - node->pos;
                int weight = diff.manhattanLength() / 10;
                bool ok;
                int newWeight = QInputDialog::getInt(this, "Edge Weight",
                                                     "Enter edge weight:",
                                                     weight, 1, 1000, 1, &ok);
                if (ok) {
                    edges.push_back({firstNode->id, node->id, newWeight});
                    graph.addEdge(firstNode->id, node->id, newWeight);
                }
                isAddingEdge = false;
                firstNode = nullptr;
                update();
            }
        } else {
            Node *node = findNodeAt(pos);
            if (!node) {
                // Добавляем новый
                int newId = nodes.empty() ? 0 : nodes.back().id + 1;
                nodes.push_back({pos, newId});
                graph.addVertex();
                update();
            } else {
                // Начало добавления ребра
                isAddingEdge = true;
                firstNode = node;
            }
        }
    } else if (event->button() == Qt::RightButton) {
        // Удаляем вершину или ребро
        QPoint pos = event->pos() - QPoint(width() / 2, (height() - startY)/2 + startY);
        size_t curID;
        Node *node = findNodeAt(pos, curID);
        if (node) {

            edges.erase(std::remove_if(edges.begin(), edges.end(),
                                       [node](const Edge &e) { return e.from == node->id || e.to == node->id; }),
                        edges.end());

            nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                                       [node](const Node &n) { return n.id == node->id; }),
                        nodes.end());
            graph.removeVertex(curID);

            // Переиндексируем оставшиеся вершины
            for (size_t i = 0; i < nodes.size(); ++i) {
                nodes[i].id = i;
            }

            // Обновляем ссылки в рёбрах
            for (auto &edge : edges) {
                if (edge.from > node->id) edge.from--;
                if (edge.to > node->id) edge.to--;
            }
            update();
        }
    }
}


GraphWidget::Node* GraphWidget::findNodeAt(const QPoint &pos, size_t& curID)
{
    for (Node &node : nodes) {
        QPoint diff = node.pos - pos;
        if (diff.manhattanLength() < 15) { // В радиусе узла
            curID = node.id;
            return &node;
        }
    }
    return nullptr;
}

GraphWidget::Node* GraphWidget::findNodeAt(const QPoint &pos)
{
    for (Node &node : nodes) {
        QPoint diff = node.pos - pos;
        if (diff.manhattanLength() < 15) { // В радиусе узла
            return &node;
        }
    }
    return nullptr;
}

void GraphWidget::deleteAllGraph(){
    nodes.clear();
    edges.clear();
    shortestPath.clear();
    update();
}
