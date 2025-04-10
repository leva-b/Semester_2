#include <cstddef>
using uint = unsigned int;

class Array
{
private:
    uint sizeArr = 0;
    int *myArr = nullptr;
    int *sortArr = nullptr;
    void initSortArr();

    void mergeSort(uint left, uint right);
    void merge(uint left, uint middle, uint right);

    void quickSort(int first, int last);

    void heapify(uint n, uint i);
    void heapSort();

public:
    void print();
    void printSortedArray();
    void generationArray();
    void init();
    void sort();
    uint binsearch(int number);
    ~Array();
};