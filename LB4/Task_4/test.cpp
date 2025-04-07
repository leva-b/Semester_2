#include <vector>
#include <iostream>
#include <cstdint>
#include "headers/vector.h"

int main()
{
    Pair<Vector<int>, Vector<Pair<int, double>>> a;
    a.first.resize(10);
    a.second.resize(12);
    a.first[1] = 10;
    (a.second)[1].first = 4;
    std::cout << a.first[1] << ' ' << (a.second)[1].first;
    // Vector<Pair<int, double>> a(7);
    // for (int i = 0; i < a.size(); i++)
    // {
    //     std::cout << a[i].first << ' ' << a[i].second << "; ";
    // }
    // std::cout << std::endl;
    // // a.insert(a.begin() + 2, 10, 3);
    // a.insert(a.begin() + 3, Pair<int, double>(111, 10.1));
    // for (int i = 0; i < a.size(); i++)
    // {
    //     std::cout << a[i].first << ' ' << a[i].second << "; ";
    // }
    // std::cout << std::endl;
    return 0;
}