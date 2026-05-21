# Day 09 — 연습 문제 (다형성)

---

## 문제 1 (쉬움) — 추상 `Shape` + `Triangle`

다음 추상 클래스 `Shape`와 두 derived `Triangle`, `Square`를 작성하세요.

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual const char* name() const = 0;
    virtual ~Shape() = default;
};
```

- `Triangle(double base, double height)` : 넓이 = `0.5 * base * height`, name = `"Triangle"`
- `Square(double side)` : 넓이 = `side * side`, name = `"Square"`

main 시나리오:
- `Triangle t(3.0, 4.0);`
- `Square sq(5.0);`
- `Shape* shapes[] = { &t, &sq };`
- 각 shape에 대해 `name()`과 `area()` 출력

예시 출력:
```
Triangle: 6
Square: 25
```

---

## 문제 2 (중간) — `override` 함정 잡기

다음 코드는 컴파일은 되지만 **의도와 다르게 동작**합니다. 무엇이 잘못됐는지 진단하고, `override` 키워드를 추가하여 컴파일러가 잡도록 수정하세요. 수정 후 정상 동작하는 코드까지 제출하세요.

```cpp
#include <iostream>

class Logger {
public:
    virtual void log(const std::string& msg) const {
        std::cout << "[LOG] " << msg << '\n';
    }
    virtual ~Logger() = default;
};

class FileLogger : public Logger {
public:
    // 의도: log를 오버라이드해서 파일 prefix를 붙이고 싶음
    void log(std::string msg) {   // <-- 어딘가 잘못되어 있음
        std::cout << "[FILE] " << msg << '\n';
    }
};

int main() {
    FileLogger fl;
    Logger* p = &fl;
    p->log("hello");  // 어떤 함수가 호출될까?
    return 0;
}
```

요구사항:
- 잘못된 부분을 **두 군데** 찾아 명시
- `override` 적용
- 수정 후 출력 결과를 `[FILE] hello`로 만들기

---

## 문제 3 (임베디드 응용) — `Sensor` 인터페이스 다형성

다음 추상 인터페이스를 작성하고 두 derived를 만드세요.

```cpp
class ISensor {
public:
    virtual bool init() = 0;        // 초기화
    virtual double read() = 0;       // 측정값 반환
    virtual const char* type() const = 0;
    virtual ~ISensor() = default;
};
```

derived:
- `class FakeTempSensor : public ISensor` — `read()`는 25.0 + (호출 횟수) (호출마다 1씩 증가)
- `class FakePressureSensor : public ISensor` — `read()`는 100.0 - (호출 횟수 * 0.5)

요구사항:
- `init()`은 둘 다 항상 true 반환하며 cout으로 "[<type>] init OK" 출력
- main에서 다음을 수행:
  - `std::vector<std::unique_ptr<ISensor>> sensors;`
  - 두 derived 객체를 `make_unique`로 넣음
  - 모든 센서에 대해 init 호출
  - 모든 센서를 3번씩 read하여 출력 형식: `[<type>] sample <i>: <value>`
- 가상 소멸자 검증을 위해 derived 소멸자에 `std::cout << "[<type>] dtor\n";`를 추가하고 sensors가 소멸할 때 두 derived의 dtor가 모두 호출되는지 확인

예시 출력 (요지):
```
[Temp] init OK
[Pressure] init OK
[Temp] sample 1: 26
[Temp] sample 2: 27
[Temp] sample 3: 28
[Pressure] sample 1: 99.5
[Pressure] sample 2: 99
[Pressure] sample 3: 98.5
[Pressure] dtor
[Temp] dtor
```

(소멸 순서는 push_back된 역순. 두 derived의 dtor가 호출되어야 가상 소멸자가 올바로 동작하는 것입니다.)

> 힌트: 임베디드 환경에서는 `std::unique_ptr` 대신 정적 객체와 raw pointer 배열을 쓰는 경우가 더 흔하지만, 본 문제는 학습 목적으로 unique_ptr을 사용합니다. 핵심은 **base 포인터로 derived의 멤버 함수가 정확히 호출되는지**입니다.
