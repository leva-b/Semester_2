#include "graphwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <cmath>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);

    findPathButton = new QPushButton("Find Path", this);
    connect(findPathButton, &QPushButton::clicked, this, &GraphWidget::findShortestPath);
    // findPathButton->setStyleSheet("QPushButton {"
    //                               "background-color: #4CAF50;"
    //                               "color: white;"
    //                               "border: none;"
    //                               "padding: 8px;"
    //                               "font-size: 14px;"
    //                               "}");

    mainLayout->addWidget(findPathButton);
    mainLayout->addStretch(); // Растягиваем оставшееся пространство

    setLayout(mainLayout);
    // Инициализация с более разнесенными вершинами
    nodes.push_back({QPoint(150, 150), 0});
    nodes.push_back({QPoint(350, 150), 1});
    nodes.push_back({QPoint(250, 300), 2});

    edges.push_back({0, 1, 5});
    edges.push_back({1, 2, 3});
}

void GraphWidget::updateGraphFromMaze(const std::vector<std::vector<int>> &maze)
{
    nodes.clear();
    edges.clear();

    // Увеличиваем расстояние между вершинами при конвертации из лабиринта
    int nodeId = 0;
    int spacing = 60; // Увеличенное расстояние между узлами

    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            if (maze[y][x] == 0) { // Path
                // Добавляем смещение от краев и увеличиваем расстояние
                nodes.push_back({QPoint(x * spacing + 100, y * spacing + 100), nodeId++});
            }
        }
    }

    // Создаем ребра только между достаточно близкими вершинами
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            QPoint diff = nodes[i].pos - nodes[j].pos;
            double distance = sqrt(diff.x() * diff.x() + diff.y() * diff.y());
            if (distance < spacing * 1.8) { // Увеличенный порог
                edges.push_back({static_cast<int>(i), static_cast<int>(j), static_cast<int>(distance/10)});
            }
        }
    }

    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // Рисуем рёбра
    painter.setPen(QPen(QColor("#3498db"), 3, Qt::SolidLine, Qt::RoundCap));
    for (const Edge &edge : edges) {
        if (edge.from < (int)nodes.size() && edge.to < (int)nodes.size()) {
            QPointF from = nodes[edge.from].pos;
            QPointF to = nodes[edge.to].pos;
            painter.drawLine(from, to);

            // Вес ребра
            QPointF mid = (from + to) / 2;
            painter.setPen(QPen(Qt::black));
            QFont font = painter.font();
            font.setPointSize(10);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(mid, QString::number(edge.weight));
        }
    }

    // Рисуем вершины
    QFont nodeFont = painter.font();
    nodeFont.setPointSize(12);
    nodeFont.setBold(true);
    painter.setFont(nodeFont);

    for (const Node &node : nodes) {
        // Внешний круг
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::gray));
        painter.drawEllipse(node.pos, 20, 20);

        // Текст ID
        painter.setPen(QPen(Qt::white));
        QRectF textRect(node.pos.x() - 15, node.pos.y() - 10, 30, 20);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(node.id));
    }

    // Рисуем путь (если есть)
    if (!shortestPath.empty()) {
        painter.setPen(QPen(QColor("#2ecc71"), 4, Qt::SolidLine, Qt::RoundCap));
        for (size_t i = 1; i < shortestPath.size(); ++i) {
            if (shortestPath[i-1] < (int)nodes.size() && shortestPath[i] < (int)nodes.size()) {
                painter.drawLine(nodes[shortestPath[i-1]].pos, nodes[shortestPath[i]].pos);
            }
        }
    }
}


void GraphWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    findPathButton = new QPushButton("Find Shortest Path", this);
    mainLayout->addWidget(findPathButton);
    mainLayout->addStretch();

    connect(findPathButton, &QPushButton::clicked, this, &GraphWidget::findShortestPath);
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

    // Здесь будет алгоритм Дейкстры
    // Симуляция пути
    shortestPath = {startNode, (int)((startNode + 1) % nodes.size()), endNode};
    update();
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();

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
                update();
            } else {
                // Начало добавления ребра
                isAddingEdge = true;
                firstNode = node;
            }
        }
    } else if (event->button() == Qt::RightButton) {
        // Удаляем вершину или ребро
        Node *node = findNodeAt(event->pos());
        if (node) {
            // Сначала удаляем соединенные края
            edges.erase(std::remove_if(edges.begin(), edges.end(),
                                       [node](const Edge &e) { return e.from == node->id || e.to == node->id; }),
                        edges.end());
            // Удаление вершины
            nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
                                       [node](const Node &n) { return n.id == node->id; }),
                        nodes.end());
            update();
        }
    }
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
