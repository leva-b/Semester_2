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
        quickSort(0, sizeArr - 1);
    }
    int left = 0, right = sizeArr - 1, middle = (left + right) / 2;
    while (left <= right)
    {
        middle = left + (right - left) / 2;
        if (sortArr[middle] > digit)
        {
            right = middle;
        }
        else if (sortArr[middle] < digit)
        {
            left = middle;
        }
        else if (sortArr[middle] == digit)
        {
            return middle;
        }
    }
    return std::numeric_limits<unsigned int>::max();
}

void Array::sort()
{
    clock_t start, end;

    initSortArr();
    start = clock();
    heapSort();
    end = clock();
    std::cout << "heap sorting time: " << end - start << std::endl;

    initSortArr();
    start = clock();
    quickSort(0, sizeArr - 1);
    end = clock();
    std::cout << "quick sorting time " << end - start << std::endl;

    initSortArr();
    start = clock();
    mergeSort(0, sizeArr - 1);
    end = clock();
    std::cout << "merge sorting time " << end - start << std::endl;
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
        myArr = nullptr; // Устанавливаем указатель в nullptr для предотвращения двойного освобождения
    }

    if (sortArr != nullptr)
    {
        delete[] sortArr;
        sortArr = nullptr;
    }

    std::cout << "Enter array size: ";
    std::cin >> sizeArr;
    myArr = new int[sizeArr];
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Инициализация генератора случайных чисел

    for (size_t i = 0; i < sizeArr; ++i)
    {
        myArr[i] = -1000 + std::rand() % (2000 + 1); // Генерация случайного числа
    }
}