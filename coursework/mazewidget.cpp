#include "mazewidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <algorithm>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent)
{

    maze = {
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1}
    };
}

void MazeWidget::loadMazeFromFile(const QString &filePath)
{
    maze.clear();
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        std::vector<int> row;
        for (QChar ch : line) {
            row.push_back(ch == '1' ? 1 : 0);
        }
        maze.push_back(row);
    }
    file.close();
    update();
}

std::vector<std::vector<int>> MazeWidget::getMazeData() const
{
    return maze;
}

void MazeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рассчитываем размер ячейки на основе размеров виджета и размера лабиринта
    if (!maze.empty()) {
        cellSize = std::min(width() / maze[0].size(), height() / maze.size());
    }

    // Draw maze
    for (size_t y = 0; y < maze.size(); ++y) {
        for (size_t x = 0; x < maze[y].size(); ++x) {
            QRect cellRect(x * cellSize, y * cellSize, cellSize, cellSize);
            if (maze[y][x] == 1) {
                painter.fillRect(cellRect, Qt::black); // Wall
            } else {
                painter.fillRect(cellRect, Qt::white); // Path
            }
            painter.drawRect(cellRect);
        }
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        toggleWall(pos);
    }
}

void MazeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = event->pos();
        toggleWall(pos);
    }
}

void MazeWidget::toggleWall(const QPoint &pos)
{
    int x = pos.x() / cellSize;
    int y = pos.y() / cellSize;

    if (x >= 0 && y >= 0 && y < static_cast<int>(maze.size()) && x < static_cast<int>(maze[y].size())) {
        maze[y][x] = maze[y][x] == 1 ? 0 : 1;
        update();
    }
}

void MazeWidget::drawPath(QPainter &painter, const std::vector<QPoint> &path)
{
    if (path.empty()) return;

    painter.setPen(QPen(Qt::red, 2));
    for (size_t i = 1; i < path.size(); ++i) {
        QPoint prev = path[i-1];
        QPoint current = path[i];
        painter.drawLine(prev.x() * cellSize + cellSize/2,
                         prev.y() * cellSize + cellSize/2,
                         current.x() * cellSize + cellSize/2,
                         current.y() * cellSize + cellSize/2);
    }
}
