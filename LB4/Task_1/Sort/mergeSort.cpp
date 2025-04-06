#include "../services/array.h"
#include <iostream>

void Array::mergeSort(uint left, uint right)
{
    if (left >= right)
    {
        return;
    }
    int middle = left + (right - left) / 2;
    mergeSort(left, middle);
    mergeSort(middle + 1, right);
    merge(left, middle, right);
}

void Array::merge(uint left, uint middle, uint right)
{
    if (!(left <= middle && middle <= right))
    {
        throw std::invalid_argument("Invalid merge bounds");
    }
    uint leftBorder = left;
    uint rightBorder = middle + 1;

    unsigned int index = 0;
    int *result = new int[right - left + 1];
    while (leftBorder <= middle && rightBorder <= right)
    {
        if (sortArr[leftBorder] < sortArr[rightBorder])
        {
            result[index++] = sortArr[leftBorder++];
        }
        else
        {
            result[index++] = sortArr[rightBorder++];
        }
    }
    while (leftBorder <= middle)
    {
        result[index++] = sortArr[leftBorder++];
    }
    while (rightBorder <= right)
    {
        result[index++] = sortArr[rightBorder++];
    }
    for (unsigned int i = 0; i < right - left + 1; i++)
    {
        sortArr[left + i] = result[i];
    }
    delete[] result;
}