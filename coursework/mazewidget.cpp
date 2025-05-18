#include "mazewidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <algorithm>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

MazeWidget::MazeWidget(QWidget *parent, MazeFromFileParser* parser) : QWidget(parent), parser(parser), pathColor(Qt::red)
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Создаем горизонтальный layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    openFileButton = new QPushButton("Open file with maze");
    findPathButton = new QPushButton("Find a way");

    buttonLayout->addWidget(openFileButton);
    buttonLayout->addWidget(findPathButton);

    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, [this]() {
        if (currentPathIndex < totalPathCells) {
            currentPathIndex++;
            update();
        } else {
            animationTimer->stop();
        }
    });
    connect(openFileButton, &QPushButton::clicked, this, &MazeWidget::loadMazeFromFile);
    connect(findPathButton, &QPushButton::clicked, this, &MazeWidget::findWayThroughMaze);
}

void MazeWidget::loadMazeFromFile()
{
    pathCells.clear();
    currentPathIndex = 0;
    animationTimer->stop();
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        try{
            delete parser;
            qDebug() << filePath.toStdString();
            parser = new MazeFromFileParser(filePath.toStdString(), wall_symbol, path_symbol);
            update();
        }catch(const std::runtime_error& e){
            QMessageBox::warning(nullptr, "Error", "Cannot open file for reading:");
        }
    } else {
        QMessageBox::warning(this, "Invalid File", "Please select a valid .txt file.");
    }
}

void MazeWidget::findWayThroughMaze() {
    try {
        pathCells.clear();
        currentPathIndex = 0;
        animationTimer->stop();
        if(!isCorrectMaze()){
            QMessageBox::information(this, "Error", "The maze must have no more than two entry/exit points");
            return;
        }
        Graph graph = parser->buildGraph();
        graph.dijkstra();
        auto path = graph.getPath();

        if (!path.empty()) {
            const auto& vertices = parser->getVerticesPositions();

            // Заполняем pathCells только вершинами пути
            for (size_t i = 0; i < path.size(); ++i) {
                pathCells.emplace_back(vertices[path[i]].second, vertices[path[i]].first);
            }

            // Вычисляем общее количество клеток для анимации
            totalPathCells = 0;
            for (size_t i = 0; i < pathCells.size() - 1; ++i) {
                totalPathCells += std::max(std::abs(pathCells[i].x() - pathCells[i+1].x()),
                                           std::abs(pathCells[i].y() - pathCells[i+1].y())) + 1;
            }
            animationTimer->start(std::max(1000/totalPathCells,5));
        }
    } catch (const char* e) {
        QMessageBox::warning(this, "Error", "Path not found");
    }
}

bool MazeWidget::isCorrectMaze(){
    int numberOfPath = 0;
    for(size_t i = 0; i < getMazeConstData().size(); i++){
        if(getMazeConstData()[i][0] == path_symbol)numberOfPath++;
        if(getMazeConstData()[i][getMazeConstData()[i].size() - 1] == path_symbol)numberOfPath++;
    }
    for(size_t i = 0; i < getMazeConstData()[0].size(); i++){
        if(getMazeConstData()[0][i] == path_symbol)
            numberOfPath++;
        if(getMazeConstData()[getMazeConstData().size() - 1][i] == path_symbol)
            numberOfPath++;
    }
    return numberOfPath == 2;
}

std::vector<std::vector<char>>& MazeWidget::getMazeData()
{
    return parser->getMazeData();
}

const std::vector<std::vector<char>>& MazeWidget::getMazeConstData() const
{
    return parser->getMazeConstData();
}

void MazeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(scaleFactor, scaleFactor);

    // Рассчитываем размер ячейки на основе размеров виджета и размера лабиринта
    if (!parser->getMazeData().empty()) {
        int availableHeight = height() - openFileButton->height() - 10;
        cellSize = std::min(width() / getMazeConstData()[0].size(),
                            availableHeight / getMazeConstData().size());
    }


    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int startX = (width() - mazeWidth) / 2;
    int startY = openFileButton->height() + 10;

    for (size_t y = 0; y < getMazeConstData().size(); ++y) {
        for (size_t x = 0; x < getMazeConstData()[y].size(); ++x) {
            QRect cellRect(startX + x * cellSize, y * cellSize + startY, cellSize, cellSize);

            if (getMazeConstData()[y][x] == '1') {
                painter.fillRect(cellRect, Qt::black); // Стена
            } else {
                painter.fillRect(cellRect, Qt::white); // Путь
            }
            painter.drawRect(cellRect);
        }
    }

    if (!pathCells.empty() && currentPathIndex > 0) {
        drawPath(startX, startY);
    }
}



void MazeWidget::drawPath(int startX, int startY){
    QPainter painter(this);
    painter.setBrush(pathColor);
    painter.setPen(Qt::black);

    int cellsDrawn = 0;
    for (size_t i = 0; i < pathCells.size() - 1; ++i) {
        const QPoint& start = pathCells[i];
        const QPoint& end = pathCells[i+1];

        int dx = end.x() - start.x();
        int dy = end.y() - start.y();
        int steps = std::max(std::abs(dx), std::abs(dy));

        // Количество клеток в этом сегменте
        int segmentCells = steps + 1;

        // Сколько нужно отрисовать в этом сегменте
        int toDraw = std::min(segmentCells, currentPathIndex - cellsDrawn);

        for (int j = 0; j < toDraw; ++j) {
            double progress = (steps > 0) ? static_cast<double>(j) / steps : 0;
            int x = start.x() + std::round(dx * progress);
            int y = start.y() + std::round(dy * progress);

            QRect cellRect(startX + x * cellSize, startY + y * cellSize, cellSize, cellSize);
            painter.fillRect(cellRect, pathColor);
            painter.drawRect(cellRect);
        }

        cellsDrawn += toDraw;
        if (cellsDrawn >= currentPathIndex) break;
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    if(parser->getMazeData().empty() || event->button() != Qt::LeftButton)
        return;

    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int mazeHeight = getMazeConstData().size() * cellSize;
    int startX = (width() - mazeWidth) / 2;
    QPoint pos = event->pos();
    int startY = openFileButton->height() + 10;

    // Проверяем, что клик был в области лабиринта
    if (pos.y() >= startY && pos.y() < startY + mazeHeight &&
        pos.x() >= startX && pos.x() < startX + mazeWidth) {
        int x = (pos.x() - startX) / cellSize;
        int y = (pos.y() - startY) / cellSize;

        if (x >= 0 && y >= 0 &&
            y < static_cast<int>(getMazeConstData().size()) &&
            x < static_cast<int>(getMazeConstData()[y].size())) {
            toggleWall(QPoint(x, y));
        }
    }
}

void MazeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(parser->getMazeData().empty() || !(event->buttons() & Qt::LeftButton))
        return;

    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int mazeHeight = getMazeConstData().size() * cellSize;
    int startX = (width() - mazeWidth) / 2;
    int startY = openFileButton->height();


    QPoint pos = event->pos();
    if (pos.y() >= startY && pos.y() < startY + mazeHeight &&
        pos.x() >= startX && pos.x() < startX + mazeWidth) {
        int x = (pos.x() - startX) / cellSize;
        int y = (pos.y() - startY) / cellSize;

        if (x != lastCellPos.x() || y != lastCellPos.y()) {
            lastCellPos = QPoint(x, y);

            if (x >= 0 && y >= 0 &&
                y < static_cast<int>(getMazeConstData().size()) &&
                x < static_cast<int>(getMazeConstData()[y].size())) {
                toggleWall(QPoint(x, y));
            }
        }
    }else{
        lastCellPos = QPoint(-1, -1);
    }
}

void MazeWidget::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    lastCellPos = QPoint(-1, -1); // Сброс при отпускании кнопки
}

void MazeWidget::toggleWall(const QPoint &pos)
{
    pathCells.clear();
    currentPathIndex = 0;
    animationTimer->stop();
    int x = pos.x();
    int y = pos.y();

    if (x >= 0 && y >= 0 && y < static_cast<int>(getMazeConstData().size()) && x < static_cast<int>(getMazeConstData()[y].size())) {
        getMazeData()[y][x] = getMazeConstData()[y][x] == '1' ? '0' : '1';
        update();
    }
}

void MazeWidget::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        const double zoomFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            scaleFactor *= zoomFactor;
        } else {
            scaleFactor /= zoomFactor;
        }
        scaleFactor = qBound(0.1, scaleFactor, 10.0);
        update();
    } else {
        QWidget::wheelEvent(event);
    }
}

QSize MazeWidget::sizeHint() const
{
    if (parser->getMazeData().empty()) {
        return QSize(800, 600);
    }

    return QSize(parser->getMazeData()[0].size() * cellSize * scaleFactor,
                 parser->getMazeData().size() * cellSize * scaleFactor + openFileButton->height() + 10);
}

