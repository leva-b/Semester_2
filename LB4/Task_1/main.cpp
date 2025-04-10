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
        std::cout << "4.Output array\n";
        std::cout << "5.Output sorted array\n";
        std::cout << "6.Exit" << std::endl;
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
            std::cout << myArray.binsearch(digit) << std::endl;
            break;
        case '4':
            myArray.print();
            break;
        case '5':
            myArray.printSortedArray();
            break;
        case '6':
            std::cout << "Program execution completed\n";
            return 0;
        default:
            std::cout << "Input error, please try again\n";
            break;
        }
    }
    return 0;
}