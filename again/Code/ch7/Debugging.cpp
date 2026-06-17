#include <iostream>

int make_offset()
{
    int offset = 100;
    return offset;
}

void add_bias(int& value, int bias)
{
    value += bias;
}

int main()
{
    int reading = 50;
    add_bias(reading, 10);
    std::cout << reading << '\n';   // 기대 출력: 60

    int off = make_offset();
    std::cout << off << '\n';       // 기대 출력: 100
}
