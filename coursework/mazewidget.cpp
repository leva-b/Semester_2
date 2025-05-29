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
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Создаем горизонтальный layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    openFileButton = new QPushButton("Open file with maze");
    findPathButton = new QPushButton("Find a way");
    pathDisplayButton = new QPushButton("Display the entire path", this);

    buttonLayout->addWidget(openFileButton);
    buttonLayout->addWidget(findPathButton);
    buttonLayout->addWidget(pathDisplayButton);
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
    connect(pathDisplayButton, &QPushButton::clicked, this, &MazeWidget::pathDisplay);
}

void MazeWidget::pathDisplay(){
    if(pathCells.empty())
        findWayThroughMaze();
    currentPathIndex = totalPathCells;
    update();
}

void MazeWidget::loadMazeFromFile()
{

    pathCells.clear();
    currentPathIndex = 0;
    animationTimer->stop();
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        try{
            qDebug() << filePath.toStdString();
            parser = new MazeFromFileParser(filePath.toStdString(), wall_symbol, path_symbol);
            emit parserUpdated(parser);
            scaleFactor = 1;
            scrollOffset = QPointF(0, 0);
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
            totalPathCells = pathCells.size() - 1;
            animationTimer->start(0);
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

    if (parser->getMazeData().empty()) return;

    // Рассчитываем размеры лабиринта
    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int mazeHeight = getMazeConstData().size() * cellSize;


    // Автоматически центрируем, если лабиринт меньше виджета
    if (mazeWidth * scaleFactor <= width() &&
        mazeHeight * scaleFactor <= height()) {
        scrollOffset = QPointF(0, 0);
    }

    // Применяем трансформации
    painter.translate(width()/2, height()/2);
    painter.translate(scrollOffset);
    painter.scale(scaleFactor, scaleFactor);
    if (!parser->getMazeData().empty()) {
        int availableHeight = height() - openFileButton->height() - 10;
        cellSize = std::min(width() / getMazeConstData()[0].size(),
                            availableHeight / getMazeConstData().size());
    }

    // Начальные координаты (левый верхний угол лабиринта)
    int startX = -mazeWidth / 2;
    int startY = -mazeHeight / 2 + (openFileButton->height() - 10) / scaleFactor;

    // Рисуем лабиринт
    for (size_t y = 0; y < getMazeConstData().size(); ++y) {
        for (size_t x = 0; x < getMazeConstData()[y].size(); ++x) {
            QRect cellRect(startX + x * cellSize, startY + y * cellSize, cellSize, cellSize);

            if (getMazeConstData()[y][x] == '1') {
                painter.fillRect(cellRect, Qt::black);
            } else {
                painter.fillRect(cellRect, Qt::white);
            }
            painter.drawRect(cellRect);
        }
    }

    if (!pathCells.empty() && currentPathIndex > 0) {
        drawPath(painter, startX, startY);
    }
}


void MazeWidget::drawPath(QPainter& painter, int startX, int startY) {
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(pathColor);
    painter.setPen(Qt::black);

    // Отрисовываем все сегменты до currentPathIndex
    for (int i = 0; i < currentPathIndex && i < pathCells.size() - 1; ++i) {
        const QPoint& start = pathCells[i];
        const QPoint& end = pathCells[i + 1];

        // Отрисовываем линию между узлами
        int dx = end.x() - start.x();
        int dy = end.y() - start.y();
        int steps = std::max(std::abs(dx), std::abs(dy));

        for (int j = 0; j <= steps; ++j) {
            float progress = static_cast<float>(j) / steps;
            int x = start.x() + std::round(dx * progress);
            int y = start.y() + std::round(dy * progress);

            if (x >= 0 && y >= 0 && y < static_cast<int>(getMazeConstData().size()) &&
                x < static_cast<int>(getMazeConstData()[y].size())) {
                QRect cellRect(startX + x * cellSize, startY + y * cellSize, cellSize, cellSize);
                painter.fillRect(cellRect, pathColor);
            }
        }
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    if(parser->getMazeData().empty() || event->button() != Qt::LeftButton)
        return;

    // Преобразуем координаты с учетом трансформаций
    QPointF scenePos = (event->pos() - QPoint(width()/2, height()/2) - scrollOffset);
    scenePos /= scaleFactor;

    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int mazeHeight = getMazeConstData().size() * cellSize;

    int startX = -mazeWidth / 2;
    int startY = -mazeHeight / 2 + (openFileButton->height() - 10) / scaleFactor;

    if (scenePos.y() >= startY && scenePos.y() < startY + mazeHeight &&
        scenePos.x() >= startX && scenePos.x() < startX + mazeWidth) {
        int x = (scenePos.x() - startX) / cellSize;
        int y = (scenePos.y() - startY) / cellSize;

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

    // Аналогичное преобразование координат
    QPointF scenePos = (event->pos() - QPoint(width()/2, height()/2) - scrollOffset) / scaleFactor;

    int mazeWidth = getMazeConstData()[0].size() * cellSize;
    int mazeHeight = getMazeConstData().size() * cellSize;

    int startX = -mazeWidth / 2;
    int startY = -mazeHeight / 2 + (openFileButton->height() + 10) / scaleFactor;

    if (scenePos.y() >= startY && scenePos.y() < startY + mazeHeight &&
        scenePos.x() >= startX && scenePos.x() < startX + mazeWidth) {
        int x = (scenePos.x() - startX) / cellSize;
        int y = (scenePos.y() - startY) / cellSize;

        if (x != lastCellPos.x() || y != lastCellPos.y()) {
            lastCellPos = QPoint(x, y);

            if (x >= 0 && y >= 0 &&
                y < static_cast<int>(getMazeConstData().size()) &&
                x < static_cast<int>(getMazeConstData()[y].size())) {
                toggleWall(QPoint(x, y));
            }
        }
    } else {
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

void MazeWidget::initializedMinMaxScale()
{
    if (parser->getMazeData().empty() || cellSize <= 0) {
        minScale = 0.1;
        maxScale = 10.0;
        return;
    }

    // Размеры виджета
    qreal widgetWidth = qMax(1.0, static_cast<qreal>(width()));
    qreal widgetHeight = qMax(1.0, static_cast<qreal>(height() - openFileButton->height() - 10));

    // Размеры лабиринта в пикселях без масштабирования
    qreal mazeWidth = qMax(1.0, static_cast<qreal>(getMazeConstData()[0].size() * cellSize));
    qreal mazeHeight = qMax(1.0, static_cast<qreal>(getMazeConstData().size() * cellSize));

    // Минимальный масштаб - чтобы лабиринт не был меньше половины виджета
    qreal minWidthScale = (widgetWidth / 2.0) / mazeWidth;
    qreal minHeightScale = (widgetHeight / 2.0) / mazeHeight;
    qreal calculatedMinScale = qMin(minWidthScale, minHeightScale);

    // Максимальный масштаб - чтобы размер одной ячейки не превышал 100px
    qreal maxCellScale = 100.0 / static_cast<qreal>(cellSize);

    // Устанавливаем безопасные границы
    minScale = qMax(0.05, calculatedMinScale);
    maxScale = qMax(1.0, maxCellScale);

    // Критически важно: убеждаемся что minScale <= maxScale
    if (minScale > maxScale) {
        qreal temp = (minScale + maxScale) / 2.0;
        minScale = temp * 0.8;
        maxScale = temp * 1.2;
    }

    // Дополнительная проверка
    minScale = qMax(0.05, minScale);
    maxScale = qMax(minScale * 1.1, maxScale);
}

void MazeWidget::wheelEvent(QWheelEvent* event)
{
    if (parser->getMazeData().empty()) return;

    // Рассчитываем размеры лабиринта с учетом масштаба
    qreal mazeWidth = getMazeConstData()[0].size() * cellSize * scaleFactor;
    qreal mazeHeight = getMazeConstData().size() * cellSize * scaleFactor;

    // Размеры виджета с учетом кнопок
    qreal widgetWidth = width();
    qreal availableHeight = height() - openFileButton->height() - 10; // Доступная высота для лабиринта

    // Добавляем дополнительные отступы для комфортного просмотра
    const qreal paddingX = 50.0; // Дополнительный отступ по горизонтали
    const qreal paddingY = 50.0; // Дополнительный отступ по вертикали

    if (event->modifiers() & Qt::ControlModifier) {
        // Инициализируем масштаб перед использованием
        initializedMinMaxScale();

        // Масштабирование
        const double zoomSensitivity = 0.2;
        const double zoomFactorBase = 1.1;

        double wheelDelta = event->angleDelta().y();
        double zoomFactor = pow(zoomFactorBase, zoomSensitivity * qAbs(wheelDelta) / 120.0);

        double oldScale = scaleFactor;

        if (wheelDelta > 0) {
            scaleFactor *= zoomFactor;
        } else {
            scaleFactor /= zoomFactor;
        }

        // КРИТИЧЕСКАЯ ПРОВЕРКА: убеждаемся что границы корректны
        if (minScale > maxScale) {
            qDebug() << "Warning: minScale > maxScale, fixing...";
            minScale = 0.1;
            maxScale = 10.0;
        }

        // Применяем ограничения масштаба только если границы корректны
        if (minScale <= maxScale) {
            scaleFactor = qBound(minScale, scaleFactor, maxScale);
        } else {
            // Fallback на безопасные значения
            scaleFactor = qBound(0.1, scaleFactor, 10.0);
        }

        // Плавное масштабирование относительно позиции курсора
        QPointF mousePos = event->position();
        QPointF centerDelta = mousePos - QPointF(width()/2, height()/2);

        if (oldScale != 0) {  // Защита от деления на ноль
            scrollOffset = (scrollOffset + centerDelta) * (scaleFactor / oldScale) - centerDelta;
        }

        update();
    } else {
        // Прокрутка - только если лабиринт не помещается
        if (mazeWidth <= widgetWidth && mazeHeight <= availableHeight) {
            return;
        }

        QPoint delta = event->angleDelta();
        if (!delta.isNull()) {
            // Рассчитываем допустимые границы скроллинга с учетом отступов
            qreal maxXOffset = qMax(0.0, (mazeWidth - widgetWidth) / 2.0 + paddingX);

            // Для вертикального скроллинга учитываем высоту кнопок и добавляем отступы
            qreal buttonOffset = (openFileButton->height() + 10) / 2.0; // Смещение из-за кнопок сверху
            qreal maxYOffset = qMax(0.0, (mazeHeight - availableHeight) / 2.0 + paddingY + buttonOffset);

            if (event->modifiers() & Qt::ShiftModifier) {
                // Горизонтальный скроллинг
                scrollOffset.rx() -= delta.y() * 0.5; // Немного замедляем скорость
                scrollOffset.rx() = qBound(-maxXOffset, scrollOffset.x(), maxXOffset);
            } else {
                // Вертикальный скроллинг
                scrollOffset.ry() -= delta.y() * 0.5; // Немного замедляем скорость
                scrollOffset.ry() = qBound(-maxYOffset, scrollOffset.y(), maxYOffset);
            }
            update();
        }
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

