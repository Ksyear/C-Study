# Day 03 — 연습 문제 정답 (반복문)

---

## 문제 1 정답

```cpp
#include <iostream>

int main() {
    int N = 0;
    std::cout << "Enter N: ";
    std::cin >> N;

    if (N <= 0) {
        std::cerr << "[ERROR] N must be positive: " << N << '\n';
        return 1;
    }

    long long sum = 0;  // N이 클 수 있으므로 long long
    for (int i = 1; i <= N; ++i) {
        sum += i;
    }
    std::cout << "Sum = " << sum << '\n';
    return 0;
}
```

**해설**
- N이 큰 경우 `int sum`은 오버플로우 가능성이 있어 `long long`을 사용했습니다. `int` 범위는 보통 ±2,147,483,647이며, N = 65536일 때 합은 약 21억으로 `int` 한계에 근접합니다.
- 학습 목적이 아닌 실제 계산이라면 가우스 공식 `N * (N+1) / 2`로 O(1)에 해결 가능하지만, 본 문제는 반복문 학습이 목적입니다.

---

## 문제 2 정답

```cpp
#include <iostream>

int main() {
    int N = 0;
    std::cout << "Enter N: ";
    std::cin >> N;

    if (N < 2) {
        std::cout << '\n';
        return 0;
    }

    for (int n = 2; n <= N; ++n) {
        bool is_prime = true;
        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            std::cout << n << ' ';
        }
    }
    std::cout << '\n';
    return 0;
}
```

**해설**
- `i * i <= n` 조건으로 sqrt(n) 까지만 검사합니다. n = 100이면 i는 10까지만 보면 됩니다.
- `break`로 약수 발견 즉시 검사를 중단합니다. 이게 없으면 불필요한 계산이 늘어납니다.
- 더 효율적인 알고리즘으로 **에라토스테네스의 체**가 있지만 배열을 사용해야 하므로 Day04 이후에 다룰 수 있습니다.
- 임베디드에서 소수 판정은 거의 안 쓰지만, **루프 안에서 조기 종료(early exit)**는 매우 중요한 패턴입니다. 센서 폴링 중 유효 데이터를 찾으면 `break`로 즉시 빠져나가는 식.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    int cycles = 0, duty = 0;
    std::cout << "Enter cycles and duty(%): ";
    std::cin >> cycles >> duty;

    if (cycles <= 0 || duty < 0 || duty > 100) {
        std::cerr << "[ERROR] invalid input\n";
        return 1;
    }

    constexpr int kTicksPerCycle = 100;
    long long totalH = 0;
    long long totalL = 0;

    for (int c = 1; c <= cycles; ++c) {
        std::string line;
        line.reserve(kTicksPerCycle);
        for (int t = 0; t < kTicksPerCycle; ++t) {
            if (t < duty) {
                line.push_back('H');
                ++totalH;
            } else {
                line.push_back('L');
                ++totalL;
            }
        }
        std::cout << "Cycle " << c << ": " << line << '\n';
    }

    const double ratio = (totalH + totalL == 0)
                       ? 0.0
                       : (100.0 * static_cast<double>(totalH) / (totalH + totalL));
    std::cout << "Total H = " << totalH
              << ", L = " << totalL
              << ", H ratio = "
              << std::fixed << std::setprecision(1) << ratio << "%\n";
    return 0;
}
```

**해설 / 임베디드 관점**
- `std::string::reserve(100)`으로 메모리 재할당을 회피했습니다. **임베디드에서 메모리 단편화는 큰 문제**이므로, 가능한 한 컨테이너 크기를 미리 알면 `reserve` 또는 고정 크기 컨테이너(`std::array`)를 쓰는 게 좋습니다.
- `std::fixed`와 `std::setprecision(1)`은 `<iomanip>`에 있습니다. `std::cout`의 부동소수점 출력 형식을 제어합니다.
- duty=0인 경우 모두 L, duty=100인 경우 모두 H가 나오는지 확인하세요. 경계값 테스트는 임베디드 코드에서 매우 중요합니다.
- 실제 PWM 하드웨어 레지스터(예: STM32의 `TIM_CCR1`)에 직접 듀티 값을 쓰는 코드는 다음 학습 단계(`volatile` 포인터)에서 다룹니다.
- `static_cast<double>(totalH)`로 명시 변환을 한 이유: `(100.0 * totalH / (totalH + totalL))`로 그냥 써도 동작하지만, 정수 오버플로우/타입 변환 시점을 코드로 명시하는 것이 안전합니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `for (int i = 0; i <= n; ++i)` (`<=` 사용) | off-by-one |
| `i++`를 무의식적으로 사용 | 내장 타입은 무해, 사용자 타입은 비용 발생 |
| 반복 변수 `int`로 `size_t`와 비교 | 부호 비교 경고 |
| range-based for에서 값 복사 | 큰 객체에서 비용 발생 — `const auto&` 사용 |
| `do-while` 세미콜론 누락 | 컴파일 에러 |
| ISR과 공유하는 flag를 `bool`로 두고 `while`에서 폴링 | 최적화로 루프 제거되거나 데이터 race |
