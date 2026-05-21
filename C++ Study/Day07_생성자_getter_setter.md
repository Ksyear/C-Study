# Day 07 — 생성자 · getter / setter

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

**생성자(constructor)**는 객체가 만들어질 때 자동 호출되는 특수 멤버 함수로, 객체의 **불변식(invariant)을 처음부터 보장**하는 역할을 합니다. **멤버 초기화 리스트(member initializer list)**를 통해 멤버 변수를 효율적으로 초기화하며, 가능하면 **대입(`=`)이 아니라 초기화**를 사용해야 합니다.

getter / setter는 private 멤버에 대한 접근 통로이지만, **모든 멤버에 기계적으로 만드는 것은 캡슐화의 의미를 잃게 합니다**. 객체에 일을 시키는 멤버 함수를 우선시하세요.

---

## [개념 설명]

### 1. 생성자 기본 형태

```cpp
class Point {
private:
    double x_;
    double y_;
public:
    // 기본 생성자
    Point() : x_(0.0), y_(0.0) {}

    // 매개변수 생성자
    Point(double x, double y) : x_(x), y_(y) {}
};
```

- 생성자 이름은 **클래스 이름과 동일**, 반환 타입 **없음**.
- `: x_(x), y_(y)` 부분이 **멤버 초기화 리스트**입니다.

### 2. 초기화 리스트 vs 대입

```cpp
// 권장: 초기화
Point(double x, double y) : x_(x), y_(y) {}

// 비권장: 대입 (생성자 본문 안에서)
Point(double x, double y) {
    x_ = x;
    y_ = y;
}
```

차이점:
- 초기화 리스트는 멤버를 **만들 때 곧바로 원하는 값으로** 설정합니다.
- 대입 방식은 멤버를 먼저 기본 초기화한 뒤, 본문에서 값을 다시 대입합니다. (`int` 같은 기본 타입은 차이가 없지만) 사용자 정의 타입은 **두 번 일하는 셈**입니다.
- `const` 멤버나 reference 멤버는 **반드시** 초기화 리스트로 초기화해야 합니다(대입 불가).

### 3. 초기화 순서

```cpp
class A {
private:
    int b_;
    int a_;
public:
    A() : a_(10), b_(a_) {}  // 함정: 멤버 선언 순서대로 초기화됨
};
```

**중요**: 멤버는 초기화 리스트에 적힌 순서가 아니라 **클래스 안 선언 순서대로** 초기화됩니다. 위 예에서 `b_(a_)`가 먼저 실행되는데 그 시점에 `a_`는 아직 초기화되지 않았으므로 UB.

**컴파일러 경고(`-Wreorder`)**가 이를 잡아주므로 항상 활성화하세요. 멤버 초기화 리스트의 순서를 멤버 선언 순서와 일치시키면 안전합니다.

### 4. 다양한 생성자

```cpp
class Date {
public:
    Date();                          // 기본 생성자
    Date(int y, int m, int d);       // 매개변수 생성자
    Date(const Date& other);         // 복사 생성자 (Day는 별도, 추후 학습)
    explicit Date(int yyyymmdd);     // explicit: 묵시적 변환 차단
};
```

- **`explicit`**: 단일 인자 생성자에 붙이면 묵시적 변환을 막습니다.
  ```cpp
  class Temperature {
  public:
      explicit Temperature(double c) : c_(c) {}
  };
  void f(Temperature t);
  f(36.5);              // 컴파일 에러 (explicit 때문)
  f(Temperature{36.5}); // OK
  ```
  단일 인자 생성자는 거의 항상 `explicit`을 붙이는 것이 안전합니다.

### 5. 위임 생성자(C++11~)

```cpp
class Point {
public:
    Point(double x, double y) : x_(x), y_(y) {}
    Point() : Point(0.0, 0.0) {}  // 다른 생성자에 위임
private:
    double x_, y_;
};
```

중복 코드 제거에 유용합니다.

### 6. 기본 인자 vs 여러 생성자

```cpp
// 방법 1: 기본 인자
class Point {
public:
    Point(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}
};

// 방법 2: 여러 생성자 + 위임
class Point {
public:
    Point() : Point(0.0, 0.0) {}
    Point(double x, double y) : x_(x), y_(y) {}
};
```

기능은 같습니다. 단, 기본 인자가 있는 단일 인자 생성자는 묵시적 변환에 주의(필요 시 `explicit`).

### 7. getter / setter 패턴

```cpp
class Person {
private:
    std::string name_;
    int         age_;
public:
    Person(std::string name, int age) : name_(std::move(name)), age_(age) {}

    // getter (const 멤버 함수, const ref 반환으로 복사 회피)
    const std::string& name() const { return name_; }
    int                age()  const { return age_;  }

    // setter (불변식 검사 포함)
    void set_age(int new_age) {
        if (new_age < 0 || new_age > 150) {
            std::cerr << "[ERROR] invalid age: " << new_age << '\n';
            return;
        }
        age_ = new_age;
    }
};
```

권장 사항:
- getter는 **`const` 멤버 함수**, 큰 객체는 `const T&` 반환.
- setter는 **불변식을 검증**해야 함. 검증 없는 setter는 public 멤버와 다를 바 없음.
- 모든 멤버에 setter를 만들지 말 것. 정말 변경이 필요한 멤버에만 제공.

### 8. 소멸자(잠깐 언급)

```cpp
~Point() {}  // 소멸자, 객체가 사라질 때 호출
```

본 Day의 범위는 아니지만, `class`마다 소멸자가 자동 생성됩니다. 동적 자원(힙 메모리, 파일 핸들 등)을 들고 있다면 소멸자에서 해제해야 합니다(RAII, 추후 학습).

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <string>
#include <utility>   // std::move

class Sensor {
private:
    std::string name_;
    int         id_;
    double      threshold_;
    double      last_reading_ = 0.0;

public:
    // 매개변수 생성자: 멤버 초기화 리스트 사용
    Sensor(std::string name, int id, double threshold)
        : name_(std::move(name)),
          id_(id),
          threshold_(threshold) {
        std::cout << "[CTOR] Sensor #" << id_ << " '" << name_ << "' created\n";
    }

    // 위임 생성자: 기본값
    Sensor() : Sensor("unknown", 0, 0.0) {}

    // getter (const)
    const std::string& name()        const { return name_; }
    int                id()          const { return id_; }
    double             threshold()   const { return threshold_; }
    double             last_reading() const { return last_reading_; }

    // setter: 불변식 검증
    void set_threshold(double t) {
        if (t < 0.0) {
            std::cerr << "[ERROR] threshold must be >= 0\n";
            return;
        }
        threshold_ = t;
    }

    // 객체에 일을 시키는 함수 (단순 getter/setter 보다 우선)
    void feed(double reading) {
        last_reading_ = reading;
        if (reading > threshold_) {
            std::cerr << "[ALERT] #" << id_
                      << " exceeded threshold: " << reading
                      << " > " << threshold_ << '\n';
        }
    }
};

int main() {
    Sensor s1("Temperature_A", 1, 80.0);
    Sensor s_default;

    s1.feed(75.0);
    s1.feed(85.0);
    s1.set_threshold(-10.0);  // 거부됨
    s1.set_threshold(90.0);

    std::cout << s1.name() << " (id=" << s1.id()
              << ") threshold=" << s1.threshold()
              << " last=" << s1.last_reading() << '\n';
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
[CTOR] Sensor #1 'Temperature_A' created
[CTOR] Sensor #0 'unknown' created
[ALERT] #1 exceeded threshold: 85 > 80
[ERROR] threshold must be >= 0
Temperature_A (id=1) threshold=90 last=85
```

---

## [동작 원리]

1. **`Sensor s1("Temperature_A", 1, 80.0)`** 호출 시:
   - 컴파일러가 매개변수 시그니처 일치하는 생성자 선택.
   - 멤버 초기화 리스트가 멤버 변수 선언 순서대로 실행 (`name_` → `id_` → `threshold_` → `last_reading_`).
   - 생성자 본문 실행 (`std::cout`).
2. **`std::move(name)`**: 매개변수 `name`(`std::string` 값 전달)을 `name_` 멤버로 **이동**합니다. 복사 없이 자원을 옮김. 이동 후 `name`은 빈 상태가 됩니다 (이 시점에 사용하지 않으므로 안전).
   - 매개변수를 `const std::string& name`으로 받으면 복사가 발생합니다. 값 전달 + move는 임시 객체에서는 move를, 좌측값에서는 한 번 복사를 수행하는 패턴입니다(C++ Core Guidelines F.15).
3. **`Sensor s_default`**: 기본 생성자 호출 → 위임 생성자 통해 매개변수 생성자가 실행됨.
4. **`set_threshold(-10.0)`**: 음수 검증으로 거부. 멤버 변수 변경 없음. 이것이 캡슐화가 막아주는 무효 상태입니다.

---

## [임베디드 관점]

- **초기화 순서가 중요한 이유**: 임베디드에서는 다른 멤버에 의존하는 멤버가 흔합니다 (예: 버퍼 포인터가 버퍼 크기 멤버에 의존). 멤버 선언 순서를 의존 순서와 일치시키지 않으면 미초기화 데이터를 사용해 UB 발생.
- **생성자에서 무거운 작업 금지**: 생성자에서 하드웨어 초기화나 통신을 수행하면, 객체 생성 자체가 느리고 실패할 수 있습니다. 임베디드에서는 `init()` 메서드로 분리하여 객체 생성과 자원 획득을 분리하는 패턴을 자주 씁니다 (two-phase initialization).
  - 단, two-phase init은 RAII의 이점을 일부 잃습니다. trade-off.
- **`constexpr` 생성자**: 컴파일 타임에 객체를 만들 수 있게 합니다. ROM/Flash에 객체를 둘 때 유용.
  ```cpp
  class Pin {
  public:
      constexpr Pin(int port, int idx) : port_(port), idx_(idx) {}
  private:
      int port_, idx_;
  };
  constexpr Pin led_pin{2, 13};  // ROM에 상수로 배치 가능
  ```
- **객체의 크기 = 멤버 합 + 정렬 패딩**: 가상 함수가 없으면 vtable 포인터(8 bytes on 64-bit)도 없습니다. 임베디드에서 객체 비용을 정확히 알 수 있는 이유.
- **`explicit` 사용 빈도가 높음**: 묵시적 변환은 진단이 어려운 버그를 만들 수 있어, 임베디드 코드 가이드라인(MISRA, AUTOSAR)에서는 단일 인자 생성자에 `explicit`을 강제하는 경우가 많습니다.
- **예외 던지는 생성자 회피**: 예외가 비활성화된 환경(`-fno-exceptions`)에서는 생성자에서 실패를 표현할 방법이 제한됩니다. `init()` 메서드로 분리하고 `bool` 반환으로 결과를 알리는 방식이 흔합니다.

---

## [주의할 점]

1. **초기화 리스트 순서 ≠ 멤버 선언 순서**: 컴파일 경고를 활성화해 멤버 선언 순서대로 적도록 강제하세요.
2. **단일 인자 생성자에 `explicit` 누락**: 묵시적 변환으로 의도치 않은 객체가 만들어집니다.
3. **생성자에서 가상 함수 호출 금지**: 생성자 안에서 가상 함수를 호출하면 **현재 생성 중인 클래스의 버전**이 호출됩니다(아직 파생 클래스 부분이 만들어지지 않음). 의도와 다른 동작을 일으킵니다. (Day09 다형성에서 다시 언급)
4. **기본 생성자 자동 생성 조건**: 클래스에 다른 생성자를 하나라도 정의하면, 컴파일러는 기본 생성자를 자동 생성하지 않습니다. 필요하면 명시적으로 `ClassName() = default;`를 적으세요.
5. **getter가 멤버의 복사를 반환**: 큰 객체를 반환할 때 `std::string get_name() const` 같은 코드는 매번 복사가 발생합니다. `const std::string& get_name() const`로 작성하세요. 단, 객체보다 짧은 수명의 임시 객체를 반환하면 댕글링.
6. **setter 없는 getter만 = read-only**: 의도적입니다. 모든 멤버에 setter를 자동으로 만들지 마세요.
7. **`std::move`된 매개변수의 사용 금지**: `std::move(name)` 이후 `name`을 사용하면 안 됩니다(빈 상태일 수 있음). 컴파일러는 잡지 않습니다.

---

## [연습 문제]

- 문제: [`Day07_problems.md`](./Day07_problems.md)
- 정답: [`Day07_solutions.md`](./Day07_solutions.md)
