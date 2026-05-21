# Day 08 — 상속 · protected

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

상속(inheritance)은 한 클래스(파생 클래스, derived)가 다른 클래스(기반 클래스, base)의 멤버를 **재사용**하면서 **확장**하는 메커니즘입니다. C++의 상속 종류는 `public`, `protected`, `private` 세 가지이며, 일반적으로는 `public` 상속만 사용합니다. `protected` 접근 제어자는 **파생 클래스에는 접근 허용, 그 외 외부에는 차단**합니다.

> 핵심 원칙: `public` 상속은 **"is-a" 관계**여야 합니다. `Derived`가 `Base`의 모든 인터페이스를 만족해야 합니다(Liskov 치환 원칙).

---

## [개념 설명]

### 1. 기본 문법

```cpp
class Base {
public:
    void hello() { std::cout << "Base::hello\n"; }
};

class Derived : public Base {
    // public, protected 멤버를 모두 상속
};

Derived d;
d.hello();  // Base의 멤버 함수가 호출됨
```

### 2. 상속의 종류

| 상속 방식 | base의 public | base의 protected | base의 private |
|-----------|---------------|------------------|----------------|
| `public` | public 유지 | protected 유지 | (접근 불가) |
| `protected` | protected로 격하 | protected 유지 | (접근 불가) |
| `private` | private로 격하 | private로 격하 | (접근 불가) |

- 기본은 `class`에서 `private`, `struct`에서 `public`. **명시적으로 `public`을 적는 것이 안전합니다**.
- `private` 멤버는 어떤 경우에도 파생 클래스에서 직접 접근할 수 없습니다.

### 3. `protected` 접근 제어

```cpp
class Base {
private:
    int secret_;       // Derived에서도 접근 불가
protected:
    int internal_;     // Derived에서 접근 가능, 외부에서는 불가
public:
    int api_;          // 어디서든 접근 가능
};

class Derived : public Base {
public:
    void f() {
        // secret_ = 0;   // 컴파일 에러
        internal_ = 1;    // OK
        api_ = 2;         // OK
    }
};

Derived d;
// d.internal_ = 5;       // 컴파일 에러
d.api_ = 5;               // OK
```

`protected`는 **하위 클래스에게는 데이터를 공유**하면서 **외부 사용자에게는 캡슐화를 유지**하는 절충안입니다. 하지만 남용하면 캡슐화가 깨지므로 **가능하면 protected getter/setter**를 두고, protected 데이터 멤버는 최소화하는 것이 권장됩니다.

### 4. 생성자의 호출 순서

```cpp
class Base {
public:
    Base()        { std::cout << "Base()\n"; }
    Base(int x)   { std::cout << "Base(" << x << ")\n"; }
};

class Derived : public Base {
public:
    Derived() : Base(42) { std::cout << "Derived()\n"; }
};

Derived d;
// 출력:
// Base(42)
// Derived()
```

- **기반 클래스의 생성자가 먼저** 호출됩니다. 그래야 파생 클래스에서 기반 멤버에 안전하게 접근할 수 있습니다.
- 멤버 초기화 리스트에서 `Base(인자...)`로 호출하지 않으면 기본 생성자가 호출됩니다. 기본 생성자가 없으면 컴파일 에러.
- 소멸 순서는 **역순**: Derived 소멸 → Base 소멸.

### 5. 멤버 함수 숨김(name hiding)

```cpp
class Base {
public:
    void f(int) { std::cout << "Base::f(int)\n"; }
};

class Derived : public Base {
public:
    void f(double) { std::cout << "Derived::f(double)\n"; }
};

Derived d;
d.f(1);          // Derived::f(double) 호출됨 (Base::f(int)이 가려짐)
d.Base::f(1);    // 명시적 접근으로만 호출 가능
```

이를 해결하려면 `using Base::f;`로 base의 오버로드를 가져옵니다.

```cpp
class Derived : public Base {
public:
    using Base::f;     // Base::f(int)도 사용 가능
    void f(double) { /* ... */ }
};
```

### 6. 슬라이싱(slicing) — 매우 중요한 함정

```cpp
class Base {
public:
    int a = 1;
};

class Derived : public Base {
public:
    int b = 2;
};

Derived d;
Base copy = d;     // 슬라이싱: b 부분이 잘려나감
// copy.b 는 존재하지 않음
```

값 복사 시에는 **base 부분만 복사**됩니다. 이를 피하려면 **참조나 포인터로 다형성을 다루세요**.

```cpp
Base& ref = d;     // OK, 다형성 가능
Base* ptr = &d;    // OK
```

### 7. `final` 키워드 (C++11)

```cpp
class Base final {  // 이 클래스는 더 이상 상속 불가
    // ...
};
```

또는 가상 함수에 적용하여 추가 오버라이딩 금지(Day09 다형성에서 다룸).

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <string>
#include <cstdint>

// 기반 클래스: 모든 센서의 공통 인터페이스
class Sensor {
private:
    std::string name_;
    int         id_;

protected:
    double threshold_ = 0.0;   // 파생 클래스가 직접 다룰 수 있게 protected

public:
    Sensor(std::string name, int id) : name_(std::move(name)), id_(id) {
        std::cout << "[Sensor ctor] '" << name_ << "' id=" << id_ << '\n';
    }

    ~Sensor() {
        std::cout << "[Sensor dtor] '" << name_ << "'\n";
    }

    const std::string& name() const { return name_; }
    int                id()   const { return id_; }

    void set_threshold(double t) { threshold_ = t; }
    double threshold() const { return threshold_; }

    void report() const {
        std::cout << "[REPORT] " << name_
                  << " (id=" << id_
                  << ", thr=" << threshold_ << ")\n";
    }
};

// 파생 클래스 1: 온도 센서
class TempSensor : public Sensor {
private:
    double last_c_ = 0.0;

public:
    TempSensor(std::string name, int id)
        : Sensor(std::move(name), id) {
        std::cout << "[TempSensor ctor] " << this->name() << '\n';
    }

    ~TempSensor() {
        std::cout << "[TempSensor dtor] " << this->name() << '\n';
    }

    void feed_c(double c) {
        last_c_ = c;
        // protected 멤버 threshold_에 직접 접근
        if (c > threshold_) {
            std::cerr << "[TEMP ALERT] " << name() << " " << c << " > "
                      << threshold_ << '\n';
        }
    }

    double last_c() const { return last_c_; }
};

// 파생 클래스 2: 압력 센서
class PressureSensor : public Sensor {
private:
    double last_kpa_ = 0.0;

public:
    PressureSensor(std::string name, int id)
        : Sensor(std::move(name), id) {}

    void feed_kpa(double kpa) {
        last_kpa_ = kpa;
        if (kpa > threshold_) {
            std::cerr << "[PRESS ALERT] " << name() << " " << kpa << " kPa > "
                      << threshold_ << '\n';
        }
    }

    double last_kpa() const { return last_kpa_; }
};

int main() {
    TempSensor t("temp_A", 1);
    t.set_threshold(80.0);
    t.feed_c(75.0);
    t.feed_c(85.0);

    PressureSensor p("press_B", 2);
    p.set_threshold(150.0);
    p.feed_kpa(120.0);
    p.feed_kpa(160.0);

    t.report();
    p.report();

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
[Sensor ctor] 'temp_A' id=1
[TempSensor ctor] temp_A
[TEMP ALERT] temp_A 85 > 80
[Sensor ctor] 'press_B' id=2
[PRESS ALERT] press_B 160 kPa > 150
[REPORT] temp_A (id=1, thr=80)
[REPORT] press_B (id=2, thr=150)
[TempSensor dtor] temp_A
[Sensor dtor] 'temp_A'
[Sensor dtor] 'press_B'
```

(소멸 출력 순서는 main의 지역 변수가 역순으로 소멸되어 p가 먼저, t가 나중에 소멸됩니다. 정확한 출력 순서는 컴파일러/실행 환경에 따라 다를 수 있습니다.)

---

## [동작 원리]

1. **메모리 레이아웃**: `TempSensor` 객체의 메모리는 `[Sensor 부분 (name_, id_, threshold_)] [TempSensor 부분 (last_c_)]` 형태입니다. **base의 멤버가 먼저** 배치되므로, `Sensor*`로 캐스트하면 동일한 시작 주소로 접근 가능합니다.
2. **생성자 호출**: `TempSensor("temp_A", 1)` 호출 시 → `Sensor("temp_A", 1)` 먼저 실행 → `Sensor` 생성자 본문 → 멤버 초기화 → `TempSensor` 생성자 본문.
3. **`protected threshold_`에 접근**: TempSensor의 `feed_c` 안에서 `threshold_`를 직접 읽고 씁니다. `private`이었다면 `set_threshold`/`threshold()`를 거쳐야 했을 것입니다.
4. **`std::move(name)`이 base로 전달**: 매개변수 `name`을 `Sensor(std::move(name), id)`로 넘기면 string의 내용이 base로 이동합니다. 이후 derived의 본문에서 `name`은 비어있을 수 있으므로, derived 생성자 본문에서는 `this->name()`(base의 getter)으로 접근합니다.

---

## [임베디드 관점]

- **인터페이스 일관화**: 다양한 센서 드라이버(I2C, SPI, ADC 기반)에서 공통 인터페이스(`init`, `read`, `report`)를 base 클래스로 추출하고, 각 통신 방식에 맞춰 파생 클래스로 구현하는 패턴이 자주 보입니다.
- **vtable 비용**: **이번 Day의 예제에는 가상 함수가 없으므로 vtable 비용이 없습니다**. 즉, 상속만으로는 런타임 오버헤드가 없습니다. 다형성(Day09)에서 `virtual`을 도입하면 vtable이 추가됩니다.
- **상속 깊이**: 임베디드 코드 가이드라인은 상속 깊이를 보통 2~3 단계로 제한합니다. 깊은 상속은 디버깅을 어렵게 만들고 객체 크기를 키웁니다.
- **EBO (Empty Base Optimization)**: 멤버가 없는 base 클래스를 상속받으면 컴파일러가 1 byte도 추가하지 않습니다. 정책 클래스(policy class) 패턴에 사용.
- **`public` 외 상속의 드문 사용**: `private` 상속은 "has-a"를 표현하지만 합성(composition)으로 대체 가능. 임베디드 코드에서 `public` 외 상속을 보면 거의 항상 합성으로 바꿀 수 있는지 검토하세요.
- **MISRA C++ 가이드라인**: 다이아몬드 상속(다중 상속) 사용을 매우 제한합니다. 본 학습 단계에서도 다중 상속은 다루지 않습니다.

---

## [주의할 점]

1. **`public` 상속을 항상 명시**: `class Derived : Base` (키워드 누락)는 private 상속이 되어 base의 public 멤버가 모두 외부 접근 불가. 거의 항상 의도와 다름.
2. **슬라이싱**: `Base b = derived_obj;` 또는 `void f(Base x); f(derived_obj);`는 base 부분만 복사됩니다. 다형성을 원하면 참조/포인터를 사용하세요.
3. **base 클래스의 소멸자가 `virtual`이 아닌 채로 `delete base_ptr`**: 파생 클래스 소멸자가 호출되지 않아 자원 누수. base 클래스가 다형성 용도라면 **`virtual ~Base() = default;`**를 두세요(Day09).
4. **`protected` 데이터 멤버 남용**: 파생 클래스가 직접 멤버를 만지면 캡슐화가 깨집니다. base 변경 시 파생 클래스가 함께 깨질 위험이 큽니다. `protected` 멤버 함수로 노출하는 편이 안전.
5. **base의 private 멤버 접근 시도**: 컴파일 에러. base의 인터페이스(`get_x()` 등)를 통해 접근.
6. **이름 숨김(name hiding)**: 같은 이름의 함수가 base에도 있고 derived에도 있으면 base의 모든 오버로드가 가려집니다. 의도하지 않은 동작을 일으킬 수 있으니 `using Base::name;`을 활용하세요.
7. **생성자/소멸자에서 가상 함수 호출 금지** (Day09 예고): 부모 생성자 안에서는 자식 함수가 아직 존재하지 않으므로, 가상 함수를 호출해도 부모 버전이 호출됩니다.

---

## [연습 문제]

- 문제: [`Day08_problems.md`](./Day08_problems.md)
- 정답: [`Day08_solutions.md`](./Day08_solutions.md)
