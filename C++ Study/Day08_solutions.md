# Day 08 — 연습 문제 정답 (상속 · protected)

---

## 문제 1 정답

```cpp
#include <iostream>
#include <string>
#include <utility>

class Animal {
private:
    std::string name_;
public:
    Animal(std::string name) : name_(std::move(name)) {}
    const std::string& name() const { return name_; }
    void sleep() const {
        std::cout << name_ << " is sleeping\n";
    }
};

class Dog : public Animal {
public:
    Dog(std::string name) : Animal(std::move(name)) {}
    void bark() const {
        std::cout << name() << " says Woof!\n";
    }
};

int main() {
    Dog d("Rex");
    d.bark();
    d.sleep();
    return 0;
}
```

**해설**
- `Dog`의 생성자는 초기화 리스트에서 `Animal(std::move(name))`을 호출. base의 매개변수 생성자가 없으면 컴파일 에러가 났을 것입니다.
- `bark()` 안에서 `name_`을 직접 쓰지 않고 `name()` getter를 사용한 이유: `name_`은 `Animal`의 `private` 멤버라 `Dog`에서 직접 접근 불가. base의 public getter를 통해야 합니다.

---

## 문제 2 정답

```cpp
#include <iostream>

class Vehicle {
protected:
    int speed_kmh_ = 0;
public:
    Vehicle() = default;
    int speed() const { return speed_kmh_; }
};

class Car : public Vehicle {
public:
    Car() = default;

    void accelerate(int delta) {
        if (delta < 0) {
            std::cerr << "[ERROR] delta must be >= 0\n";
            return;
        }
        speed_kmh_ += delta;
        if (speed_kmh_ > 200) speed_kmh_ = 200;
    }

    void brake(int delta) {
        if (delta < 0) {
            std::cerr << "[ERROR] delta must be >= 0\n";
            return;
        }
        speed_kmh_ -= delta;
        if (speed_kmh_ < 0) speed_kmh_ = 0;
    }
};

int main() {
    Car c;
    c.accelerate(80);
    std::cout << "speed=" << c.speed() << '\n';
    c.accelerate(150);
    std::cout << "speed=" << c.speed() << '\n';
    c.brake(50);
    std::cout << "speed=" << c.speed() << '\n';
    c.brake(-10);
    std::cout << "speed=" << c.speed() << '\n';
    return 0;
}
```

**해설**
- `Car`는 `speed_kmh_`에 직접 접근 가능 (protected). 만약 private이었다면 `set_speed(int)` 같은 인터페이스가 필요했을 것입니다.
- `Vehicle()`을 `= default`로 두면 컴파일러가 기본 생성자를 생성합니다. NSDMI(`int speed_kmh_ = 0;`)와 함께라면 안전한 초기 상태가 보장됩니다.
- 음수 delta는 사전에 검증하므로 클램핑 로직(max 200, min 0)이 안전합니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <iomanip>
#include <cstdint>

class Device {
protected:
    bool initialized_ = false;
    uint16_t address_;
public:
    explicit Device(uint16_t addr) : address_(addr) {}

    bool is_initialized() const { return initialized_; }
    uint16_t address() const { return address_; }

    void status() const {
        std::cout << "[STATUS] addr=0x"
                  << std::hex << std::uppercase << address_
                  << std::dec
                  << " init=" << std::boolalpha << initialized_ << '\n';
    }
};

class I2cDevice : public Device {
private:
    uint32_t clock_hz_;
public:
    I2cDevice(uint16_t addr, uint32_t clock_hz)
        : Device(addr), clock_hz_(clock_hz) {}

    bool begin() {
        initialized_ = true;
        std::cout << "[I2C] addr=0x" << std::hex << std::uppercase << address_
                  << std::dec
                  << " clock=" << clock_hz_ << " started\n";
        return true;
    }

    bool write(uint8_t reg, uint8_t value) {
        if (!initialized_) {
            std::cerr << "[ERROR] device not initialized\n";
            return false;
        }
        std::cout << "[I2C] write reg=0x" << std::hex << std::uppercase
                  << static_cast<unsigned>(reg)
                  << " val=0x" << static_cast<unsigned>(value)
                  << std::dec << '\n';
        return true;
    }
};

class SpiDevice : public Device {
private:
    uint8_t cs_pin_;
public:
    SpiDevice(uint16_t addr, uint8_t cs_pin)
        : Device(addr), cs_pin_(cs_pin) {}

    bool begin() {
        initialized_ = true;
        std::cout << "[SPI] addr=0x" << std::hex << std::uppercase << address_
                  << std::dec
                  << " cs=" << static_cast<unsigned>(cs_pin_) << " started\n";
        return true;
    }

    bool transfer(uint8_t tx, uint8_t& rx) {
        if (!initialized_) {
            std::cerr << "[ERROR] device not initialized\n";
            return false;
        }
        rx = static_cast<uint8_t>(~tx);  // dummy 응답: 비트 반전
        std::cout << "[SPI] tx=0x" << std::hex << std::uppercase
                  << static_cast<unsigned>(tx)
                  << " rx=0x" << static_cast<unsigned>(rx)
                  << std::dec << '\n';
        return true;
    }
};

int main() {
    I2cDevice i(0x68, 400000);
    i.write(0x01, 0xFF);   // 실패
    i.begin();
    i.write(0x01, 0xFF);   // 성공

    SpiDevice s(0x10, 5);
    s.begin();
    uint8_t rx = 0;
    s.transfer(0xA5, rx);

    i.status();
    s.status();
    return 0;
}
```

**해설 / 임베디드 관점**
- `protected initialized_`을 두어 파생 클래스가 직접 설정할 수 있도록 했습니다. private이었다면 `protected: void set_initialized(bool);` 같은 보조 함수가 필요했을 것입니다. 디자인 trade-off: 직접 접근은 편하지만, base 변경 시 모든 파생 클래스가 영향받을 수 있습니다.
- `std::hex`, `std::dec`, `std::uppercase`는 stream의 상태를 변경하는 **매니퓰레이터**입니다. 한 번 설정하면 이후 출력에 계속 영향을 줍니다. 본 풀이에서는 사용 후 `std::dec`로 복원했습니다.
- `static_cast<unsigned>(uint8_t)`는 cout에 그대로 출력하면 글자로 처리되는 문제를 피하기 위함입니다.
- `Device` 생성자는 `explicit`. 단일 인자라서 묵시적 변환을 막아야 합니다. 파생 클래스의 다인자 생성자에는 `explicit`이 필수가 아닙니다(C++17까지 다인자 생성자는 묵시적 변환 대상이 아님; 단 C++17의 aggregate 초기화는 별개).
- 실제 임베디드 코드는 `begin()` 안에서 GPIO 초기화, 클럭 설정 등을 수행합니다. 본 풀이는 stdout으로 대체.
- 다음 단계(Day09)에서 `Device`에 `virtual bool begin() = 0;`을 추가하면, `std::vector<Device*>` 같은 컨테이너에 다양한 디바이스를 담아 일괄 초기화할 수 있는 구조가 됩니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `class D : Base` (`public` 누락) | private 상속이 됨, 외부에서 base 인터페이스 사용 불가 |
| 슬라이싱 (`Base b = derived;`) | derived 부분 손실 |
| base 생성자를 명시적으로 호출 안 함 | base의 기본 생성자가 자동 호출, 없으면 컴파일 에러 |
| protected 데이터 멤버 남용 | 캡슐화 약화 |
| derived 안에서 base의 private 멤버 접근 시도 | 컴파일 에러 |
| base 소멸자가 virtual이 아닌 채 `delete base_ptr` | 부분 소멸, 자원 누수 (Day09 주제) |
| 같은 이름 함수 정의로 base 오버로드 가림 | 의도와 다른 호출, `using Base::name;` 필요 |
