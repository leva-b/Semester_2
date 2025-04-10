#include <iostream>
#include "services/array.h"
#include <ctime>
int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    char c;
    Array myArray;
    while (true)
    {
        std::cout << "Menu:\n";
        std::cout << "1.Enter an array\n";
        std::cout << "2.Sort the array\n";
        std::cout << "3.Output the index of the found element raised\nto the power of the array length modulo the number.\n";
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
            long module;
            std::cout << "Enter a number to search: ";
            std::cin >> digit;
            std::cout << "Enter a module number: ";
            std::cin >> module;
            std::cout << myArray.binpow(myArray.binsearch(digit), myArray.getSizeArr(), module) << std::endl;
            break;
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