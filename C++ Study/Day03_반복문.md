# Day 03 — 반복문

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

반복문은 코드 블록을 조건이 참인 동안 반복 실행합니다. C++의 반복문은 네 종류입니다: `for`, `while`, `do-while`, 그리고 **range-based for**(범위 기반 for, C++11~). 임베디드 코드에서 가장 자주 보는 패턴은 main 함수의 무한 루프 `while (true) { ... }`입니다.

---

## [개념 설명]

### 1. `while`

```cpp
while (condition) {
    // condition이 true인 동안 반복
}
```

- 조건을 **먼저 평가**합니다. 처음부터 false면 본문이 한 번도 실행되지 않습니다.

### 2. `do-while`

```cpp
do {
    // 본문
} while (condition);
```

- 본문을 **먼저 실행**한 뒤 조건을 평가합니다. **최소 한 번은 실행**됩니다.
- 임베디드에서 "최소 한 번 실행 후 재시도" 패턴(예: 통신 재시도)에 사용됩니다.

### 3. 전통적 `for`

```cpp
for (int i = 0; i < n; ++i) {
    // ...
}
```

- 세 부분: **초기화 / 조건 / 증감**.
- 초기화 변수 `i`의 스코프는 for 본문에 한정됩니다.
- `++i`와 `i++`의 차이: 내장 타입(`int` 등)에서는 동일하지만, 사용자 정의 타입(iterator 등)에서는 후위가 임시 객체를 만들어 비용이 더 큽니다. **기본은 `++i`**를 사용하세요.

### 4. Range-based `for` (C++11~)

```cpp
int arr[] = {1, 2, 3, 4, 5};
for (int v : arr) {
    std::cout << v << ' ';
}
```

- 컨테이너의 모든 원소를 순회.
- **참조로 받으면 복사 비용 없음**: `for (const auto& v : arr)`.
- 원소 수정이 필요하면 `for (auto& v : arr)`.

| 형태 | 의미 |
|------|------|
| `for (T v : c)` | 값 복사 |
| `for (T& v : c)` | 참조, 원본 수정 가능 |
| `for (const T& v : c)` | 읽기 전용 참조, 복사 없음 (권장) |
| `for (auto v : c)` | 타입 추론 + 값 복사 |
| `for (const auto& v : c)` | 타입 추론 + 읽기 전용 참조 (가장 흔함) |

### 5. `break` / `continue`

- `break` : 현재 반복문을 **즉시 종료**, 가장 가까운 한 단계만 빠져나옴.
- `continue` : 본문 나머지를 건너뛰고 **다음 반복으로**.
- 중첩 루프에서 한 번에 빠져나오려면 플래그 변수나 함수로 분리해 `return`하는 방법을 사용하세요. C++에는 `goto`가 있지만 학습 단계에서는 사용 금지.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <array>

int main() {
    // 1. while: 0이 입력될 때까지 합산
    int sum = 0;
    int x = 0;
    std::cout << "Enter integers (0 to stop):\n";
    while (true) {
        std::cin >> x;
        if (x == 0) break;
        if (x < 0) {
            std::cerr << "[skip] negative: " << x << '\n';
            continue;
        }
        sum += x;
    }
    std::cout << "Sum of positives = " << sum << '\n';

    // 2. for: 구구단 한 줄
    int n = 7;
    for (int i = 1; i <= 9; ++i) {
        std::cout << n << " * " << i << " = " << (n * i) << '\n';
    }

    // 3. do-while: 사용자 응답을 받을 때까지 반복
    char ans = '\0';
    do {
        std::cout << "Continue? (y/n): ";
        std::cin >> ans;
    } while (ans != 'y' && ans != 'n');
    std::cout << "Answered: " << ans << '\n';

    // 4. range-based for
    std::array<int, 5> samples = {12, 7, 23, 19, 5};
    int total = 0;
    for (const auto& v : samples) {
        total += v;
    }
    std::cout << "Total = " << total << '\n';
    return 0;
}
```

---

## [실행 방법]

```bash
clang++ -std=c++17 -Wall -Wextra -Wpedantic main.cpp -o main
./main
```

---

## [실행 결과]

```
Enter integers (0 to stop):
3 -1 5 7 0
[skip] negative: -1
Sum of positives = 15
7 * 1 = 7
7 * 2 = 14
... (생략)
7 * 9 = 63
Continue? (y/n): k
Continue? (y/n): y
Answered: y
Total = 66
```

---

## [동작 원리]

1. `while (true) { ... if (cond) break; }` 형태의 **무한 루프 + break**는 종료 조건을 본문 어디서든 검사할 수 있어 유연합니다. 임베디드 main 루프의 표준 형태입니다.
2. `continue`는 for/while/do-while의 **조건 검사 단계로 점프**합니다. for의 경우 증감 표현식(`++i`)도 실행됩니다.
3. range-based for는 컴파일러가 다음과 같이 풀어 씁니다(개념적으로).
   ```cpp
   for (auto __it = std::begin(c); __it != std::end(c); ++__it) {
       auto&& v = *__it;
       // 본문
   }
   ```
   따라서 `c`는 `begin()`/`end()`를 가진 컨테이너여야 합니다. `std::array`, `std::vector`, C-style 배열 모두 사용 가능합니다. **포인터로 decay된 배열은 사용 불가** (크기 정보가 사라지므로).

---

## [임베디드 관점]

- **Super-loop 패턴**: RTOS 없이 동작하는 단순 펌웨어의 main은 보통 다음 형태입니다.
  ```cpp
  int main() {
      init_hw();
      while (true) {
          read_sensors();
          process();
          drive_actuators();
          delay_ms(10);
      }
  }
  ```
- **busy-wait 주의**: `while (!flag) {}` 같은 폴링은 CPU를 100% 점유합니다. RTOS 환경에서는 세마포어/큐 등 블로킹 API를 사용하세요. ISR과 main loop 사이의 플래그라면 **`std::atomic` 또는 `volatile`** 한정자가 필수입니다 (Day는 별도, 추후 학습).
- **반복 횟수 결정성**: 실시간 task는 **WCET(Worst-Case Execution Time)**를 알아야 합니다. 입력에 따라 반복 횟수가 변하는 `while`보다, 상한이 있는 `for (int i = 0; i < MAX; ++i)` 형태가 분석에 유리합니다.
- **DMA/하드웨어 가속**: 큰 데이터를 순회하는 루프(예: 1000개 ADC 샘플)는 가능하면 CPU 루프 대신 DMA로 처리하세요. C++ 루프는 코드는 짧아도 인터럽트 latency를 늘릴 수 있습니다.
- **루프 unrolling**: 컴파일러가 `-O2` 이상에서 자동으로 풀어주므로 손으로 unroll하지 않는 것을 권장합니다. 측정 없는 수동 unroll은 코드 크기만 늘릴 수 있습니다.

---

## [주의할 점]

1. **off-by-one 에러**: `i <= n`과 `i < n`은 한 번 차이입니다. 배열 인덱싱에서 `<`를 사용하는 것이 표준 관행입니다.
2. **부호 있는/없는 비교**: `for (int i = 0; i < vec.size(); ++i)`는 경고가 발생합니다 (`vec.size()`는 `size_t`). `for (std::size_t i = 0; i < vec.size(); ++i)` 또는 range-based for를 사용하세요.
3. **range-based for 중 컨테이너 수정 금지**: 순회 중에 원소를 `push_back`/`erase` 하면 iterator가 무효화되어 UB. 인덱스 기반 루프를 사용하거나 별도 수집 후 일괄 수정하세요.
4. **`do-while` 세미콜론**: `} while (cond);` 끝의 세미콜론 누락이 자주 발생합니다.
5. **무한 루프와 `volatile`**: `while (running) {}`에서 `running`이 ISR에서 바뀐다면 컴파일러가 루프를 최적화로 제거할 수 있습니다. `std::atomic<bool>` 또는 `volatile bool`(C 호환 코드)이 필요합니다.
6. **참조 캡처 누락**: `for (auto v : huge_vector)`에서 `auto v`는 **복사**입니다. 큰 객체라면 `for (const auto& v : huge_vector)`로 작성해 복사 비용을 없애세요.

---

## [연습 문제]

- 문제: [`Day03_problems.md`](./Day03_problems.md)
- 정답: [`Day03_solutions.md`](./Day03_solutions.md)
