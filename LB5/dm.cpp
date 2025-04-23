#include <iostream>

int f(int a)
{
    int sum = 0;
    while (a > 0)
    {
        sum += a % 2 == 0 ? 1 : 0;
        a -= 3;
        if (a > 0)
            sum += f(a);
    }
    if (sum == 0 && a % 2 == 0)
        return 1;
    return sum;
}

int main()
{
    std::cout << f(27) + f(19) + f(11) + f(3);
    // int arr[28]{1, 0, 1, 1, 1, 2, 2, 3, 5};
    // for (int i = 8; i < 28; i++)
    // {
    //     arr[i] = arr[i - 8] + arr[i - 3] + arr[i - 2];
    // }
    // for (int i = 0; i < 28; i++)
    // {
    //     std::cout << "f( " << i << " ) = " << arr[i] << std::endl;
    // }
    return 0;
}