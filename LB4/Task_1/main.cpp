#include <iostream>
#include "services/array.h"

int main()
{
    char c;
    Array myArray;
    while (true)
    {
        std::cout << "Menu:\n";
        std::cout << "1.Enter array\n";
        std::cout << "2.Sort the array\n";
        std::cout << "3.Binary search\n";
        std::cout << "4.Exit" << std::endl;
        std::cin >> c;
        switch (c)
        {
        case '1':
            myArray.generationArray();
            break;
        case '2':
            myArray.sort();
            break;
        case '3':
            int digit;
            std::cout << "Enter a number to search: ";
            std::cin >> digit;
            myArray.binsearch(digit);
        case '4':
            std::cout << "Program execution completed";
            return 0;
        default:
            std::cout << "Input error, please try again\n";
            break;
        }
    }
    return 0;
}