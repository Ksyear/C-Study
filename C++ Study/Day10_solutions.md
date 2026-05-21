# Day 10 — 연습 문제 정답 (예외 처리)

---

## 문제 1 정답

```cpp
#include <iostream>
#include <stdexcept>

int safe_div(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("division by zero");
    }
    return a / b;
}

int main() {
    int a = 0, b = 0;
    std::cout << "Enter a b: ";
    std::cin >> a >> b;

    try {
        int r = safe_div(a, b);
        std::cout << "result = " << r << '\n';
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] " << e.what() << '\n';
        return 1;
    }
    return 0;
}
```

**해설**
- `safe_div` 자체는 0 검사를 하므로 UB가 발생하지 않습니다. 호출자에게 실패를 알리는 방법으로 예외를 사용했습니다.
- 호출 측은 try/catch로 처리. 본 풀이는 단일 catch이지만, 더 일반적인 `catch (const std::exception&)`로도 잡힙니다(`runtime_error`가 derived이므로).
- 임베디드라면 반환 코드 패턴이 더 자연스럽습니다. 예: `bool safe_div(int a, int b, int& out)`.

---

## 문제 2 정답

```cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class TempOutOfRange : public std::out_of_range {
public:
    TempOutOfRange(double value)
        : std::out_of_range("temperature out of range: " + std::to_string(value)),
          value_(value) {}

    double value() const noexcept { return value_; }

private:
    double value_;
};

double process_temp(double c) {
    if (c < -50.0 || c > 200.0) {
        throw TempOutOfRange(c);
    }
    return c * 1.8 + 32.0;
}

int main() {
    std::vector<double> samples = {25.0, 150.0, -100.0, 300.0, 0.0};
    for (double c : samples) {
        try {
            double f = process_temp(c);
            std::cout << "F = " << f << '\n';
        } catch (const TempOutOfRange& e) {
            // 구체적인 derived가 위쪽
            std::cerr << "[TOR] " << e.value() << " degC out of range\n";
        } catch (const std::exception& e) {
            // base는 아래쪽
            std::cerr << "[EXC] " << e.what() << '\n';
        }
    }
    return 0;
}
```

**해설**
- catch 순서가 중요합니다. `TempOutOfRange`가 `std::out_of_range`의 derived → `std::logic_error`의 derived → `std::exception`의 derived. 가장 구체적인 것을 위에 두지 않으면 base catch가 먼저 잡습니다.
- `std::to_string`은 `<string>` 헤더에 있습니다. `<stdexcept>`에는 없으므로 명시적으로 include 해야 합니다.
- `value()`에 `noexcept`를 붙인 이유: 단순 멤버 반환이라 예외를 던질 일이 없습니다. **getter는 거의 항상 `noexcept`로 표시**하는 것이 안전합니다.
- 임베디드 응용 시, 사용자 정의 예외 클래스는 `id`, `error_code`, `timestamp` 등의 정보를 추가로 담아 진단을 풍부하게 할 수 있습니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

class HwResource {
public:
    HwResource(const std::string& name) : name_(name) {
        std::cout << "[acquire] " << name_ << '\n';
    }

    ~HwResource() {
        std::cout << "[release] " << name_ << '\n';
    }

    void use(bool fail) {
        if (fail) {
            throw std::runtime_error("device fault");
        }
        std::cout << "[use OK] " << name_ << '\n';
    }

private:
    std::string name_;
};

int main() {
    try {
        HwResource a("UART");
        HwResource b("SPI");
        HwResource c("I2C");

        a.use(false);
        b.use(false);
        c.use(true);   // throw

        std::cout << "this line should NOT be reached\n";
    } catch (const std::exception& e) {
        std::cout << "[caught] " << e.what() << '\n';
    }
    return 0;
}
```

**예상 출력**:
```
[acquire] UART
[acquire] SPI
[acquire] I2C
[use OK] UART
[use OK] SPI
[release] I2C
[release] SPI
[release] UART
[caught] device fault
```

**해설 / 임베디드 관점**
- `c.use(true)`에서 던진 예외는 try 블록을 빠져나가기 위해 **스택 풀기**를 수행합니다.
- 스택 위에 있는 객체부터 역순으로 소멸자 호출 → `c`, `b`, `a` 순으로 release.
- 만약 `HwResource`가 동적 자원(예: 파일 핸들, 동적 메모리, 뮤텍스 락)을 담고 있었다면 모두 정확히 해제됩니다. 이것이 **RAII가 예외 안전성의 핵심**인 이유입니다.
- 임베디드에서 동등한 패턴:
  - **lock_guard** 스타일: 함수 진입 시 인터럽트 비활성화, 소멸자에서 복원.
  - **scope guard**: 자원 획득과 해제를 한 쌍으로 묶음.
  - 예외가 비활성화되었어도 RAII는 동작합니다. early return으로 함수가 종료되면 마찬가지로 소멸자가 호출되어 자원이 해제됩니다.
- 만약 **생성자가 예외를 던지면** 해당 객체의 소멸자는 호출되지 않습니다(객체가 완전히 만들어지지 않았으므로). 단, 그 시점까지 이미 만들어진 멤버 변수의 소멸자는 호출됩니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `catch (std::exception e)` 값 받음 | 슬라이싱, 복사 비용 |
| catch 순서가 base가 위 | derived catch 도달 불가 |
| `throw e;` (변수 사용) | 슬라이싱 |
| 소멸자에서 예외 던짐 | `std::terminate` |
| `noexcept` 함수에서 예외 던짐 | `std::terminate` |
| 예외를 흐름 제어로 사용 | 성능/가독성 저하 |
| catch (...) 만 사용 | 예외 정보 손실 |
| 생성자에서 throw 후 dtor 호출 기대 | 객체가 완성되지 않아 dtor 미호출 |
| ISR 또는 -fno-exceptions 환경에서 throw | UB 또는 즉시 종료 |
| RAII 없는 코드에 예외 도입 | 자원 누수, 데드락 |

---

## 학습 마무리

이 10일 과정으로 다음 흐름이 자연스럽게 연결됩니다:

1. **입출력 → 조건문 → 반복문 → 배열 → 함수** : 절차적 C++의 토대
2. **class → 접근 제어 → 캡슐화** : 객체 지향의 기본
3. **생성자 → getter/setter** : 객체의 불변식 보장
4. **상속 → protected** : 재사용을 위한 기반
5. **다형성 (virtual/override/추상)** : 런타임 유연성
6. **예외 처리** : 오류 흐름 관리, RAII와의 연결

다음 학습으로 추천:
- **RAII와 스마트 포인터** (`std::unique_ptr`, `std::shared_ptr`)
- **복사/이동 의미론** (Rule of Five)
- **`const`, `constexpr`, `static`의 정확한 의미**
- **템플릿 기초** + STL 알고리즘
- **`enum class`, 비트 연산, 구조체 패킹과 정렬**
- **임베디드 핵심**: `volatile`, `std::atomic`, ISR, 메모리 모델
- **멀티스레딩 기초** (`std::thread`, `std::mutex`, `std::condition_variable`)
- **CMake 기초** + 파일 분할 + 헤더 가드/`#pragma once`

질문이 있으면 "문제 내줘", "정답 확인해줘", "임베디드 관점으로 다시", "면접식으로 물어봐" 같은 명령으로 학습을 이어가세요.
