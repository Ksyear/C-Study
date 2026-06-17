#include <iostream>

std::uint8_t xor_checksum(const std::uint8_t* data, std::size_t n);
bool frame_ok(const std::uint8_t* frame, std::size_t n);
void run_tests();

int main()
{
    run_tests();
}

std::uint8_t xor_checksum(const std::uint8_t* data, std::size_t n)
{
    std::uint8_t sum = 0;
    for(int i = 0; i < n; i++)
	{
	    sum ^= data[i];
	}
    return sum;
}

bool frame_ok(const std::uint8_t* frame, std::size_t n)
{
    if (n < 2) return false;
    return xor_checksum(frame, n - 1) == frame[n - 1];
}

void run_tests()
{
    const std::uint8_t good[] = {0x10, 0x20, 0x30, 0x00};   // 0x10^0x20^0x30 = 0x00
    const std::uint8_t bad[]  = {0x10, 0x20, 0x30, 0xFF};   // checksum 불일치
    const std::uint8_t tiny[] = {0x42};                     // 최소 길이 미달
	
    std::cout << "valid frame" << (frame_ok(good, sizeof good) ? ": PASS" : ": FAIL") << '\n'; 
    std::cout << "checksum mismatch" << (!frame_ok(bad, sizeof bad) ? ": PASS" : ": FAIL") << '\n';
    std::cout << "below minimum length" << (!frame_ok(tiny, sizeof tiny) ? ": PASS" : ": FAIL") << '\n';
}
