#include <iostream>

int max_of(int a, int b);
double max_of(double a, double b);

int main()
{
    std::cout << max_of(3, 7) << '\n';
    std::cout << max_of(2.5, 1.5) << '\n';
}

int max_of(int a, int b)
{
    return (a > b) ? a : b;
}

double max_of(double a, double b)
{
    return (a > b) ? a : b;
}
