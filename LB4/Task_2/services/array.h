using uint = unsigned int;

class Array
{
private:
    uint sizeArr = 0;
    int *myArr = nullptr;
    int *sortArr = nullptr;
    void initSortArr();
    void interpolationSort();

public:
    void generationArray();
    void init();

    void sort();
    uint getSizeArr();

    uint binsearch(int number);
    long long binpow(int digit, int powder, int mod);
    static void printArray(int *arr, uint size);
    ~Array();
};