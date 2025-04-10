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