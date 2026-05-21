# Day 06 — class · public/private · 캡슐화

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

`class`는 **데이터(멤버 변수) + 그 데이터를 다루는 함수(멤버 함수)**를 하나의 타입으로 묶는 도구입니다. `public`/`private` 키워드는 **접근 제어자(access specifier)**로, 외부에서 멤버에 직접 접근할 수 있는지를 제한합니다. 이를 통해 **캡슐화(encapsulation)** — 내부 구현을 외부로부터 감추는 설계 — 를 구현합니다.

---

## [개념 설명]

### 1. `class` vs `struct`

C++에서 `class`와 `struct`는 **단 한 가지만 다릅니다**: 기본 접근 제어가 다른 것입니다.

- `class` : 멤버의 기본 접근 = **`private`**
- `struct` : 멤버의 기본 접근 = **`public`**

```cpp
class C {
    int x;       // private (기본)
public:
    int y;       // public
};

struct S {
    int x;       // public (기본)
private:
    int y;       // private
};
```

관례:
- **`struct`**: 데이터만 모아둔 POD(Plain Old Data) 또는 매우 단순한 데이터 묶음
- **`class`**: 불변식(invariant)을 가지는 객체 — 멤버 데이터에 직접 접근하지 않고 멤버 함수를 통해서만 조작

C 구조체 호환성을 위해 데이터만 담는 경우는 `struct`, 동작과 상태를 함께 가지는 객체는 `class`로 작성하는 것이 일반적입니다.

### 2. 접근 제어자

| 키워드 | 의미 |
|--------|------|
| `public` | 어디서든 접근 가능 |
| `private` | 같은 클래스의 멤버 함수와 friend만 접근 가능 |
| `protected` | 같은 클래스 + 파생 클래스에서 접근 가능 (Day08에서 다룸) |

```cpp
class Counter {
private:
    int value_;  // 외부에서 직접 접근 불가
public:
    void increment() { ++value_; }
    int  get() const { return value_; }
};
```

### 3. 멤버 함수와 `this` 포인터

멤버 함수 안에서는 **암묵적으로 `this` 포인터**가 전달됩니다. `this`는 현재 객체를 가리키는 `Counter*` (혹은 const 함수면 `const Counter*`).

```cpp
class Counter {
private:
    int value_;
public:
    void increment() {
        ++this->value_;   // this-> 명시 가능
        // 또는: ++value_;  // 컴파일러가 this->를 자동으로 붙임
    }
};
```

### 4. `const` 멤버 함수

```cpp
int get() const { return value_; }
```

- 함수 선언 끝의 `const`는 "**이 함수는 멤버를 수정하지 않는다**"는 계약입니다.
- `const` 객체에 대해 호출 가능한 함수는 `const` 멤버 함수뿐입니다.
- **getter는 거의 항상 `const` 멤버 함수**여야 합니다.

```cpp
const Counter c;
c.get();         // OK (const 함수)
c.increment();   // 컴파일 에러 (non-const 함수)
```

### 5. 캡슐화의 본질

> "외부에서 멤버 변수에 직접 접근하지 못하게 하라"가 캡슐화의 표면이고,
> **"객체의 불변식을 외부로부터 보호하라"**가 본질입니다.

예: `Temperature` 클래스가 `-273.15`보다 낮은 값을 가지면 물리적으로 무효. 이 불변식을 멤버 함수에서 검증하면, 어떤 코드에서도 무효한 객체가 만들어질 수 없습니다.

```cpp
class Temperature {
private:
    double celsius_;
public:
    void set(double c) {
        if (c < -273.15) {
            c = -273.15;  // 클램프 또는 throw
        }
        celsius_ = c;
    }
    double get() const { return celsius_; }
};
```

만약 `celsius_`가 public이었다면, 어디서든 `t.celsius_ = -500`이 가능해져 불변식이 깨집니다. 이것이 캡슐화가 막아주는 것입니다.

### 6. 멤버 함수의 정의 위치

```cpp
// 1. 클래스 안에 정의 (자동으로 inline)
class A {
public:
    void f() { /* ... */ }
};

// 2. 클래스 밖에 정의 (선언만 안쪽)
class B {
public:
    void f();
};
void B::f() { /* ... */ }
```

대규모 프로젝트에서는 헤더에 선언, .cpp에 정의하는 형태가 표준입니다. 작은 인라인 함수는 클래스 안에 두어도 됩니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <string>

class BankAccount {
private:
    std::string owner_;
    long long   balance_won_;  // 원 단위 정수 (부동소수점 오차 회피)

public:
    // 초기 잔액을 0으로 설정하는 단순 초기화 함수 (생성자는 Day07에서 다룸)
    void init(const std::string& owner) {
        owner_ = owner;
        balance_won_ = 0;
    }

    void deposit(long long amount) {
        if (amount <= 0) {
            std::cerr << "[ERROR] deposit must be positive\n";
            return;
        }
        balance_won_ += amount;
    }

    bool withdraw(long long amount) {
        if (amount <= 0) {
            std::cerr << "[ERROR] withdraw must be positive\n";
            return false;
        }
        if (amount > balance_won_) {
            std::cerr << "[ERROR] insufficient balance\n";
            return false;
        }
        balance_won_ -= amount;
        return true;
    }

    long long balance() const { return balance_won_; }
    const std::string& owner() const { return owner_; }
};

int main() {
    BankAccount acc;
    acc.init("Seungyeon");

    acc.deposit(100000);
    acc.deposit(50000);
    acc.withdraw(30000);
    acc.withdraw(999999);  // 실패

    std::cout << "Owner   : " << acc.owner() << '\n';
    std::cout << "Balance : " << acc.balance() << " KRW\n";

    // acc.balance_won_ = 10000000;  // 컴파일 에러: private 멤버 접근 불가
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
[ERROR] insufficient balance
Owner   : Seungyeon
Balance : 120000 KRW
```

---

## [동작 원리]

1. **메모리 레이아웃**: `BankAccount` 객체는 `owner_`(보통 24~32 bytes, 구현에 따라) + `balance_won_`(8 bytes)로 구성됩니다. 멤버 변수만 메모리에 자리를 차지하며, **멤버 함수는 객체마다 따로 존재하지 않습니다**. 함수 코드는 한 곳에 있고, 호출 시 어떤 객체에 대해 호출됐는지 `this` 포인터로 구분합니다.
2. **`this` 포인터의 전달**: `acc.deposit(100000)`는 컴파일러 입장에서 대략 `deposit(&acc, 100000)`처럼 변환됩니다. 함수 안의 `balance_won_`은 `this->balance_won_`입니다.
3. **접근 제어는 컴파일 타임 검사**: `private` 멤버 접근은 컴파일 단계에서 차단됩니다. 런타임에는 검사하지 않습니다. 따라서 보안 목적이 아닌 **설계 규율 도구**입니다 (포인터로 캐스팅하면 우회 가능).
4. **`const` 멤버 함수의 효과**: `balance() const`에서 `this`는 `const BankAccount*` 타입이 됩니다. 따라서 멤버 변수 수정 시도가 컴파일 에러로 잡힙니다.

---

## [임베디드 관점]

- **하드웨어 추상화**: 임베디드 펌웨어에서 `class`는 **HAL(Hardware Abstraction Layer)**과 디바이스 드라이버에 자주 쓰입니다. 예: `class Uart { ... }`가 `UART_HandleTypeDef*`를 멤버로 가지고 송수신 메서드를 제공.
- **객체 크기 제어**: 멤버 변수의 순서가 객체 크기에 영향을 줍니다(패딩 때문). `char` `int` `char`보다 `char` `char` `int`가 효율적인 패킹입니다. (Day는 별도 — 구조체 패킹/정렬 학습 시점에)
- **vtable 없음**: 가상 함수가 없는 클래스는 **C struct와 메모리 레이아웃이 동일**합니다. 따라서 펌웨어의 메모리 비용은 멤버 변수 합과 거의 같습니다.
- **`static` 멤버**: 모든 객체가 공유하는 데이터. ROM/Flash에 둘 수 있으면 RAM 절약. `static constexpr` 멤버는 컴파일 타임 상수.
- **싱글톤 패턴**: MCU에서 하드웨어 자원은 일반적으로 단일 인스턴스. 싱글톤(`static T& instance()`) 또는 namespace-level 정적 객체로 표현합니다. C++11 이후 `static` 지역 객체는 thread-safe 초기화가 보장되지만, 임베디드 컴파일러 옵션에 따라 그렇지 않을 수 있습니다(`-fno-threadsafe-statics`).
- **PIMPL 회피**: PC 코드에서 자주 쓰이는 PIMPL(Pointer to Implementation) 패턴은 힙 할당을 동반하므로 임베디드에서는 피하는 경우가 많습니다.

---

## [주의할 점]

1. **모든 멤버를 public으로 두지 말 것**. C에서 넘어온 개발자가 자주 하는 실수입니다. 데이터 + 동작을 묶을 거라면 데이터를 숨기세요.
2. **`const` 멤버 함수 누락**: getter가 non-const면 `const` 객체에 호출 불가. const-correctness가 깨지면 전염성이 강합니다 (호출자도 const를 못 씀).
3. **멤버 이름 컨벤션**: 멤버 변수에 `_` 접미사(`balance_`) 또는 `m_` 접두사(`m_balance`)를 붙이는 컨벤션이 흔합니다. 지역 변수와 구분하고, 함수 매개변수와의 이름 충돌을 피합니다.
4. **자기 대입(self-assignment)**: 복사 대입 연산자 등을 직접 구현할 때 자기 대입 처리를 잊지 마세요 (Day07 이후의 주제이지만 기억해 둘 것).
5. **public 메서드가 너무 많음**: 클래스의 public 인터페이스가 클수록 사용처가 다양해져 변경이 어려워집니다. 정말 필요한 것만 public으로 두세요.
6. **getter/setter 남발**: 모든 멤버에 기계적으로 getter/setter를 만들면 캡슐화가 무의미해집니다(객체 외부에서 결국 다 만질 수 있으니까). **객체에 일을 시키는 메서드**(예: `account.deposit(x)`)로 표현하는 것이 객체 지향의 핵심입니다.
7. **`friend` 남용**: `friend` 키워드는 private 접근 권한을 부여합니다. 캡슐화를 깨므로 신중히 사용하세요.

---

## [연습 문제]

- 문제: [`Day06_problems.md`](./Day06_problems.md)
- 정답: [`Day06_solutions.md`](./Day06_solutions.md)
