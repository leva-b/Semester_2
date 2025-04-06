#include "../services/array.h"
#include <algorithm>

void Array::quickSort(int first, int last)
{
    if (first >= last)
    {
        return;
    }
    int left = first;
    int right = last;
    int pivot = sortArr[left + (right - left) / 2];
    while (left <= right)
    {
        while (sortArr[left] < pivot)
            left++;
        while (sortArr[right] > pivot)
            right--;
        if (left <= right)
            std::swap(sortArr[left++], sortArr[right--]);
    }
    quickSort(first, right);
    quickSort(left, last);
}
// #include "array.h"
// #include <algorithm>
// #include <cstdlib>

// void Array::quickSort(int first, int last)
// {
//     if (first >= last)
//     {
//         return;
//     }

//     // Выбор опорного элемента
//     int pivotIndex = first + std::rand() % (last - first + 1);
//     std::swap(sortArr[pivotIndex], sortArr[last]); // Перемещение опорного элемента в конец
//     int pivot = sortArr[last];

//     int left = first;
//     int right = last - 1;

//     while (left <= right)
//     {
//         while (left <= right && sortArr[left] < pivot)
//             left++;
//         while (left <= right && sortArr[right] > pivot)
//             right--;
//         if (left <= right)
//         {
//             std::swap(sortArr[left++], sortArr[right--]);
//         }
//     }

//     // Рекурсивные вызовы
//     quickSort(first, right);
//     quickSort(left, last);
// }