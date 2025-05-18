#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <mazefromfileparser.h>

class MazeWidget : public QWidget {
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr, MazeFromFileParser* parser = nullptr);

    std::vector<std::vector<char>>& getMazeData() ;
    const std::vector<std::vector<char>>& getMazeConstData() const;
    void wheelEvent(QWheelEvent* event) override;
    QSize sizeHint() const override;
    void clearPath();
    MazeFromFileParser* getConstParser();

private slots:
    void findWayThroughMaze();
    void loadMazeFromFile();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    double scaleFactor = 1.0;
    //const int minCellSize = 5;
    const char wall_symbol = '1';
    const char path_symbol = '0';
    //QWidget* mazeDisplay;
    //std::vector<std::vector<char>> maze;
    MazeFromFileParser *parser;
    int cellSize = 20;
    QPushButton *findPathButton;
    QPushButton *openFileButton;
    QPoint lastCellPos = QPoint(-1, -1);
    QTimer* animationTimer;
    QColor pathColor;
    std::vector<QPoint> pathCells;  // Все клетки пути (x, y)
    int currentPathIndex = 0;
    int totalPathCells = 0;
    void toggleWall(const QPoint &pos);
    bool isCorrectMaze();
    void drawPath(int startX, int startY);


    //void setupUI();
};

#endif // MAZEWIDGET_H
