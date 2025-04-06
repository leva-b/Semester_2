#include "../services/array.h"
#include <iostream>
#include <limits>
#include <vector>
#include <ctime>

void Array::init()
{
    if (myArr != nullptr)
    {
        delete[] myArr;
    }
    if (sortArr != nullptr)
    {
        delete[] myArr;
    }
    std::cout << "Enter array size: ";
    std::cin >> sizeArr;
    myArr = new int[sizeArr];
    for (int i = 0; i < sizeArr; i++)
    {
        std::cin >> myArr[i];
    }

    std::cin.clear();
    if (std::cin.peek() != '\n')
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void Array::initSortArr()
{
    if (sortArr != nullptr)
    {
        delete[] sortArr;
    }
    sortArr = new int[sizeArr];
    for (unsigned int i = 0; i < sizeArr; i++)
    {
        sortArr[i] = myArr[i];
    }
}

uint Array::binsearch(int digit)
{
    if (sortArr == nullptr)
    {
        initSortArr();
        interpolationSort();
    }
    int left = 0, right = sizeArr - 1, middle = (left + right) / 2;
    while (left <= right)
    {
        middle = left + (right - left) / 2;
        if (sortArr[middle] > digit)
        {
            right = middle - 1;
        }
        else if (sortArr[middle] < digit)
        {
            left = middle + 1;
        }
        else if (sortArr[middle] == digit)
        {
            std::cout << "werwer " << middle;
            return middle;
        }
    }
    std::cout << "werwer " << std::numeric_limits<unsigned int>::max() << std::endl;
    return std::numeric_limits<unsigned int>::max();
}

void Array::sort()
{
    clock_t start, end;

    initSortArr();
    start = clock();
    interpolationSort();
    end = clock();
    std::cout << "interpolition sort time " << end - start << std::endl;
}

Array::~Array()
{
    delete[] myArr;
    delete[] sortArr;
}

void Array::generationArray()
{
    if (myArr != nullptr)
    {
        delete[] myArr;
        myArr = nullptr;
    }

    if (sortArr != nullptr)
    {
        delete[] sortArr;
        sortArr = nullptr;
    }

    std::cout << "Enter array size: ";
    std::cin >> sizeArr;
    myArr = new int[sizeArr];

    for (size_t i = 0; i < sizeArr; ++i)
    {
        myArr[i] = -1000 + std::rand() % (2000 + 1);
    }
}

long long Array::binpow(int digit, int powder, int mod)
{
    if (digit == std::numeric_limits<unsigned int>::max())
    {
        if (powder % 2 == 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    if (powder == 0)
        return 1;
    if (powder % 2 == 1)
        return binpow(digit, powder - 1, mod) * digit % mod;
    else
    {
        long long result = binpow(digit, powder / 2, mod);
        return result * result % mod;
    }
}

uint Array::getSizeArr()
{
    return sizeArr;
}

void Array::printArray(int *arr, uint size)
{
    if (size <= 0)
        return;
    std::cout << '{' << arr[0];
    for (int i = 1; i < size; i++)
    {
        std::cout << ", " << arr[i];
    }
    std::cout << '}' << std::endl;
}