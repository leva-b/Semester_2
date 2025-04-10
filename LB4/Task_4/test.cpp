#include <vector>
#include <iostream>
#include <cstdint>
#include "headers/vector.h"
#include <vector>

int main()
{
    Vector<Pair<Vector<int>, Vector<Pair<int, double>>>> data;
    Vector<int> firstVec1;
    firstVec1.push_back(10);
    firstVec1.push_back(20);
    firstVec1.push_back(30);
    Vector<Pair<int, double>> secondVec1;
    secondVec1.emplace_back(1, 1.5);
    secondVec1.emplace_back(2, 2.5);
    data.emplace_back(firstVec1, secondVec1);

    Vector<int> firstVec2;
    firstVec2.push_back(40);
    firstVec2.push_back(50);
    Vector<Pair<int, double>> secondVec2;
    secondVec2.emplace_back(3, 3.5);
    data.emplace_back(std::move(firstVec1), std::move(secondVec2));
    int a;
    std::cout << "                   " << a << std::endl;
    data.emplace(data.begin() + 0, Vector<int>(), Vector<Pair<int, double>>());

    return 0;
}