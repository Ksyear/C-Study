# Day 05 — 연습 문제 정답 (함수)

---

## 문제 1 정답

```cpp
#include <iostream>

// 유클리드 호제법 (반복 버전)
int gcd(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main() {
    int a = 0, b = 0;
    std::cout << "Enter a b: ";
    std::cin >> a >> b;
    if (a <= 0 || b <= 0) {
        std::cerr << "[ERROR] inputs must be positive\n";
        return 1;
    }
    std::cout << "gcd = " << gcd(a, b) << '\n';
    return 0;
}
```

**해설**
- 재귀 버전: `int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }` — 간결하지만 재귀 깊이가 깊어질 수 있습니다. 임베디드에서는 반복 버전을 권장합니다.
- 시간 복잡도: O(log min(a, b)). 매우 빠릅니다.
- 음수 처리: 본 풀이는 양수만 가정합니다. 실제로는 `std::abs`로 양수화하거나 `std::gcd`(`<numeric>`, C++17)를 사용할 수 있습니다.

---

## 문제 2 정답

```cpp
#include <iostream>
#include <string>
#include <vector>

void print_info(int v) {
    std::cout << "int: " << v << '\n';
}

void print_info(double v) {
    std::cout << "double: " << v << '\n';
}

void print_info(const std::string& v) {
    std::cout << "string: " << v << '\n';
}

void print_info(const std::vector<int>& v) {
    std::cout << "vector(" << v.size() << "):";
    for (const auto& x : v) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    print_info(42);
    print_info(3.14);
    print_info(std::string("hello"));
    print_info(std::vector<int>{1, 2, 3});
    return 0;
}
```

**해설**
- `print_info("hello")` (string literal)는 어떻게 매칭될까요? string literal은 `const char*`이고, `std::string`으로 묵시적 변환됩니다. 다만 `int`/`double`로의 변환도 가능성이 검토되므로 모호성 회피를 위해 `std::string("hello")`로 명시했습니다. C++17의 string literal suffix(`"hello"s`)를 사용하면 더 깔끔합니다(`using namespace std::string_literals;`).
- 큰 객체(`std::string`, `std::vector<int>`)를 `const T&`로 받았기 때문에 복사가 발생하지 않습니다.
- 컴파일러는 인자 타입을 보고 최적의 오버로드를 선택합니다. 후보 두 개가 동시에 가능하면 모호성 에러가 발생합니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <cstddef>

constexpr uint16_t kAdcMax = 4095;

double adc_to_voltage(uint16_t raw, double vref = 3.3) {
    if (raw > kAdcMax) {
        std::cerr << "[WARN] raw clamped: " << raw << " -> " << kAdcMax << '\n';
        raw = kAdcMax;
    }
    return static_cast<double>(raw) * vref / kAdcMax;
}

std::size_t convert_all(const std::vector<uint16_t>& in_raw,
                        std::vector<double>& out_v,
                        double vref = 3.3) {
    out_v.clear();
    out_v.reserve(in_raw.size());
    for (const auto& r : in_raw) {
        out_v.push_back(adc_to_voltage(r, vref));
    }
    return out_v.size();
}

int main() {
    std::vector<uint16_t> raws = {0, 1024, 2048, 3072, 4095};
    std::vector<double> volts;

    const std::size_t processed = convert_all(raws, volts);

    std::cout << std::fixed << std::setprecision(3);
    for (std::size_t i = 0; i < raws.size(); ++i) {
        std::cout << "raw=" << raws[i] << "\t-> " << volts[i] << " V\n";
    }
    std::cout << "processed = " << processed << '\n';
    return 0;
}
```

**해설 / 임베디드 관점**
- `<cstdint>`의 `uint16_t`는 정확히 16비트 unsigned. 실제 STM32의 ADC 결과 레지스터(`ADC_DR`)는 보통 16비트 워드에 12비트 값을 담아 반환합니다.
- `uint16_t`는 `int`로 자동 promotion되므로 산술 연산에서는 별 문제 없지만, 출력 시 `std::cout`이 `int`로 출력합니다. 일부 경우 `static_cast<unsigned>`가 필요할 수 있습니다.
- `vref`의 기본값 3.3은 헤더의 함수 **선언부에 한 번만** 적어야 합니다. 정의에도 기본값을 적으면 컴파일 에러. (본 풀이는 선언과 정의를 분리하지 않았으므로 정의에 기본값을 두었습니다.)
- `convert_all`의 `out_v`를 출력 매개변수로 받는 패턴은 임베디드에서 자주 사용됩니다. 큰 객체를 반환값으로 만들지 않아 NRVO에 의존하지 않고 메모리 동작을 명시적으로 제어할 수 있습니다.
- `static_cast<double>(raw)`로 명시 변환: `raw * vref`는 자동으로 `double` 산술이 되지만, `raw * vref / kAdcMax`의 평가 순서에 따라 정수 오버플로우 가능성이 있을 수도 있어 명시한 것이 안전합니다 (이 경우는 vref가 left-associative로 먼저 평가되어 double이 되므로 안전하지만, 일반적 습관).
- 실제 ADC 변환 공식은 ADC의 분해능에 따라 `4095` 대신 `4096`을 쓰는 경우도 있습니다(분모 = 2^N 또는 2^N - 1). 데이터시트 확인이 필요합니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `void f(std::string s)`로 큰 객체 값 전달 | 매 호출마다 복사 비용 |
| 지역 변수를 참조로 반환 | 댕글링 참조, UB |
| 정의에 기본 인자 중복 | 컴파일 에러 |
| 헤더에 비-`inline` 함수 정의 | ODR 위반, 링커 에러 |
| 오버로드 모호성 (`int`/`double` 혼재 시) | 컴파일 에러 |
| 깊은 재귀 | MCU에서 스택 오버플로우 |
| `nullptr`를 받을 수 있는 포인터를 검사 없이 dereference | UB |
