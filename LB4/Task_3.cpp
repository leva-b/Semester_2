#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

int main()
{
    std::vector<int> a, b;
    std::string input;

    std::cout << "Enter numbers (space-separated):" << std::endl;
    std::getline(std::cin, input);

    std::replace(input.begin(), input.end(), ',', ' ');

    std::istringstream stream(input);
    int number;
    // Считываем числа из потока
    while (stream >> number)
    {
        a.push_back(number);
    }
    int remainder = a.size() % 3;
    for (int i = 0; i < a.size() - remainder; i += 3)
    {
        if ((a[i] >= a[i + 1] && a[i] <= a[i + 2]) || (a[i] <= a[i + 1] && a[i] >= a[i + 2]))
        {
            b.push_back(a[i]);
            continue;
        }
        else if ((a[i + 1] >= a[i + 2] && a[i + 1] <= a[i]) || (a[i + 1] <= a[i + 2] && a[i + 1] >= a[i]))
        {
            b.push_back(a[i + 1]);
            continue;
        }
        else if ((a[i + 2] >= a[i] && a[i + 2] <= a[i + 1]) || (a[i + 2] <= a[i] && a[i + 2] >= a[i + 1]))
        {
            b.push_back(a[i + 2]);
            continue;
        }
    }
    if (remainder != 0)
    {
        int arithmeticMean = 0;
        for (int i = a.size() - remainder; i < a.size(); i++)
        {
            arithmeticMean += a[i];
        }
        b.push_back(arithmeticMean / remainder);
    }
    std::cout << "array b: { ";
    for (int i = 0; i < b.size(); i++)
    {
        std::cout << b[i] << ' ';
    }
    std::cout << '}';
    return 0;
}