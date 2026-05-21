# Day 05 — 함수

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

함수는 코드를 재사용 가능한 단위로 묶는 가장 기본 도구입니다. C++ 함수의 핵심 학습 포인트는 다음과 같습니다.

1. **선언(declaration)**과 **정의(definition)**의 분리
2. **매개변수 전달 방식**: 값 / 참조 / `const` 참조 / 포인터
3. **함수 오버로딩(overloading)**: 같은 이름, 다른 매개변수 시그니처
4. **기본 인자(default arguments)**
5. **`inline`** 키워드와 헤더에 함수 정의 시 주의점

---

## [개념 설명]

### 1. 선언 vs 정의

```cpp
// 선언 (declaration) — 함수가 존재함을 알림
int add(int a, int b);

// 정의 (definition) — 실제 본문
int add(int a, int b) {
    return a + b;
}
```

- 헤더 파일(.hpp)에는 **선언**, 소스 파일(.cpp)에는 **정의**가 들어가는 것이 일반적입니다.
- 함수는 호출 전에 **선언**이 가시 범위 안에 있어야 합니다. 그래야 컴파일러가 인자 타입을 검사할 수 있습니다.

### 2. 매개변수 전달 방식

| 방식 | 문법 | 비용 | 원본 수정 가능 |
|------|------|------|----------------|
| 값 전달 (pass by value) | `f(int x)` | 복사 발생 | 불가 (사본만 수정) |
| 참조 전달 (pass by reference) | `f(int& x)` | 복사 없음 | 가능 |
| `const` 참조 | `f(const int& x)` | 복사 없음 | 불가 |
| 포인터 전달 | `f(int* p)` | 복사 없음 (포인터 자체는 복사) | 가능 (nullptr 가능성) |

**원칙**:
- 작은 기본 타입(`int`, `double`, `bool`): 값 전달
- 큰 객체(`std::string`, `std::vector`, 사용자 클래스): `const T&` 전달 (수정 안 함) 또는 `T&` (수정 함)
- "옵셔널" 의미를 표현하려면 포인터(`T*`) — `nullptr`로 부재 표현. 단, C++17에서는 `std::optional<T>`가 더 표현력 있음.

### 3. 참조자(reference)

```cpp
void increment(int& x) {
    ++x;
}

int v = 10;
increment(v);  // v는 이제 11
```

- 참조자는 **별명(alias)**입니다. 포인터처럼 `nullptr`이 될 수 없고, 한 번 바인딩되면 다른 변수로 다시 바인딩할 수 없습니다.
- `const T&`는 "이 함수는 인자를 읽기만 한다"는 계약을 코드로 명시합니다.

### 4. 함수 오버로딩

```cpp
int add(int a, int b)         { return a + b; }
double add(double a, double b) { return a + b; }
int add(int a, int b, int c)  { return a + b + c; }
```

- 같은 이름 + 다른 시그니처(매개변수 타입 또는 개수). **반환 타입만 다른 것은 오버로딩 불가**.
- 컴파일 시 인자 타입으로 적절한 오버로드가 선택됩니다 (overload resolution).

### 5. 기본 인자

```cpp
void log(const std::string& msg, int level = 1) {
    // ...
}
log("hello");    // level=1
log("err", 3);   // level=3
```

- **기본값은 선언부에만 적습니다**(정의부에 또 적으면 컴파일 에러).
- 오른쪽부터만 가능 (`f(int a = 1, int b)` 불가).

### 6. `inline`과 헤더에 함수 정의

```cpp
// utils.hpp
inline int square(int x) { return x * x; }
```

- 헤더에 함수 **정의**를 두려면 `inline` 또는 `static`을 붙여야 합니다. 그렇지 않으면 여러 .cpp가 같은 헤더를 include할 때 **ODR(One Definition Rule) 위반**으로 링커 에러.
- `inline`은 "동일한 정의를 여러 번 봐도 OK"라는 의미이지, 강제 인라인 확장이 아닙니다. 인라인 확장은 컴파일러가 판단.

### 7. `[[nodiscard]]` (C++17)

```cpp
[[nodiscard]] int read_sensor() { /* ... */ }

read_sensor();  // 경고: 반환값 무시
```

- 반환값을 무시하면 컴파일러가 경고를 냅니다. 오류 코드를 반환하는 함수에 매우 유용합니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <string>
#include <vector>

// 1. 선언만 (정의는 아래)
double celsius_to_fahrenheit(double c);

// 2. 값 전달
int square(int x) {
    return x * x;
}

// 3. 참조 전달 (원본 수정)
void swap_int(int& a, int& b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// 4. const 참조 (큰 객체 읽기 전용)
double sum(const std::vector<double>& v) {
    double s = 0.0;
    for (const auto& x : v) {
        s += x;
    }
    return s;
}

// 5. 오버로딩
int add(int a, int b)             { return a + b; }
double add(double a, double b)    { return a + b; }
int add(int a, int b, int c)      { return a + b + c; }

// 6. 기본 인자 + [[nodiscard]]
[[nodiscard]] bool check_range(int v, int lo = 0, int hi = 100) {
    return v >= lo && v <= hi;
}

int main() {
    std::cout << "square(5) = " << square(5) << '\n';

    int a = 1, b = 2;
    swap_int(a, b);
    std::cout << "after swap: a=" << a << ", b=" << b << '\n';

    std::vector<double> data = {1.5, 2.5, 3.0};
    std::cout << "sum = " << sum(data) << '\n';

    std::cout << "add(1,2) = "       << add(1, 2)        << '\n';
    std::cout << "add(1.5,2.5) = "   << add(1.5, 2.5)    << '\n';
    std::cout << "add(1,2,3) = "     << add(1, 2, 3)     << '\n';

    std::cout << "check(50) = " << std::boolalpha << check_range(50) << '\n';
    std::cout << "check(150,0,200) = " << check_range(150, 0, 200) << '\n';

    std::cout << "100C = " << celsius_to_fahrenheit(100.0) << "F\n";
    return 0;
}

// 함수 정의 (선언이 위쪽에 있으므로 가능)
double celsius_to_fahrenheit(double c) {
    return c * 9.0 / 5.0 + 32.0;
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
square(5) = 25
after swap: a=2, b=1
sum = 7
add(1,2) = 3
add(1.5,2.5) = 4
add(1,2,3) = 6
check(50) = true
check(150,0,200) = true
100C = 212F
```

---

## [동작 원리]

1. **값 전달**: `square(5)` 호출 시 `5`가 매개변수 `x`로 **복사**됩니다. `int`처럼 작은 타입은 비용이 무시할 수준입니다.
2. **참조 전달**: `swap_int(a, b)` 호출 시 `a`와 `b`의 **주소(별명)**가 전달됩니다. 함수 내부에서 `a`를 수정하면 호출자의 변수가 직접 수정됩니다. 어셈블리 수준에서는 포인터와 거의 같지만, 컴파일러는 참조자에 대해 `nullptr` 가능성을 가정하지 않을 수 있어 최적화 여지가 더 큽니다.
3. **오버로딩 해석**: `add(1, 2)`는 `int, int` 시그니처와 정확히 일치. `add(1.5, 2.5)`는 `double, double`과 일치. 일치하는 게 둘 이상이거나 모호하면 컴파일 에러.
4. **기본 인자**: `check_range(50)`은 컴파일러가 `check_range(50, 0, 100)`으로 풀어 씁니다. 디폴트는 컴파일 타임에 결정되므로 함수 내부에서 결정되는 게 아닙니다.
5. **`[[nodiscard]]`**: 컴파일러는 호출 사이트에서 반환값이 사용되지 않는지를 검사해 경고를 냅니다.

---

## [임베디드 관점]

- **함수 호출 비용**: 함수 호출은 스택 프레임 생성, 인자 전달, 반환 주소 push 등의 비용이 있습니다. ISR이나 매우 짧은 핫 루프에서는 비용이 될 수 있어 `inline` 함수 또는 매크로(가능하면 피할 것)를 사용합니다. `inline`은 컴파일러에게 힌트일 뿐 강제는 아니므로, 강제하려면 GCC/Clang 확장 `__attribute__((always_inline))`을 사용합니다.
- **재진입(reentrancy)**: ISR에서 호출되는 함수는 **재진입 가능(reentrant)**해야 합니다. 정적 지역 변수나 전역 상태를 수정하면 재진입 안전성이 깨집니다. 가능한 한 매개변수와 지역변수만 사용하세요.
- **참조 vs 포인터**: 임베디드 C에서 익숙한 포인터 대신 C++ 참조를 쓰면 `nullptr` 가능성을 컴파일 타임에 배제할 수 있어 안전합니다. 단, **C와 인터페이스**할 때는 `extern "C"`와 포인터를 사용해야 합니다.
- **큰 객체 반환**: C++17의 **NRVO(Named Return Value Optimization)** 덕분에 큰 객체를 반환해도 보통 복사가 일어나지 않습니다. 그래도 임베디드에서는 출력 매개변수(`T& out`)나 `std::array` 반환을 선호하는 경향이 있습니다.
- **함수 포인터 vs `std::function`**: 임베디드에서 콜백을 등록할 때는 **함수 포인터**(`void (*)(int)`)가 가장 가볍습니다. `std::function`은 type erasure로 인해 작은 힙 할당이 발생할 수 있어 제약 환경에서는 피하는 경우가 많습니다.
- **WCET 분석**: 함수가 분기/재귀를 많이 포함하면 WCET 분석이 어려워집니다. 실시간 task에 호출되는 함수는 분기를 단순화하고 재귀를 피하세요.

---

## [주의할 점]

1. **`const` 누락**: `void f(std::string s)`(값 전달, 매번 복사)와 `void f(const std::string& s)`(참조, 복사 없음)은 성능 차이가 큽니다. **읽기 전용이면 `const T&`**를 습관화하세요.
2. **참조 반환의 댕글링**: 지역 변수를 참조로 반환하면 댕글링 참조가 됩니다.
   ```cpp
   int& bad() {
       int x = 5;
       return x;  // x는 함수 종료 시 파괴 — UB
   }
   ```
3. **기본 인자는 헤더에만**: 정의에 또 적으면 컴파일 에러.
4. **오버로딩 모호성**: `int` 인자가 있는 `add(int, int)`와 `double` 인자의 `add(double, double)`가 있을 때 `add(1, 2.0)`은 모호. 명시적 변환 또는 별도 오버로드 필요.
5. **함수 선언 순서**: 호출하려는 함수의 선언이 위쪽에 있어야 합니다. 그렇지 않으면 컴파일 에러 (`implicit function declaration`).
6. **헤더에 비-`inline` 함수 정의**: ODR 위반으로 링커 에러. 헤더에 정의한다면 `inline` 또는 템플릿이어야 함.
7. **재귀 깊이**: 임베디드 MCU의 스택은 보통 수 KB. 깊은 재귀는 스택 오버플로우로 시스템 충돌을 일으킵니다. 가능하면 반복문으로 변환하세요.

---

## [연습 문제]

- 문제: [`Day05_problems.md`](./Day05_problems.md)
- 정답: [`Day05_solutions.md`](./Day05_solutions.md)
