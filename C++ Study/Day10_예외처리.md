# Day 10 — 예외 처리 (try / catch / throw)

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

C++의 예외 처리는 `throw`로 오류 상황을 신호하고, `try` 블록 안에서 발생한 예외를 `catch`로 받아 처리합니다. 표준 예외는 `<stdexcept>`와 `<exception>`에 정의된 계층 구조이며, **모든 표준 예외의 최종 base는 `std::exception`**입니다. `std::runtime_error`는 가장 흔히 사용하는 예외 클래스 중 하나입니다.

> **임베디드 관점**: 많은 임베디드 프로젝트는 `-fno-exceptions`로 예외 메커니즘 자체를 비활성화합니다. 예외 처리에는 추가 메모리(stack unwinding 정보)와 런타임 비용이 따르기 때문입니다. 대안으로 **반환 코드(`enum`)** 또는 C++23의 `std::expected`(C++17에서는 `std::optional` + 오류 코드)를 사용합니다.

---

## [개념 설명]

### 1. 기본 문법

```cpp
#include <stdexcept>

void divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("division by zero");
    }
    std::cout << (a / b) << '\n';
}

int main() {
    try {
        divide(10, 0);
    } catch (const std::runtime_error& e) {
        std::cerr << "runtime_error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "exception: " << e.what() << '\n';
    } catch (...) {
        std::cerr << "unknown exception\n";
    }
}
```

### 2. 표준 예외 계층 (간략)

```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   └── std::out_of_range
└── std::runtime_error
    ├── std::range_error
    ├── std::overflow_error
    └── std::underflow_error
```

- `std::logic_error` : **프로그램의 논리 오류** (사전에 검사 가능한 것). 예: 잘못된 인자 (`invalid_argument`).
- `std::runtime_error` : **실행 중에만 알 수 있는 오류** (외부 요인). 예: 파일 열기 실패, 통신 오류.

이 구분은 **사용자에게 시그널링하는 의미**에 가깝습니다. 컴파일러가 강제하지는 않습니다.

### 3. `throw`로 던지기

```cpp
throw std::runtime_error("sensor disconnected");
throw std::invalid_argument("value must be positive");
throw std::out_of_range("index out of range");
```

- 예외 객체는 임시 객체로 생성되어 던져집니다.
- 던져진 객체는 catch 블록의 매개변수에 **참조나 값으로** 받습니다. **거의 항상 `const ReferenceType&`**로 받는 것이 표준 관행입니다 (슬라이싱 회피).

### 4. catch 매칭 규칙

```cpp
try {
    // ...
} catch (const std::out_of_range& e) {
    // 가장 구체적인 예외부터 위에
} catch (const std::logic_error& e) {
    // out_of_range는 logic_error의 derived
} catch (const std::exception& e) {
    // base
} catch (...) {
    // 모든 예외 (마지막 안전망)
}
```

- catch는 **위에서부터 매칭**됩니다. 더 구체적인 타입을 위에 두지 않으면 derived가 base에 잡혀버립니다.
- `catch (...)`는 타입에 무관한 catch-all. 디버깅용 외엔 가급적 피하세요(예외 정보가 사라짐).

### 5. 예외 전파(re-throw)

```cpp
try {
    // ...
} catch (const std::exception& e) {
    log("error: " + std::string(e.what()));
    throw;     // 같은 예외를 그대로 재전파
}
```

- `throw;` (매개변수 없음): 현재 잡은 예외를 다시 던집니다. catch 안에서만 유효.
- `throw e;` (변수 사용): **슬라이싱이 발생할 수 있음** — `e`의 정적 타입이 base이면 base 타입 객체가 던져짐. `throw;`를 사용하세요.

### 6. 스택 풀기(stack unwinding)

```cpp
class Resource {
public:
    Resource()  { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }
};

void f() {
    Resource r;             // 스택에 자동 객체
    throw std::runtime_error("oops");
    // r은 unwinding 중에 소멸자가 호출됨 → "Resource released" 출력
}

int main() {
    try { f(); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; }
}
```

- 예외가 던져지면 catch에 도달할 때까지 **스택의 모든 지역 객체가 역순으로 소멸**됩니다.
- 이것이 **RAII(Resource Acquisition Is Initialization)**가 강력한 이유입니다. 동적 자원을 `std::unique_ptr`/`std::lock_guard`/소멸자에서 정리하는 객체에 담아두면 예외 발생 시에도 자원이 자동 해제됩니다.

### 7. `noexcept` 키워드

```cpp
double safe_compute() noexcept {
    // 이 함수는 예외를 던지지 않음을 약속
    return 42.0;
}
```

- `noexcept`는 함수가 예외를 던지지 않음을 명시. 컴파일러가 최적화 기회로 활용.
- 단, `noexcept` 함수에서 예외가 던져지면 `std::terminate`가 호출됩니다 (catch 전에).
- **소멸자, 이동 생성자, swap**은 가급적 `noexcept`로 선언하는 것이 권장됩니다.

### 8. 사용자 정의 예외 클래스

```cpp
class SensorError : public std::runtime_error {
public:
    SensorError(int sensor_id, const std::string& msg)
        : std::runtime_error(msg), id_(sensor_id) {}

    int sensor_id() const { return id_; }

private:
    int id_;
};

throw SensorError(3, "communication timeout");

try {
    // ...
} catch (const SensorError& e) {
    std::cerr << "[Sensor " << e.sensor_id() << "] " << e.what() << '\n';
}
```

- `std::runtime_error` 등을 상속하여 도메인 특화 정보를 추가.
- `what()`은 `std::exception`의 가상 함수로, 메시지를 반환합니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// 사용자 정의 예외 클래스
class SensorError : public std::runtime_error {
public:
    SensorError(int sensor_id, const std::string& msg)
        : std::runtime_error(msg), id_(sensor_id) {}

    int sensor_id() const noexcept { return id_; }

private:
    int id_;
};

class Sensor {
public:
    Sensor(int id) : id_(id) {}

    // 임계 범위 검사: 유효 범위 외면 throw
    double read(double raw) {
        if (raw < -50.0 || raw > 200.0) {
            throw std::out_of_range(
                "raw out of valid range: " + std::to_string(raw));
        }
        if (id_ < 0) {
            throw SensorError(id_, "invalid sensor id");
        }
        return raw * 1.0;  // identity for demo
    }

private:
    int id_;
};

int main() {
    Sensor s(1);

    // 예외 처리: 다양한 시나리오
    std::vector<double> samples = {25.0, 100.0, 300.0, -100.0};
    int idx = 0;
    for (double raw : samples) {
        try {
            double v = s.read(raw);
            std::cout << "sample[" << idx << "] = " << v << '\n';
        } catch (const std::out_of_range& e) {
            std::cerr << "[OOR] " << e.what() << '\n';
        } catch (const SensorError& e) {
            std::cerr << "[Sensor " << e.sensor_id()
                      << "] " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cerr << "[Unknown] " << e.what() << '\n';
        }
        ++idx;
    }

    // RAII + 예외 안전성 확인
    struct Resource {
        std::string name;
        Resource(std::string n) : name(std::move(n)) {
            std::cout << "[acquire] " << name << '\n';
        }
        ~Resource() {
            std::cout << "[release] " << name << '\n';
        }
    };

    try {
        Resource a("A");
        Resource b("B");
        throw std::runtime_error("inner failure");
        // 도달 불가
    } catch (const std::exception& e) {
        std::cerr << "caught: " << e.what() << '\n';
    }

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
sample[0] = 25
sample[1] = 100
[OOR] raw out of valid range: 300.000000
[OOR] raw out of valid range: -100.000000
[acquire] A
[acquire] B
[release] B
[release] A
caught: inner failure
```

---

## [동작 원리]

1. `throw std::out_of_range(...)` 호출 시:
   - `std::out_of_range` 임시 객체가 만들어짐.
   - 스택 풀기 시작: 현재 함수의 지역 객체들이 역순으로 소멸자 호출.
   - 호출 스택을 거슬러 올라가며 일치하는 `catch`를 찾음.
2. **catch 매칭**: `std::out_of_range`는 `std::logic_error`의 derived이고 `std::logic_error`는 `std::exception`의 derived. 위에서부터 매칭하므로 `catch (const std::out_of_range&)`가 먼저 잡습니다.
3. **RAII 검증**: `Resource a("A"); Resource b("B"); throw ...` 시 스택 풀기는 LIFO 순서. 따라서 `b` 먼저, `a` 나중에 소멸자가 호출됩니다.
4. `e.what()`은 `std::exception`의 가상 함수. derived 클래스에서 메시지를 저장하고 `what()`이 그 메시지를 반환합니다. `std::runtime_error`는 생성자 인자로 받은 메시지를 내부에 보관.

---

## [임베디드 관점]

- **예외 비활성화(`-fno-exceptions`) 정책**:
  - **장점**: 바이너리 크기 감소(예외 메타데이터 제거), 런타임 비용 제거, 결정성 향상.
  - **단점**: 표준 라이브러리 일부가 동작 제한. 예를 들어 `std::vector::at`는 예외 의존, `new`는 실패 시 예외 던짐(`std::nothrow` 사용 필요).
- **예외 대안: 반환 코드**
  ```cpp
  enum class SensorStatus { Ok, OutOfRange, CommTimeout, NotInitialized };

  SensorStatus read_sensor(double& out);
  ```
- **C++17 `std::optional`**: 값이 있을 수도/없을 수도 있는 함수 반환에 유용.
  ```cpp
  std::optional<double> read_sensor();  // nullopt이면 실패
  ```
- **C++23 `std::expected<T, E>`**: 값 또는 에러를 명시적으로 표현. 임베디드 친화적. (현재 C++17 학습 단계라 직접 구현하거나 `tl::expected` 등의 라이브러리 사용)
- **예외 사용 가능한 경우의 비용**:
  - **0-cost exception model** (Itanium ABI, GCC/Clang 기본): 예외가 던져지지 않는 경로에는 비용 없음. 예외가 던져지면 상당히 큰 비용(stack unwinding, table lookup).
  - 메모리 사용: unwind table이 Flash에 저장됨. 작은 MCU에서는 무시할 수 없는 크기.
- **소멸자에서 예외 던지지 말 것**: 만약 unwinding 중에 다른 예외가 던져지면 `std::terminate`가 즉시 호출됩니다. 임베디드/일반 모두 동일하게 위험.
- **ISR에서 예외 던지지 말 것**: 예외 메커니즘이 ISR 컨텍스트에서 동작하지 않을 수 있고, 동작하더라도 latency가 매우 큽니다.
- **AUTOSAR/MISRA**: 자동차 임베디드 코드에서는 예외 사용을 금지하거나 강하게 제한합니다.

---

## [주의할 점]

1. **`catch`에서 값으로 받지 말 것**: `catch (std::exception e)`는 슬라이싱 발생 + 복사 비용. **`catch (const std::exception& e)`**가 표준.
2. **catch 순서**: 구체적인 derived가 위, base가 아래. 반대면 derived가 base에 의해 잡혀버려 더 좁은 catch가 unreachable.
3. **소멸자에서 예외 던지지 말 것**. `std::terminate` 발생 위험.
4. **`noexcept` 함수에서 예외 던지면 즉시 종료**. 던질 가능성이 있다면 `noexcept`를 붙이지 마세요.
5. **`throw;` vs `throw e;`**: `throw;`는 정확한 동적 타입으로 재전파. `throw e;`는 정적 타입으로 다시 던져 슬라이싱.
6. **예외를 흐름 제어로 사용하지 말 것**. 예외는 **예외적인 상황**을 위한 것. 빈 컨테이너 체크 같은 일상적 분기에 사용하면 성능과 가독성 모두 저하.
7. **로깅과 결합**: catch에서 메시지를 로깅한 뒤 `throw;`로 상위에 전파하는 패턴은 흔하지만, 같은 오류가 여러 번 로그되지 않게 조심.
8. **`std::runtime_error` 메시지의 한정**: 생성자에 전달된 문자열은 보통 std::string으로 보관. **임베디드의 ROM 문자열**과 같이 정적 수명 문자열만 다루고 싶다면 별도 설계 필요.
9. **`assert` vs 예외**: 프로그래머의 실수(전제 조건 위반)는 `assert`(개발 시) 또는 즉시 abort. 외부 입력 오류는 예외 또는 반환 코드. 둘을 혼동하지 마세요.
10. **`-fno-exceptions` 컴파일된 객체와 예외 사용 객체 혼용 금지**: ABI 불일치로 정의되지 않은 동작.

---

## [연습 문제]

- 문제: [`Day10_problems.md`](./Day10_problems.md)
- 정답: [`Day10_solutions.md`](./Day10_solutions.md)
