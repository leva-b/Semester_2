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
    void loadMazeFromFile(const QString &filePath);
    std::vector<std::vector<int>> getMazeData() const;

signals:
    void findPathRequested();
    void openFileRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::vector<std::vector<int>> maze;
    //MazeFromFileParser parser;
    int cellSize = 20;
    QPushButton *findPathButton;
    QPushButton *openFileButton;

    void drawPath(QPainter &painter, const std::vector<QPoint> &path);
    void toggleWall(const QPoint &pos);
    void setupUI();
};

#endif // MAZEWIDGET_H
