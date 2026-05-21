# Day 01 — 연습 문제 정답 (입출력)

> **주의:** 먼저 본인의 풀이를 작성한 뒤에 확인하세요. 정답 코드는 하나의 가능한 풀이일 뿐, 유일한 정답이 아닙니다.

---

## 문제 1 정답

```cpp
#include <iostream>
#include <string>

int main() {
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cout << "Hello, " << name << "!\n";
    return 0;
}
```

**해설**
- `std::string`은 `<string>` 헤더에 정의되어 있습니다. `<iostream>`만 include한다고 자동으로 포함되지 않습니다(구현에 따라 우연히 포함될 수는 있으나 의존하지 말 것).
- `std::cin >> name`은 공백/줄바꿈을 만나기 전까지의 한 단어만 읽습니다.

---

## 문제 2 정답

```cpp
#include <iostream>

int main() {
    int a = 0, b = 0;
    std::cout << "Enter two integers: ";
    std::cin >> a >> b;

    if (b == 0) {
        std::cerr << "[ERROR] Division by zero.\n";
        return 1;
    }

    std::cout << a << " + " << b << " = " << (a + b) << '\n';
    std::cout << a << " - " << b << " = " << (a - b) << '\n';
    std::cout << a << " * " << b << " = " << (a * b) << '\n';
    std::cout << a << " / " << b << " = " << (a / b) << '\n';
    std::cout << a << " % " << b << " = " << (a % b) << '\n';
    return 0;
}
```

**해설**
- `b == 0` 체크는 나눗셈/나머지 연산 **이전에** 수행해야 합니다. `int` 0으로 나누는 것은 **정의되지 않은 동작(Undefined Behavior)**이며, 일부 플랫폼에서는 `SIGFPE` 시그널로 프로그램이 즉시 종료됩니다.
- 입력 검증 실패 시 `std::cerr`로 출력하고 종료 코드 1을 반환하는 것은 POSIX/UNIX 관행입니다. 셸 스크립트에서 `$?`로 확인 가능합니다.

---

## 문제 3 정답

```cpp
#include <iostream>

int main() {
    constexpr double kThreshold = 85.0;
    constexpr int kSampleCount = 5;

    std::cout << "Enter " << kSampleCount << " temperature samples:\n";

    double sum = 0.0;
    int exceededCount = 0;

    for (int i = 1; i <= kSampleCount; ++i) {
        double sample = 0.0;
        std::cin >> sample;
        sum += sample;
        if (sample > kThreshold) {
            std::cerr << "[WARN] sample #" << i
                      << " exceeded threshold: " << sample << " C\n";
            ++exceededCount;
        }
    }

    const double average = sum / kSampleCount;
    std::cout << "Average: " << average << " C\n";
    std::cout << "Exceeded count: " << exceededCount << '\n';
    return 0;
}
```

**해설 / 임베디드 관점**
- `constexpr`은 컴파일 타임 상수입니다. 매크로(`#define`)와 달리 **타입을 가지고 스코프를 따르며 디버거에서 보입니다**. 임베디드 코드에서 상수는 `#define`이 아니라 `constexpr`을 우선 사용하세요.
- 입력 값을 배열에 모두 저장하지 않고 누적합(`sum`)과 카운터(`exceededCount`)만 유지했습니다. 메모리 제약이 있는 MCU에서 흔히 쓰는 **online algorithm** 형태입니다.
- 평균 계산 시 `kSampleCount`가 정수라 `sum / kSampleCount`가 정수 나눗셈으로 빠질 위험이 있는데, `sum`이 `double`이므로 promotion 규칙에 따라 부동소수점 나눗셈이 수행됩니다. 만약 `sum`이 `int`였다면 `static_cast<double>(sum) / kSampleCount`로 명시했어야 합니다.
- 추가로, 실시간 시스템에서는 `std::cout`의 출력 자체가 비결정적(non-deterministic)이므로 핵심 제어 루프 안에서는 **출력 호출을 피하고**, 별도의 로깅 task로 분리하는 것이 일반적입니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `using namespace std;` 사용 | 이름 충돌, 헤더 오염 위험 |
| `std::endl` 남발 | 불필요한 플러시로 성능 저하 |
| `cin >> int` 후 `getline` | 빈 줄 읽힘 (개행 잔존) |
| 0 나눗셈 미검사 | UB, 일부 플랫폼에서 SIGFPE |
| `int` 사이 나눗셈 결과를 `double`에 대입 | 의도와 다른 정수 나눗셈 |
