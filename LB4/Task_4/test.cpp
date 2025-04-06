#include <vector>
#include <iostream>
#include <cstdint>

int main()
{
    std::vector<int> a(7);

    a.insert(a.begin() + 5, 10, 5);
    return 0;
}