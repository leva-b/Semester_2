#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stack>

using namespace std;

struct Point
{
    int x;
    int y;
};

void generateMaze(int width, int height, const string &filename)
{
    // Убедимся, что размеры нечетные для правильного лабиринта
    if (width % 2 == 0)
        width++;
    if (height % 2 == 0)
        height++;

    // Инициализация лабиринта (все клетки - стены)
    vector<vector<char>> maze(height, vector<char>(width, '#'));

    // Выбираем случайную стартовую точку (нечетные координаты)
    srand(time(0));
    int startX = 1 + 2 * (rand() % ((width - 1) / 2));
    int startY = 1 + 2 * (rand() % ((height - 1) / 2));
    maze[startY][startX] = ' ';

    // Стек для алгоритма
    stack<Point> cells;
    cells.push({startX, startY});

    // Направления движения
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};

    while (!cells.empty())
    {
        Point current = cells.top();
        vector<int> directions = {0, 1, 2, 3};
        random_shuffle(directions.begin(), directions.end());

        bool found = false;
        for (int dir : directions)
        {
            int nx = current.x + 2 * dx[dir];
            int ny = current.y + 2 * dy[dir];

            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == '#')
            {
                // Прорываем стену между текущей клеткой и новой
                maze[current.y + dy[dir]][current.x + dx[dir]] = ' ';
                maze[ny][nx] = ' ';
                cells.push({nx, ny});
                found = true;
                break;
            }
        }

        if (!found)
        {
            cells.pop();
        }
    }

    // Добавим вход и выход
    maze[1][0] = ' ';                  // Вход
    maze[height - 2][width - 1] = ' '; // Выход

    // Сохраняем в файл
    ofstream outFile(filename);
    for (const auto &row : maze)
    {
        for (char c : row)
        {
            outFile << c;
        }
        outFile << '\n';
    }
    outFile.close();
}

int main()
{
    // Генерируем 5 лабиринтов разного размера
    generateMaze(21, 11, "maze_small.txt");
    generateMaze(31, 15, "maze_medium.txt");
    generateMaze(41, 21, "maze_large.txt");
    generateMaze(51, 25, "maze_xlarge.txt");
    generateMaze(61, 31, "maze_huge.txt");

    cout << "Лабиринты успешно сгенерированы!" << endl;
    return 0;
}