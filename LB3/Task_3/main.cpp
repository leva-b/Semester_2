#include <iostream>
#include <cmath>


long long reverse_number(int n, int& z, int k = 0)
{
    if (n == 0)
        return 0;
    z++;
    k++;
    int q = reverse_number(n / 10, z, k) + (n % 10) * pow(10, z-k);
    return q;
}

int main()
{
    int z = 0;
    std::cout << reverse_number(19572, z);
    return 0;
}
