#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStack>
#include <QPoint>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void applyDiskCount();
    void resetGame();

private:
    Ui::MainWindow *ui;
    QVector<QStack<int>> towers;
    int selectedTower = -1;
    bool isDragging = false;
    int draggedDisk = -1;
    QPoint dragStartPos;
    QPoint dragOffset;
    int diskCount = 7;
    int diskHeight = 22;

    std::vector<std::pair<int, int>> moves;
    unsigned int currentDisk = 0;
    QTextEdit *textEdit;
    QLineEdit *diskCountInput;
    QPushButton *resetButton;
    QPushButton *nextStep;
    QPushButton *executeAll;


    void executeOneStep();

    void generateMoves(int n, int fromPeg, int toPeg, int unusedPeg);
    void executeAllSteps();
    void drawTowers(QPainter &painter);
    void moveDisk(int from, int to);

    void initializeTowers();
    void updateTextColor(QLineEdit *lineEdit);
};

#endif // MAINWINDOW_H
