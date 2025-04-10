#include "../services/array.h"
#include <algorithm>
#include <iostream>
void Array::interpolationSort()
{
    if (sortArr == nullptr || sizeArr == 0)
    {
        std::cerr << "Array is not initialized or empty!" << std::endl;
        return;
    }

    for (uint i = 1; i < sizeArr; i++)
    {
        int key = sortArr[i];
        uint l = 0, r = i - 1;

        while (l <= r)
        {
            if (sortArr[r] == sortArr[l])
            {
                if (sortArr[l] < key)
                {
                    l = r + 1;
                }
                break;
            }

            uint pos = l + ((key - sortArr[l]) * (r - l)) / (sortArr[r] - sortArr[l]);

            pos = std::max(l, std::min(pos, r));

            if (sortArr[pos] < key)
            {
                l = pos + 1;
            }
            else
            {
                r = pos - 1;
            }
        }

        for (long j = i - 1; j >= l; j--)
        {
            sortArr[j + 1] = sortArr[j];
        }
        sortArr[l] = key;

        std::cout << "Step " << i << ": ";
        printArray(sortArr, sizeArr);
    }
}