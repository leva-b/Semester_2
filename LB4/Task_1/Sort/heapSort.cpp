#include "../services/array.h"
#include <algorithm>

void Array::heapify(uint n, uint i)
{
    uint largest = i;   // Инициализация корня как самого большого
    uint l = 2 * i + 1; // Левый дочерний элемент
    uint r = 2 * i + 2; // Правый дочерний элемент

    // Если левый дочерний элемент больше корня
    if (l < n && sortArr[l] > sortArr[largest])
        largest = l;

    // Если правый дочерний элемент больше, чем самый большой на данный момент
    if (r < n && sortArr[r] > sortArr[largest])
        largest = r;

    // Если самый большой не корень
    if (largest != i)
    {
        std::swap(sortArr[i], sortArr[largest]);
        // Рекурсивно просеиваем затронутое поддерево
        heapify(n, largest);
    }
}

// Основная функция для сортировки кучей
void Array::heapSort()
{
    if (sizeArr <= 0)
        return; // Проверка на пустой массив

    // Построение кучи (перегруппировка массива)
    for (uint i = sizeArr / 2 - 1; i != static_cast<uint>(-1); --i)
        heapify(sizeArr, i);

    // По одному извлекаем элементы из кучи
    for (uint i = sizeArr - 1; i > 0; --i)
    {
        // Перемещаем корень в конец
        std::swap(sortArr[0], sortArr[i]);
        // Вызываем heapify на уменьшенной куче
        heapify(i, 0);
    }
}