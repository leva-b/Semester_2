#include "../services/array.h"
#include <algorithm>

void Array::interpolationSort()
{
    for (uint i = 1; i < sizeArr; i++)
    {
        // Пропускаем уже упорядоченные элементы
        if (sortArr[i] >= sortArr[i - 1])
        {
            continue;
        }

        int key = sortArr[i];
        int left = 0;
        int right = i - 1;
        int pos = left;

        // Улучшенный интерполяционный поиск
        if (sortArr[right] != sortArr[left])
        {
            // Основная интерполяционная формула
            pos = left + ((key - sortArr[left]) * (right - left)) /
                             (sortArr[right] - sortArr[left]);

            // Гарантируем, что pos находится в границах
            pos = std::max(left, std::min(right, pos));

            // Уточнение позиции бинарным поиском
            if (sortArr[pos] < key)
            {
                left = pos + 1;
                while (left <= right)
                {
                    int mid = left + (right - left) / 2;
                    if (sortArr[mid] < key)
                    {
                        left = mid + 1;
                    }
                    else
                    {
                        right = mid - 1;
                    }
                }
                pos = left;
            }
            else if (sortArr[pos] > key)
            {
                right = pos - 1;
                while (left <= right)
                {
                    int mid = left + (right - left) / 2;
                    if (sortArr[mid] > key)
                    {
                        right = mid - 1;
                    }
                    else
                    {
                        left = mid + 1;
                    }
                }
                pos = left;
            }
        }
        else
        { // Все элементы равны
            pos = (key < sortArr[left]) ? left : right + 1;
        }

        // Оптимизированный сдвиг элементов
        if (static_cast<uint>(pos) < i)
        {
            int temp = sortArr[i];
            for (uint j = i; j > static_cast<uint>(pos); j--)
            {
                sortArr[j] = sortArr[j - 1];
            }
            sortArr[pos] = temp;
        }

        // Отладочный вывод (можно убрать в финальной версии)
        Array::printArray(sortArr, sizeArr);
    }
}