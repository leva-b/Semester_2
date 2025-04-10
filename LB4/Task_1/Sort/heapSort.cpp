#include "../services/array.h"
#include <algorithm>

void Array::heapify(uint n, uint i)
{
    while (true)
    {
        uint largest = i;
        uint l = 2 * i + 1;
        uint r = 2 * i + 2;

        if (l < n && sortArr[l] > sortArr[largest])
            largest = l;
        if (r < n && sortArr[r] > sortArr[largest])
            largest = r;

        if (largest == i)
            break;

        std::swap(sortArr[i], sortArr[largest]);
        i = largest;
    }
}

void Array::heapSort()
{
    if (sizeArr <= 1)
        return;

    // Построение кучи (итеративный вариант)
    for (int i = sizeArr / 2 - 1; i >= 0; --i)
    {
        heapify(sizeArr, i);
    }

    // Извлечение элементов из кучи
    for (uint i = sizeArr - 1; i > 0; --i)
    {
        std::swap(sortArr[0], sortArr[i]);
        heapify(i, 0);
    }
}