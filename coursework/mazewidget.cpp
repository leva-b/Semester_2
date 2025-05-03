#include "mazewidget.h"
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QMouseEvent>
#include <algorithm>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent)
{
    std::vector<std::vector<char>> maze = {
        {'1', '1', '1', '1', '1', '1', '1', '1'},
        {'1', '0', '0', '0', '0', '0', '0', '1'},
        {'1', '0', '1', '1', '1', '0', '1', '1'},
        {'1', '0', '0', '0', '1', '0', '0', '1'},
        {'1', '1', '1', '0', '1', '1', '0', '1'},
        {'1', '0', '0', '0', '0', '0', '0', '1'},
        {'1', '0', '1', '1', '1', '1', '0', '1'},
        {'1', '1', '1', '1', '1', '1', '1', '1'}
    };
    parser = new MazeFromFileParser(maze, '1', '0');

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Создаем горизонтальный layout для кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    openFileButton = new QPushButton("Open file with maze");
    findPathButton = new QPushButton("Find a way");

    //buttonLayout->addStretch();
    buttonLayout->addWidget(openFileButton);
    buttonLayout->addWidget(findPathButton);
    //buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();

    connect(openFileButton, &QPushButton::clicked, this, &MazeWidget::loadMazeFromFile);
    connect(findPathButton, &QPushButton::clicked, this, &MazeWidget::findWayThroughMaze);
}

void MazeWidget::loadMazeFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        try{
            delete parser;
            qDebug() << filePath.toStdString();
            parser = new MazeFromFileParser(filePath.toStdString(), wall_symbol, path_symbol);
            for(int i = 0; i < parser->getMazeConstData().size(); i++){
                for(int j = 0; j < parser->getMazeConstData()[i].size(); j++){
                    qDebug() << parser->getMazeConstData()[i][j];
                }
                qDebug() << "\n";
            }
            update();
        }catch(const std::runtime_error& e){
            QMessageBox::warning(nullptr, "Error", "Cannot open file for reading:");
        }
    } else {
        QMessageBox::warning(this, "Invalid File", "Please select a valid .txt file.");
    }
}

void MazeWidget::findWayThroughMaze(){

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

    int startY = openFileButton->height() + 10;

    qDebug() << width()  << " " <<  height();    // Draw maze
    for (size_t y = 0; y < getMazeConstData().size(); ++y) {
        for (size_t x = 0; x < getMazeConstData()[y].size(); ++x) {
            QRect cellRect(x * cellSize, y * cellSize + startY, cellSize, cellSize);
            if (getMazeConstData()[y][x] == '1') {
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
    if(parser->getMazeData().empty() || event->button() != Qt::LeftButton)
        return;
    QPoint pos = event->pos();
    int startY = openFileButton->height() + 10;

    // Проверяем, что клик был в области лабиринта
    if (pos.y() >= startY) {
        int x = pos.x() / cellSize;
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
    QPoint posInMaze = event->pos();
    if (rect().contains(posInMaze) && (event->buttons() & Qt::LeftButton)) {
        toggleWall(posInMaze);
    }
}

void MazeWidget::toggleWall(const QPoint &pos)
{
    int x = pos.x();
    int y = pos.y() ;

    if (x >= 0 && y >= 0 && y < static_cast<int>(getMazeConstData().size()) && x < static_cast<int>(getMazeConstData()[y].size())) {
        getMazeData()[y][x] = getMazeConstData()[y][x] == '1' ? '0' : '1';
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


void MazeWidget::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        const double zoomFactor = 1.15;
        if (event->angleDelta().y() > 0) {
            scaleFactor *= zoomFactor;
        } else {
            scaleFactor /= zoomFactor;
        }
        scaleFactor = qBound(0.1, scaleFactor, 10.0); // Ограничиваем масштаб
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
