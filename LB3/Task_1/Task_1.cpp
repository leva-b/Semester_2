#include <iostream>
#include <cmath>
#include <algorithm>

using std::cout, std::endl, std::string, std::cin;

string integer_to_binary(unsigned long long number, string curBinaryNumber = ""){
    if(number == 0){
        std::reverse(curBinaryNumber.begin(), curBinaryNumber.end());
        return curBinaryNumber;
    }
    curBinaryNumber += (number % 2) + '0';
    number /= 2;
    return integer_to_binary(number, curBinaryNumber);
}

string fractional_to_binary(double number, string curBinaryNumber = ""){
    if(number - 1e-7 < 0){
        return curBinaryNumber;
    }
    curBinaryNumber += floor(number * 2) + '0';
    number *= 2;
    if(number >= 1)number--;
    return fractional_to_binary(number, curBinaryNumber);
}

double fractional(double number){
    return number - floor(number);
}

int main()
{
    char c;
    double decimal_number;
    string result;
    while(true){
        cout << "Введите * чтобы выйти или - любую другую клавишу для продолжения: " << endl;
        cin >> c;
        if(c == '*'){
            break;
        }else{
            std::cout << "Введите число: ";
            cin >> decimal_number;
            result += integer_to_binary(decimal_number);
            
            std::cout << "Результат в двоичной системе: " << integer_to_binary(decimal_number);
            string fraction = fractional_to_binary(fractional(decimal_number));
            if(!fraction.empty()){
                cout << '.' << fraction;
            }
            cout << endl;
        }
    }
    return 0;
} 