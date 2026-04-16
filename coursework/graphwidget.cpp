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
#include <QtGlobal>

GraphWidget::GraphWidget(QWidget *parent, MazeFromFileParser* parser) : QWidget(parent), parser(parser)
{
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        if ((size_t)currentAnimationStep < shortestPath.size() * 2 - 1) {
            currentAnimationStep++;
            update();
        } else {
            animationTimer->stop();
            isAnimating = false;
        }
    });

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout(this);
    updateGraph = new QPushButton("Update graph from maze", this);
    deleteGraph = new QPushButton("Delete the graph", this);
    findPathButton = new QPushButton("Find a path", this);
    pathDisplayButton = new QPushButton("Display the entire path", this);

    connect(findPathButton, &QPushButton::clicked, this, &GraphWidget::findShortestPath);
    connect(deleteGraph, &QPushButton::clicked, this, &GraphWidget::deleteAllGraph);
    connect(updateGraph, &QPushButton::clicked, this, &GraphWidget::updateGraphFromMaze);
    connect(pathDisplayButton, &QPushButton::clicked, this, &GraphWidget::pathDisplay);
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
    buttonLayout->addWidget(pathDisplayButton);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
    startY = updateGraph->height();

    scaleFactor = 1.0;
    scrollOffset = QPointF(0, 0);
}

void GraphWidget::pathDisplay(){
    if(isAnimating){
        currentAnimationStep = shortestPath.size() * 2 - 1;
        update();
    }
}

void GraphWidget::updateGraphFromMaze() {


    if (!parser) {
        qDebug() << "Parser is null";
        return;
    }

    scaleFactor = 1.0;
    scrollOffset = QPointF(0, 0);
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
    int centerY = maze.size()/2;
    int centerX = maze[0].size()/2;
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
    scaleFactor = 1;

    update();
}


void GraphWidget::findShortestPath()
{
    bool ok;
    QStringList items;
    for (const auto& node : nodes) {
        items << QString::number(node.id);
    }

    int startNode = QInputDialog::getInt(this, "Select Start Node", "Start Node:", 0, 0, nodes.size() - 1, 1, &ok);
    if (!ok) return;

    int endNode = QInputDialog::getInt(this, "Select End Node", "End Node:", 0, 0, nodes.size() - 1, 1, &ok);
    if (!ok) return;

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
        isAnimating = true;
        animationTimer->start(0);
    }

    update();
}

void GraphWidget::shortenLine(QLineF& line, float offset) {
    line.setLength(line.length() - offset);
    line.setPoints(line.p2(), line.p1()); // Разворачиваем линию
    line.setLength(line.length() - offset);
    line.setPoints(line.p2(), line.p1()); // Возвращаем ориентацию
}

QSize GraphWidget::sizeHint() const
{
    if (nodes.empty()) {
        return QSize(800, 600);
    }

    // Находим границы графа
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    if (!nodes.empty()) {
        minX = maxX = nodes[0].pos.x();
        minY = maxY = nodes[0].pos.y();
        for (const auto& node : nodes) {
            minX = qMin(minX, node.pos.x());
            maxX = qMax(maxX, node.pos.x());
            minY = qMin(minY, node.pos.y());
            maxY = qMax(maxY, node.pos.y());
        }
    }

    // Добавляем отступы для узлов
    int padding = baseNodeSize * 2;
    int graphWidth = (maxX - minX + padding); // Убрано умножение на scaleFactor
    int graphHeight = (maxY - minY + padding); // Убрано умножение на scaleFactor

    return QSize(graphWidth, graphHeight + startY);
}

// Добавьте эти методы в класс GraphWidget:

void GraphWidget::initializeMinMaxScale()
{
    if (nodes.empty()) {
        minScale = 0.1;
        maxScale = 10.0;
        return;
    }

    QRectF graphBounds;
    getGraphBounds(graphBounds);

    qreal graphWidth = qMax(1.0, graphBounds.width());
    qreal graphHeight = qMax(1.0, graphBounds.height());
    qreal viewWidth = qMax(1.0, (qreal)width());
    qreal viewHeight = qMax(1.0, (qreal)(height() - startY));

    // Минимальный масштаб - чтобы весь граф помещался в окно
    qreal minWidthScale = viewWidth * 0.8 / graphWidth;
    qreal minHeightScale = viewHeight * 0.8 / graphHeight;
    minScale = qMax(0.05, qMin(minWidthScale, minHeightScale));

    // Максимальный масштаб - чтобы узлы не становились слишком большими
    qreal maxNodeScale = 200.0 / qMax(1.0, (qreal)baseNodeSize);
    maxScale = qMax(minScale * 2.0, qMin(maxNodeScale, 15.0)); // Увеличено с 1.1 до 2.0

    // Дополнительные проверки на случай ошибок округления
    if (maxScale <= minScale) {
        qDebug() << "Warning: maxScale <= minScale detected. minScale:" << minScale << "maxScale:" << maxScale;
        maxScale = minScale * 2.0;
    }

    // Финальная проверка корректности значений
    if (minScale <= 0 || maxScale <= 0 || minScale >= maxScale) {
        qDebug() << "Warning: Invalid scale values detected. Resetting to defaults.";
        minScale = 0.1;
        maxScale = 10.0;
    }
}

void GraphWidget::wheelEvent(QWheelEvent* event)
{
    if (nodes.empty()) {
        event->ignore();
        return;
    }

    if (event->modifiers() & Qt::ControlModifier) {
        initializeMinMaxScale();

        // Проверяем корректность масштабов перед использованием
        if (minScale >= maxScale || minScale <= 0 || maxScale <= 0) {
            minScale = 0.1;
            maxScale = 10.0;
        }

        QPointF mousePos = event->position();
        QPointF graphPosBefore = mousePos - QPointF(width()/2, (height()+startY)/2) - scrollOffset;

        double zoomFactor = pow(1.001, event->angleDelta().y());
        double newScale = scaleFactor * zoomFactor;

        // Применяем ограничения масштаба
        newScale = qBound(minScale, newScale, maxScale);

        // Корректируем смещение для сохранения позиции курсора
        if (scaleFactor > 0.0) {
            scrollOffset = (scrollOffset + graphPosBefore) * (newScale / scaleFactor) - graphPosBefore;
        }
        scaleFactor = newScale;

        limitScrollOffset();
        update();
        event->accept();
    } else {
        // Обычная прокрутка
        const qreal scrollSpeed = 0.5;
        scrollOffset += QPointF(event->angleDelta().x(), event->angleDelta().y()) * scrollSpeed;
        limitScrollOffset();
        update();
        event->accept();
    }
}

void GraphWidget::getGraphBounds(QRectF& bounds) const
{
    if (nodes.empty()) {
        bounds = QRectF(0, 0, 0, 0);
        return;
    }

    // ИСПРАВЛЕНИЕ: Добавляем проверки для minNodeSize и maxNodeSize
    int safeMinNodeSize = qMin(minNodeSize, maxNodeSize);
    int safeMaxNodeSize = qMax(minNodeSize, maxNodeSize);

    // Дополнительная проверка на корректность значений
    if (safeMinNodeSize <= 0 || safeMaxNodeSize <= 0 || safeMinNodeSize > safeMaxNodeSize) {
        qDebug() << "Warning: Invalid node sizes. minNodeSize:" << minNodeSize << "maxNodeSize:" << maxNodeSize;
        safeMinNodeSize = 10;  // Значения по умолчанию
        safeMaxNodeSize = 50;
    }

    qreal nodeRadius = qBound(safeMinNodeSize, baseNodeSize, safeMaxNodeSize) / 2.0;

    // Начальные значения с учетом размера узла
    qreal minX = nodes[0].pos.x() - nodeRadius;
    qreal maxX = nodes[0].pos.x() + nodeRadius;
    qreal minY = nodes[0].pos.y() - nodeRadius;
    qreal maxY = nodes[0].pos.y() + nodeRadius;

    for (const auto& node : nodes) {
        minX = qMin(minX, node.pos.x() - nodeRadius);
        maxX = qMax(maxX, node.pos.x() + nodeRadius);
        minY = qMin(minY, node.pos.y() - nodeRadius);
        maxY = qMax(maxY, node.pos.y() + nodeRadius);
    }

    // Добавляем дополнительный отступ
    const qreal extraPadding = 20.0;
    bounds = QRectF(minX - extraPadding, minY - extraPadding,
                    (maxX - minX) + 2 * extraPadding,
                    (maxY - minY) + 2 * extraPadding);
}

// Исправленный метод limitScrollOffset
void GraphWidget::limitScrollOffset()
{
    if (nodes.empty()) {
        scrollOffset = QPointF(0, 0);
        return;
    }

    QRectF graphBounds;
    getGraphBounds(graphBounds);

    qreal scaledGraphWidth = graphBounds.width() * scaleFactor;
    qreal scaledGraphHeight = graphBounds.height() * scaleFactor;
    qreal viewWidth = width();
    qreal viewHeight = height() - startY;

    // Если граф меньше области просмотра - центрируем
    if (scaledGraphWidth <= viewWidth && scaledGraphHeight <= viewHeight) {
        scrollOffset = QPointF(0, 0);
        return;
    }

    // Максимальные смещения
    qreal maxXOffset = (scaledGraphWidth - viewWidth) / 2.0;
    qreal maxYOffset = (scaledGraphHeight - viewHeight) / 2.0;

    const qreal extraScrollMargin = 20.0 * scaleFactor;
    maxXOffset += extraScrollMargin;
    maxYOffset += extraScrollMargin;

    // ИСПРАВЛЕНИЕ: Добавляем проверки перед qBound
    if (maxXOffset < 0) maxXOffset = 0;
    if (maxYOffset < 0) maxYOffset = 0;

    // Ограничиваем смещения
    scrollOffset.setX(qBound(-maxXOffset, scrollOffset.x(), maxXOffset));
    scrollOffset.setY(qBound(-maxYOffset, scrollOffset.y(), maxYOffset));
}

// Исправленный метод paintEvent
void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    // Сохраняем исходное состояние
    painter.save();

    // Применяем трансформации
    painter.translate(width() / 2.0, (height() + startY) / 2.0);
    painter.translate(scrollOffset);
    painter.scale(scaleFactor, scaleFactor);

    // ИСПРАВЛЕНИЕ: Более надежная проверка размеров узлов
    int safeMinNodeSize = 5;   // Минимум по умолчанию
    int safeMaxNodeSize = 100; // Максимум по умолчанию

    // Проверяем, инициализированы ли переменные класса
    if (minNodeSize > 0 && maxNodeSize > 0 && minNodeSize <= maxNodeSize) {
        safeMinNodeSize = minNodeSize;
        safeMaxNodeSize = maxNodeSize;
    } else {
        qDebug() << "Warning: Using default node sizes. minNodeSize:" << minNodeSize << "maxNodeSize:" << maxNodeSize;
    }

    // Проверяем baseNodeSize
    int safeBaseNodeSize = baseNodeSize;
    if (safeBaseNodeSize <= 0) {
        safeBaseNodeSize = 20; // Значение по умолчанию
    }

    int nodeSize = qBound(safeMinNodeSize, safeBaseNodeSize, safeMaxNodeSize);

    // Рисуем все обычные рёбра
    painter.setPen(QPen(Qt::black, 2));
    for (const Edge &edge : edges) {
        if (edge.from < (int)nodes.size() && edge.to < (int)nodes.size()) {
            QPointF from = nodes[edge.from].pos;
            QPointF to = nodes[edge.to].pos;

            QLineF line(from, to);
            shortenLine(line, nodeSize / 2.0);

            painter.drawLine(line);

            // Вес ребра
            painter.setPen(QPen(Qt::green, 1));
            QFont weightFont = painter.font();
            weightFont.setPixelSize(qMax(8, nodeSize / 3));
            painter.setFont(weightFont);
            painter.drawText((from + to) / 2, QString::number(edge.weight));
            painter.setPen(QPen(Qt::black, 2));
        }
    }

    // Рисуем все узлы
    QFont nodeFont = painter.font();
    nodeFont.setPixelSize(qMax(8, (int)(nodeSize * 0.6)));
    painter.setFont(nodeFont);

    for (const Node &node : nodes) {
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(QBrush(Qt::lightGray));
        painter.drawEllipse(node.pos, nodeSize, nodeSize);

        painter.setPen(QPen(Qt::black));
        QRectF textRect(node.pos.x() - nodeSize/2, node.pos.y() - nodeSize/2,
                        nodeSize, nodeSize);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(node.id));
    }

    // Анимация пути
    if (!shortestPath.empty()) {
        int visibleElements = currentAnimationStep + 1;
        int visibleNodes = (visibleElements + 1) / 2;
        int visibleEdges = visibleElements / 2;

        // Подсвечиваем узлы пути
        painter.setPen(QPen(Qt::black, 3));
        painter.setBrush(QBrush(pathColor));

        for (size_t i = 0; i < (size_t)visibleNodes && i < shortestPath.size(); i++) {
            if (shortestPath[i] < (int)nodes.size()) {
                const Node& node = nodes[shortestPath[i]];
                painter.drawEllipse(node.pos, nodeSize, nodeSize);

                painter.setPen(QPen(Qt::white));
                QRectF textRect(node.pos.x() - nodeSize/2, node.pos.y() - nodeSize/2,
                                nodeSize, nodeSize);
                painter.drawText(textRect, Qt::AlignCenter, QString::number(node.id));
                painter.setPen(QPen(Qt::black, 3));
            }
        }

        // Подсвечиваем рёбра пути
        painter.setPen(QPen(pathColor, 4));
        for (size_t i = 0; i < (size_t)visibleEdges && i+1 < shortestPath.size(); i++) {
            int from = shortestPath[i];
            int to = shortestPath[i+1];

            if (from < (int)nodes.size() && to < (int)nodes.size()) {
                QLineF line(nodes[from].pos, nodes[to].pos);
                shortenLine(line, nodeSize / 2.0);
                painter.drawLine(line);
            }
        }
    }

    painter.restore();
}

// Исправленные методы findNodeAt
GraphWidget::Node* GraphWidget::findNodeAt(const QPoint &pos, size_t& curID)
{
    QPointF transformedPos(pos.x() / scaleFactor - width() / 2.0 / scaleFactor,
                           (pos.y() - startY) / scaleFactor - (height() - startY) / 2.0 / scaleFactor);

    // ИСПРАВЛЕНИЕ: Безопасное вычисление размера узла
    int safeMinNodeSize = 5;
    int safeMaxNodeSize = 100;
    int safeBaseNodeSize = 20;

    if (minNodeSize > 0 && maxNodeSize > 0 && minNodeSize <= maxNodeSize) {
        safeMinNodeSize = minNodeSize;
        safeMaxNodeSize = maxNodeSize;
    }

    if (baseNodeSize > 0) {
        safeBaseNodeSize = baseNodeSize;
    }

    int nodeSize = qBound(safeMinNodeSize, safeBaseNodeSize, safeMaxNodeSize);

    for (Node &node : nodes) {
        if (QLineF(transformedPos, node.pos).length() <= nodeSize) {
            curID = node.id;
            return &node;
        }
    }
    return nullptr;
}

GraphWidget::Node* GraphWidget::findNodeAt(const QPoint &pos)
{
    QPointF transformedPos(pos.x() / scaleFactor - width() / 2.0 / scaleFactor,
                           (pos.y() - startY) / scaleFactor - (height() - startY) / 2.0 / scaleFactor);

    // ИСПРАВЛЕНИЕ: Безопасное вычисление размера узла
    int safeMinNodeSize = 5;
    int safeMaxNodeSize = 100;
    int safeBaseNodeSize = 20;

    if (minNodeSize > 0 && maxNodeSize > 0 && minNodeSize <= maxNodeSize) {
        safeMinNodeSize = minNodeSize;
        safeMaxNodeSize = maxNodeSize;
    }

    if (baseNodeSize > 0) {
        safeBaseNodeSize = baseNodeSize;
    }

    int nodeSize = qBound(safeMinNodeSize, safeBaseNodeSize, safeMaxNodeSize);

    for (Node &node : nodes) {
        if (QLineF(transformedPos, node.pos).length() <= nodeSize) {
            return &node;
        }
    }
    return nullptr;
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

void GraphWidget::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        size_t curId;
        QPoint transformedPos(event->pos().x() / scaleFactor - width() / 2.0 / scaleFactor,
                               (event->pos().y() - startY) / scaleFactor - (height() - startY) / 2.0 / scaleFactor);

        Node* node = findNodeAt(event->pos(), curId);
        if (node) {
            isAddingEdge = false;
            node->pos = transformedPos;
            update();
        }
    }
}

void GraphWidget::setParser(MazeFromFileParser* newParser) {

    animationTimer->stop();

    parser = newParser;

    nodes.clear();
    edges.clear();
    shortestPath.clear();

    update();
}

void GraphWidget::deleteAllGraph(){
    nodes.clear();
    edges.clear();
    shortestPath.clear();
    scaleFactor = 1.0;
    scrollOffset = QPointF(0, 0);
    update();
}
