# Day 06 — 연습 문제 정답 (class · 접근제어 · 캡슐화)

---

## 문제 1 정답

```cpp
#include <iostream>

class Counter {
private:
    int value_ = 0;  // C++11 멤버 기본 초기화 (NSDMI)

public:
    void init(int start = 0) { value_ = start; }
    void increment()         { ++value_; }
    void decrement()         { --value_; }
    int  get() const         { return value_; }
};

int main() {
    Counter c;
    c.init();
    c.increment();
    c.increment();
    c.decrement();
    std::cout << c.get() << '\n';
    return 0;
}
```

**해설**
- `int value_ = 0;`는 C++11의 **non-static data member initializer(NSDMI)**입니다. 생성자가 없어도 멤버 초기값을 보장합니다.
- 실제로는 생성자(Day07)로 초기화하는 것이 더 자연스럽지만, 본 Day의 학습 범위에 맞춰 `init()`을 별도로 두었습니다.
- `init` 호출을 잊으면 `value_`가 0 그대로지만, 만약 NSDMI가 없었다면 미초기화 상태로 UB. 멤버 변수 기본 초기화는 안전을 위해 거의 항상 적용하는 것이 좋습니다.

---

## 문제 2 정답

```cpp
#include <iostream>
#include <array>
#include <cstddef>

class Stack {
private:
    static constexpr std::size_t kCap = 16;
    std::array<int, kCap> data_{};
    std::size_t size_ = 0;

public:
    void init() {
        size_ = 0;
        // data_는 그대로 두어도 의미 없음 (의도적)
    }

    bool push(int v) {
        if (size_ >= kCap) return false;
        data_[size_++] = v;
        return true;
    }

    bool pop(int& out) {
        if (size_ == 0) return false;
        out = data_[--size_];
        return true;
    }

    bool top(int& out) const {
        if (size_ == 0) return false;
        out = data_[size_ - 1];
        return true;
    }

    std::size_t size() const { return size_; }
    bool empty() const       { return size_ == 0; }
    bool full() const        { return size_ == kCap; }
};

int main() {
    Stack s;
    s.init();
    for (int i = 1; i <= 5; ++i) {
        s.push(i);
    }

    while (!s.empty()) {
        int v = 0;
        s.pop(v);
        std::cout << v << ' ';
    }
    std::cout << '\n';
    return 0;
}
```

**해설**
- `kCap`을 `static constexpr`로 두면 모든 객체가 공유하는 컴파일 타임 상수가 됩니다. 메모리 차지 없음.
- `pop`은 out 매개변수로 결과를 반환합니다. `int pop()`이 더 간결하지만, 빈 스택에서의 처리(예외 또는 옵셔널)가 필요해 매개변수 방식을 택했습니다. C++17 `std::optional<int>`도 좋은 선택입니다.
- 임베디드 임베디드 관점: 고정 크기 스택은 메모리가 정해져 있어 결정성이 좋습니다. 단, capacity가 부족할 수 있으므로 push 실패를 적절히 처리해야 합니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <cstdint>

class PwmController {
private:
    uint32_t freq_hz_ = 0;
    uint8_t  duty_pct_ = 0;
    bool     enabled_ = false;

    static constexpr uint32_t kFreqMin = 1;
    static constexpr uint32_t kFreqMax = 1'000'000;
    static constexpr uint32_t kFreqDefault = 1'000;

public:
    void init(uint32_t freq_hz) {
        if (freq_hz < kFreqMin || freq_hz > kFreqMax) {
            std::cerr << "[ERROR] invalid freq: " << freq_hz
                      << ", fallback to " << kFreqDefault << '\n';
            freq_hz = kFreqDefault;
        }
        freq_hz_ = freq_hz;
        duty_pct_ = 0;
        enabled_ = false;
        std::cout << "[PWM] freq=" << freq_hz_ << " Hz\n";
    }

    bool set_duty(uint8_t duty_pct) {
        if (duty_pct > 100) {
            std::cerr << "[ERROR] invalid duty: "
                      << static_cast<unsigned>(duty_pct) << '\n';
            return false;
        }
        duty_pct_ = duty_pct;
        std::cout << "[PWM] duty=" << static_cast<unsigned>(duty_pct_) << "%\n";
        return true;
    }

    void enable() {
        enabled_ = true;
        std::cout << "[PWM] ENABLED\n";
    }

    void disable() {
        enabled_ = false;
        std::cout << "[PWM] DISABLED\n";
    }

    uint32_t frequency_hz() const { return freq_hz_; }
    uint8_t  duty_pct()    const { return duty_pct_; }
    bool     enabled()     const { return enabled_; }
};

int main() {
    PwmController pwm;
    pwm.init(1000);
    pwm.set_duty(30);
    pwm.enable();
    pwm.set_duty(110);  // 거부
    pwm.disable();

    std::cout << "freq=" << pwm.frequency_hz()
              << ", duty=" << static_cast<unsigned>(pwm.duty_pct())
              << ", enabled=" << std::boolalpha << pwm.enabled() << '\n';
    return 0;
}
```

**해설 / 임베디드 관점**
- `uint8_t`는 `std::cout`에 그대로 넘기면 `char`로 해석되어 글자가 출력됩니다. `static_cast<unsigned>(duty_pct_)`로 변환해야 숫자로 출력됩니다. 이는 임베디드 C++의 흔한 출력 함정입니다.
- 멤버 변수의 기본값(NSDMI)으로 `enabled_ = false`, `duty_pct_ = 0`을 두어 init 전에도 의미 있는 상태를 유지합니다.
- 실제 PWM 드라이버는 set_duty 안에서 `TIM->CCR1 = ...` 같은 레지스터 쓰기를 수행합니다. 이 부분만 분리하면 본 클래스를 호스트 PC에서 단위 테스트할 수 있습니다(레지스터 쓰기를 mock으로 대체).
- 1'000'000 같은 숫자 구분자(C++14)는 가독성을 위한 것으로 컴파일에 영향을 주지 않습니다.
- 클래스 인터페이스 측면: `set_duty` 같은 함수는 성공/실패를 `bool`로 반환합니다. 가능하면 `[[nodiscard]]`를 붙여 호출자가 결과를 무시하지 않도록 강제하는 것이 안전합니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| 멤버 변수를 모두 public | 캡슐화 무의미, 불변식 보장 불가 |
| getter를 non-const | const 객체에서 호출 불가 |
| NSDMI 없이 init 호출 안 함 | 미초기화 멤버, UB |
| `uint8_t`를 cout에 그대로 출력 | 문자로 출력됨 |
| `kCap`을 매크로로 정의 | 타입/스코프 없음. `static constexpr` 권장 |
| 모든 멤버에 기계적으로 getter/setter | 객체에 일을 시키지 않고 데이터만 노출 |
