# Day 09 — 다형성: virtual · override · 추상 클래스

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

`virtual` 키워드는 멤버 함수에 **동적 디스패치(dynamic dispatch)**를 부여합니다. base 포인터/참조로 derived 객체를 가리킬 때, 호출되는 함수가 **실제 객체의 타입**에 따라 결정되는 것이 **런타임 다형성**입니다. C++11의 `override` 키워드는 오버라이딩 의도를 명시해 흔한 실수를 컴파일 타임에 잡아줍니다. **순수 가상 함수(`= 0`)**가 하나라도 있는 클래스는 **추상 클래스(abstract class)**가 되어 직접 인스턴스화할 수 없습니다.

> **가상 함수가 있는 base 클래스의 소멸자는 반드시 `virtual`로 선언해야 합니다.** 그렇지 않으면 `delete base_ptr`이 derived의 소멸자를 호출하지 않아 자원 누수가 발생합니다.

---

## [개념 설명]

### 1. 가상 함수와 오버라이딩

```cpp
class Animal {
public:
    virtual void speak() const { std::cout << "Generic animal sound\n"; }
    virtual ~Animal() = default;   // 가상 소멸자 (필수)
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow!\n"; }
};

void make_sound(const Animal& a) {
    a.speak();    // 런타임에 실제 타입에 맞춰 호출됨
}
```

- `virtual`을 선언한 함수는 derived 클래스가 **`override`**로 재정의할 수 있습니다.
- 호출 측은 `Animal&`나 `Animal*`로 다루어도, **실제 객체가 `Dog`이면 `Dog::speak`**가 호출됩니다.

### 2. `override` 키워드 (C++11)

```cpp
class Base {
public:
    virtual void f(int) const;
};

class Derived : public Base {
public:
    void f(int) const override;  // OK
    void f(double) override;     // 컴파일 에러: 시그니처가 base와 다름
};
```

- `override`는 "이 함수가 base의 가상 함수를 재정의한다"는 의도를 명시합니다.
- 시그니처가 일치하지 않으면 컴파일 에러로 잡아줍니다. **거의 모든 오버라이딩에 `override`를 붙이는 것이 표준 관행**입니다.

### 3. 순수 가상 함수와 추상 클래스

```cpp
class Shape {
public:
    virtual double area() const = 0;        // 순수 가상 함수
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;
};

// Shape s;  // 컴파일 에러: 추상 클래스
```

- `= 0`이 붙은 함수는 본문이 없는 **순수 가상 함수**.
- 순수 가상 함수가 하나라도 있는 클래스는 **추상 클래스**가 되어 인스턴스화 불가.
- 모든 순수 가상 함수를 derived에서 override해야 derived도 인스턴스화 가능. 하나라도 빠지면 derived도 추상 클래스.

### 4. 가상 소멸자

```cpp
class Base {
public:
    virtual ~Base() = default;   // 가상 소멸자
};

class Derived : public Base {
public:
    ~Derived() { /* derived 자원 해제 */ }
};

Base* p = new Derived();
delete p;     // virtual 소멸자 덕분에 Derived::~Derived() 호출됨
```

가상 소멸자가 없으면 `delete p`는 `Base::~Base()`만 호출 → derived의 멤버가 정리되지 않음. **다형성을 사용하는 클래스 계층은 base의 소멸자가 반드시 `virtual`**이어야 합니다.

### 5. `final` 키워드 (C++11)

```cpp
class Logger final : public BaseLogger {  // 더 이상 상속 불가
public:
    void log(const std::string& msg) override final;  // 더 이상 오버라이딩 불가
};
```

- 클래스에 `final`: 더 이상 derived 불가.
- 가상 함수에 `final`: derived에서 더 이상 override 불가.
- 컴파일러가 이를 알면 **devirtualization**(직접 호출로 최적화)을 적용할 수 있어 vtable 호출 비용을 제거할 수 있습니다.

### 6. vtable (구현 세부)

가상 함수가 있는 클래스의 객체는 각 인스턴스마다 숨겨진 포인터(**vptr**)를 가지며, 이 포인터는 **vtable**(클래스마다 하나, 정적 데이터)을 가리킵니다. vtable에는 각 가상 함수의 실제 주소가 저장되어 있습니다.

```
Derived 객체 메모리:
+---------+
| vptr    | --> Derived의 vtable --> [Derived::speak, Base::other_func, ~Derived]
| base 멤버  |
| derived 멤버 |
+---------+
```

- 호출 비용: 가상 함수 호출은 추가 메모리 로드 1회 + 간접 분기 1회. PC에서는 보통 무시할 수준, 임베디드에서는 핫 패스에서 영향을 줄 수 있습니다.
- 객체 크기: 가상 함수가 있는 클래스는 vptr 크기(64-bit 시스템에서 8 bytes)만큼 추가됨.

### 7. `dynamic_cast` (간단히 소개)

```cpp
Animal* a = new Dog();
Dog* d = dynamic_cast<Dog*>(a);  // 성공: 실제 타입이 Dog
if (d) {
    d->fetch();
}
```

- 런타임에 안전하게 다운캐스트.
- **RTTI(Run-Time Type Information)**를 필요로 하며, 임베디드에서는 흔히 비활성화(`-fno-rtti`)합니다.
- 임베디드에서는 `static_cast`(컴파일 타임)와 별도 타입 태그(`enum`)로 우회하는 경우가 많습니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <utility>

// 추상 base 클래스
class Shape {
public:
    explicit Shape(std::string name) : name_(std::move(name)) {}

    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;  // 가상 소멸자

    const std::string& name() const { return name_; }

    // 비가상 멤버 함수도 가능 (모든 derived가 동일하게 사용)
    void describe() const {
        std::cout << name_
                  << ": area=" << area()
                  << ", perimeter=" << perimeter() << '\n';
    }

private:
    std::string name_;
};

class Rectangle : public Shape {
public:
    Rectangle(double w, double h)
        : Shape("Rectangle"), w_(w), h_(h) {}

    double area() const override      { return w_ * h_; }
    double perimeter() const override { return 2.0 * (w_ + h_); }

private:
    double w_, h_;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : Shape("Circle"), r_(r) {}

    double area() const override      { return kPi * r_ * r_; }
    double perimeter() const override { return 2.0 * kPi * r_; }

private:
    static constexpr double kPi = 3.14159265358979323846;
    double r_;
};

class Square final : public Rectangle {  // 더 이상 상속 불가
public:
    explicit Square(double side) : Rectangle(side, side) {}
    // area / perimeter는 Rectangle의 것을 재사용
};

int main() {
    // 다형성: Shape*로 다양한 도형을 다룸
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Square>(2.5));

    for (const auto& s : shapes) {
        s->describe();   // 실제 타입의 area/perimeter 호출
    }

    // 합계
    double total_area = 0.0;
    for (const auto& s : shapes) {
        total_area += s->area();
    }
    std::cout << "Total area = " << total_area << '\n';

    // shapes가 스코프를 벗어나면 unique_ptr이 자동 delete
    // 가상 소멸자 덕분에 Rectangle/Circle/Square의 소멸자가 정확히 호출됨
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
Rectangle: area=12, perimeter=14
Circle: area=78.5398, perimeter=31.4159
Rectangle: area=6.25, perimeter=10
Total area = 96.7898
```

(`Square`는 `Rectangle`로부터 `name()`을 그대로 상속받았으므로 출력에서 "Rectangle"로 표시됩니다. 원한다면 `Square`의 생성자에서 base의 name을 바꾸도록 설계할 수도 있습니다.)

---

## [동작 원리]

1. `std::make_unique<Rectangle>(3.0, 4.0)`은 힙에 `Rectangle` 객체를 만들고 `std::unique_ptr<Shape>`에 저장합니다. unique_ptr은 소멸 시 자동으로 `delete`를 호출합니다.
2. `s->describe()` 호출: `describe`는 비가상 함수지만 내부에서 `area()`/`perimeter()`(가상 함수)를 호출합니다. vptr을 통해 **실제 타입**의 `area`/`perimeter`가 호출됩니다.
3. **vptr 동작 흐름**:
   - `s->area()` 호출
   - 컴파일러가 `s`의 vptr을 통해 vtable에 접근
   - vtable에서 `area`의 슬롯(예: 인덱스 0)을 조회
   - 해당 주소로 간접 호출
4. **메모리 회수**: `shapes` 벡터가 소멸되면 각 unique_ptr이 소멸하면서 `delete`를 호출. 가상 소멸자 덕분에 `delete` 시점에 실제 객체의 소멸자(`Rectangle::~Rectangle`, `Circle::~Circle` 등)가 정확히 호출됩니다.
5. **`Square : public Rectangle`**: `Square` 객체에서 `area()`를 호출하면 `Rectangle::area()`가 호출됩니다(Square가 override하지 않았으므로).

---

## [임베디드 관점]

- **vtable 비용**:
  - 객체마다 vptr 추가 (8 bytes on 64-bit, 4 bytes on 32-bit MCU)
  - 가상 함수 호출 시 메모리 로드 1회 + 간접 분기 1회
  - PC에서는 무시할 수준, 8/16-bit MCU에서는 측정 가치 있음
  - Cortex-M 32-bit MCU에서는 일반적으로 충분히 빠름
- **vtable 자체는 Flash에 저장**됩니다. 클래스마다 하나, derived마다 별도. 클래스 수가 많으면 Flash 사용량이 늘어납니다.
- **devirtualization**: `final` 키워드를 적극 사용하면 컴파일러가 가상 호출을 직접 호출로 바꿀 수 있습니다. 임베디드 최적화에 도움.
- **`-fno-rtti`**: RTTI는 `dynamic_cast`와 `typeid`를 위한 정보. 임베디드에서 거의 사용하지 않으므로 비활성화하여 바이너리 크기 절약.
- **`-fno-exceptions`**: 예외 메커니즘도 비활성화하는 경우가 많습니다. 단, STL의 일부 동작이 제한됨.
- **CRTP(Curiously Recurring Template Pattern)**: 가상 함수 비용 없이 정적 다형성을 구현하는 패턴. 임베디드에서 자주 사용:
  ```cpp
  template <typename Derived>
  class Base {
  public:
      void do_work() { static_cast<Derived*>(this)->work_impl(); }
  };

  class MyDevice : public Base<MyDevice> {
  public:
      void work_impl() { /* ... */ }
  };
  ```
  컴파일 타임에 결정되어 인라인 확장 가능. 단, 런타임 다형성을 흉내내지는 못함(컨테이너에 다양한 타입을 함께 담는 건 불가).
- **가상 함수와 ISR**: ISR 안에서 가상 함수를 호출하면 vtable lookup 비용 + 캐시 미스 가능성으로 인해 latency가 증가할 수 있습니다. ISR에서는 비가상 함수를 권장합니다.

---

## [주의할 점]

1. **가상 소멸자 누락**: `delete base_ptr`이 derived 소멸자를 호출하지 않아 부분 소멸. 다형성을 위한 base 클래스에는 반드시 `virtual ~Base() = default;`를 두세요.
2. **생성자/소멸자에서 가상 함수 호출 금지**: 부모 생성자 안에서 가상 함수를 호출하면 **부모 버전이 호출**됩니다 (자식 객체가 아직 완성되지 않음). 의도와 다른 동작.
3. **`override` 누락**: derived의 함수 시그니처가 base와 약간 다를 때(예: `const` 차이) 새로운 가상 함수로 인식되어 의도와 달리 base 함수가 호출됩니다. `override`로 컴파일러가 잡도록 하세요.
4. **시그니처 차이**:
   - `void f(int) const` vs `void f(int)` — 다른 함수로 취급
   - `virtual void f() const` vs `void f() const override` (override만 적고 virtual 생략 가능)
5. **`virtual`을 derived에서 반복**: derived의 override 함수에서 `virtual`을 다시 적어도 되지만 권장하지 않습니다. `override`만 적는 것이 명확.
6. **공변(covariant) 반환 타입**: 가상 함수가 derived에서 더 구체적인 타입(파생 클래스의 포인터/참조)을 반환할 수 있습니다. `Clone()` 같은 패턴에서 사용.
   ```cpp
   class Base {
   public:
       virtual Base* clone() const = 0;
   };
   class Derived : public Base {
   public:
       Derived* clone() const override { return new Derived(*this); }
   };
   ```
7. **다중 상속과 가상 상속**: C++의 다중 상속은 복잡한 함정(diamond problem)을 가집니다. 본 학습 단계에서는 다루지 않으며, 가능하면 단일 상속 + 합성으로 해결하세요.
8. **객체 슬라이싱**: 값 복사 시 derived 부분이 잘려나가는 문제는 다형성에서도 동일하게 적용. **참조나 스마트 포인터를 사용**하세요.
9. **`raw new`/`raw delete` 사용 금지**: `std::unique_ptr`/`std::shared_ptr`을 사용하면 가상 소멸자와 함께 자원 관리가 안전합니다. 임베디드에서 동적 할당을 피한다면 정적 객체 + 포인터 컨테이너 패턴을 고려.

---

## [연습 문제]

- 문제: [`Day09_problems.md`](./Day09_problems.md)
- 정답: [`Day09_solutions.md`](./Day09_solutions.md)
