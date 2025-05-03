#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <mazefromfileparser.h>

class MazeWidget : public QWidget {
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr);

    std::vector<std::vector<char>>& getMazeData() ;
    const std::vector<std::vector<char>>& getMazeConstData() const;
    void wheelEvent(QWheelEvent* event) override;
    QSize sizeHint() const override;

private slots:
    void findWayThroughMaze();
    void loadMazeFromFile();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    double scaleFactor = 1.0;
    const int minCellSize = 5;
    //QWidget* mazeDisplay;
    //std::vector<std::vector<char>> maze;
    MazeFromFileParser *parser;
    int cellSize = 20;
    QPushButton *findPathButton;
    QPushButton *openFileButton;
    const char wall_symbol = '1';
    const char path_symbol = '0';
    void drawPath(QPainter &painter, const std::vector<QPoint> &path);
    void toggleWall(const QPoint &pos);
    //void setupUI();
};

#endif // MAZEWIDGET_H
