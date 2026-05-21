# Day 08 — 연습 문제 (상속 · protected)

> 본 Day에서는 아직 `virtual`을 사용하지 않습니다. 다형성과 가상 함수는 Day09에서 다룹니다.

---

## 문제 1 (쉬움) — `Animal` → `Dog`

다음 구조의 클래스를 작성하세요.

```cpp
class Animal {
public:
    Animal(std::string name);
    const std::string& name() const;
    void sleep() const;  // "<name> is sleeping" 출력
};

class Dog : public Animal {
public:
    Dog(std::string name);
    void bark() const;   // "<name> says Woof!" 출력
};
```

요구사항:
- `Animal` 생성자 호출은 `Dog` 생성자의 초기화 리스트에서 수행
- main에서 `Dog d("Rex"); d.bark(); d.sleep();` 실행
- 슬라이싱이 일어나면 안 됨 (참조/포인터 사용 X, 위의 시나리오에서 자연스럽게 발생하지 않음)

예시 출력:
```
Rex says Woof!
Rex is sleeping
```

---

## 문제 2 (중간) — protected 멤버 활용

```cpp
class Vehicle {
protected:
    int speed_kmh_ = 0;   // 파생 클래스가 직접 만질 수 있게 protected
public:
    Vehicle();
    int speed() const;
};

class Car : public Vehicle {
public:
    Car();
    void accelerate(int delta);  // speed_kmh_ += delta, 최대 200으로 클램프
    void brake(int delta);       // speed_kmh_ -= delta, 최소 0으로 클램프
};
```

요구사항:
- `Car`는 base의 `speed_kmh_`에 직접 접근 (`protected` 효과 확인)
- accelerate/brake는 음수 delta를 받지 않도록 검증, 음수면 cerr 출력 후 무시
- main 시나리오:
  - `Car c; c.accelerate(80); c.accelerate(150); c.brake(50); c.brake(-10);`
  - 각 호출 후 speed 출력

예시 출력:
```
speed=80
speed=200
speed=150
[ERROR] delta must be >= 0
speed=150
```

---

## 문제 3 (임베디드 응용) — `Device` → `I2cDevice`, `SpiDevice`

장치 추상화 base 클래스와 두 파생 클래스를 작성하세요.

```cpp
class Device {
protected:
    bool initialized_ = false;
    uint16_t address_;     // 기기 주소
public:
    explicit Device(uint16_t addr);
    bool is_initialized() const;
    uint16_t address() const;
    void status() const;   // "[STATUS] addr=0x<hex> init=<bool>"
};

class I2cDevice : public Device {
private:
    uint32_t clock_hz_;
public:
    I2cDevice(uint16_t addr, uint32_t clock_hz);
    bool begin();   // initialized_ = true, "[I2C] addr=... clock=... started" 출력
    bool write(uint8_t reg, uint8_t value);  // init 안되어 있으면 false 반환
};

class SpiDevice : public Device {
private:
    uint8_t cs_pin_;
public:
    SpiDevice(uint16_t addr, uint8_t cs_pin);
    bool begin();   // initialized_ = true, "[SPI] addr=... cs=... started" 출력
    bool transfer(uint8_t tx, uint8_t& rx);  // init 안되어 있으면 false; rx = ~tx (dummy)
};
```

요구사항:
- `<cstdint>` 사용
- 주소 출력은 16진수 (`std::hex`)
- `initialized_`는 protected 멤버로 두어 파생 클래스가 직접 설정 가능
- `begin()` 호출 전에 `write`/`transfer`를 호출하면 cerr 출력 후 false
- main 시나리오:
  1. `I2cDevice i(0x68, 400000);`
  2. `i.write(0x01, 0xFF);` (실패해야 함)
  3. `i.begin();`
  4. `i.write(0x01, 0xFF);` (성공)
  5. `SpiDevice s(0x10, 5); s.begin();`
  6. `uint8_t rx; s.transfer(0xA5, rx);` 후 rx 값(0x5A) 출력
  7. 두 디바이스 모두 `status()` 호출

예시 출력 (요지):
```
[ERROR] device not initialized
[I2C] addr=0x68 clock=400000 started
[I2C] write reg=0x1 val=0xFF
[SPI] addr=0x10 cs=5 started
[SPI] tx=0xA5 rx=0x5A
[STATUS] addr=0x68 init=true
[STATUS] addr=0x10 init=true
```

> 힌트: 실제 I2C/SPI 드라이버는 base의 멤버 변수만 공유하고, 통신 프로토콜 자체는 파생 클래스에서 구현하는 구조가 흔합니다. 가상 함수를 쓰면 일반화된 `Device*` 컨테이너로 다룰 수 있지만, 그것은 Day09의 주제입니다.
