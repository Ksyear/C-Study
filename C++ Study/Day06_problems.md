# Day 06 — 연습 문제 (class · 접근제어 · 캡슐화)

> 본 Day에서는 생성자를 아직 다루지 않았으므로, 초기화는 `init(...)` 같은 별도 멤버 함수로 처리합니다. 생성자는 Day07에서 다룹니다.

---

## 문제 1 (쉬움) — `Counter` 클래스

다음 인터페이스의 `Counter` 클래스를 작성하세요.

```cpp
class Counter {
public:
    void init(int start = 0);  // 초기값 설정
    void increment();          // +1
    void decrement();          // -1 (음수로 가도 됨)
    int  get() const;          // 현재값 반환
};
```

요구사항:
- 멤버 변수는 `private`
- main에서 다음을 수행:
  - `Counter c; c.init(); c.increment(); c.increment(); c.decrement();`
  - 마지막에 `c.get()`을 출력 → `1` 이 나와야 함

---

## 문제 2 (중간) — `Stack` (고정 크기)

`std::array<int, 16>`을 내부 저장소로 사용하는 정수 스택을 작성하세요.

```cpp
class Stack {
public:
    void init();
    bool push(int v);     // 가득 차면 false
    bool pop(int& out);   // 비어있으면 false
    bool top(int& out) const;
    std::size_t size() const;
    bool empty() const;
    bool full() const;
};
```

요구사항:
- `private` 멤버: `std::array<int, 16> data_;` `std::size_t size_;`
- pop은 size를 1 감소시키지만 data를 0으로 지울 필요는 없음
- main에서 1, 2, 3, 4, 5를 push 후 모두 pop하여 역순(5, 4, 3, 2, 1) 출력

예시 출력:
```
5 4 3 2 1
```

---

## 문제 3 (임베디드 응용) — `PwmController` 클래스

PWM 제어기를 추상화한 클래스를 작성하세요.

```cpp
class PwmController {
public:
    void init(uint32_t freq_hz);      // 주파수 설정 (1Hz~1MHz 유효)
    bool set_duty(uint8_t duty_pct);  // 0~100 %, 범위 외는 false 반환 & 변경 없음
    void enable();                    // 출력 활성화
    void disable();                   // 출력 비활성화
    uint32_t frequency_hz() const;
    uint8_t  duty_pct() const;
    bool     enabled() const;
};
```

요구사항:
- `<cstdint>` 사용
- 모든 멤버 변수 `private`, 의미 있는 초기 상태 보장(init 호출 전엔 disabled, duty=0)
- 잘못된 freq(0 Hz 또는 1 MHz 초과) 입력 시 `std::cerr` 경고 후 기본값 1 kHz로 설정
- 멤버 함수의 부작용(side effect)을 `std::cout`으로 한 줄씩 출력해 동작 확인 (예: `[PWM] freq=1000 Hz`, `[PWM] duty=50%`, `[PWM] ENABLED`)
- main에서 다음 시나리오 수행:
  1. init(1000)
  2. set_duty(30) → ok
  3. enable()
  4. set_duty(110) → 거부
  5. disable()

예시 출력:
```
[PWM] freq=1000 Hz
[PWM] duty=30%
[PWM] ENABLED
[ERROR] invalid duty: 110
[PWM] DISABLED
freq=1000, duty=30, enabled=false
```

> 힌트: 실제 PWM 드라이버는 멤버 함수 내부에서 레지스터 쓰기를 수행하지만, 여기서는 `std::cout`으로 대체합니다. 실제 코드를 작성할 때는 함수 호출과 레지스터 쓰기를 분리해 두면 단위 테스트가 쉬워집니다.
