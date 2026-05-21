# Day 02 — 연습 문제 정답 (조건문)

---

## 문제 1 정답

```cpp
#include <iostream>

int main() {
    int score = 0;
    std::cout << "Enter score(0-100): ";
    std::cin >> score;

    if (score < 0 || score > 100) {
        std::cerr << "[ERROR] score out of range: " << score << '\n';
        return 1;
    }

    char grade = 'F';
    if (score >= 90)      grade = 'A';
    else if (score >= 80) grade = 'B';
    else if (score >= 70) grade = 'C';
    else if (score >= 60) grade = 'D';
    // else: 'F' 유지

    std::cout << "Grade: " << grade << '\n';
    return 0;
}
```

**해설**
- 조건의 순서가 중요합니다. 큰 값부터 검사하면 각 조건이 단순해집니다. 반대로 작은 값부터 검사하려면 `score < 60` → `score < 70` 식으로 짜야 합니다.
- 범위 검사를 `if (0 <= score <= 100)`처럼 쓰면 안 됩니다. C/C++는 `0 <= score`가 먼저 평가되어 `bool` (0 또는 1)이 되고, 그것이 `<= 100`과 비교되어 항상 참이 됩니다. 반드시 `&&`로 결합하세요.

---

## 문제 2 정답

```cpp
#include <iostream>

enum class LedState { Off, On, Blink, Error };

int main() {
    int cmd = 0;
    std::cout << "Enter command(0=Off,1=On,2=Blink,3=Error): ";
    std::cin >> cmd;

    if (cmd < 0 || cmd > 3) {
        std::cerr << "[ERROR] invalid command: " << cmd << '\n';
        return 1;
    }

    LedState state = static_cast<LedState>(cmd);

    switch (state) {
        case LedState::Off:
            std::cout << "LED OFF\n";
            break;
        case LedState::On:
            std::cout << "LED ON\n";
            break;
        case LedState::Blink:
            std::cout << "LED BLINKING at 1 Hz\n";
            break;
        case LedState::Error:
            std::cerr << "[ERROR] LED driver fault\n";
            return 1;
    }
    return 0;
}
```

**해설**
- `int` → `enum class`는 **암시적 변환이 안 되므로** `static_cast<LedState>(cmd)`로 명시 변환해야 합니다. 이 명시성이 `enum class`의 안전성입니다.
- 범위를 미리 검사했으므로 `static_cast`가 안전하지만, 일반적으로는 enum 값에 매핑된 모든 정수가 유효한지 검증해야 합니다.
- 모든 case를 명시했기 때문에 `default`가 없어도 됩니다. clang의 `-Wswitch-enum`은 누락된 case를 경고로 알려줍니다.

---

## 문제 3 정답

```cpp
#include <iostream>

int main() {
    std::cout << "Enter battery percent and charger(0/1): ";
    int battery = 0, charger = 0;
    std::cin >> battery >> charger;

    // 입력 검증
    if (battery < 0 || battery > 100 || (charger != 0 && charger != 1)) {
        std::cerr << "[ERROR] invalid input: battery=" << battery
                  << ", charger=" << charger << '\n';
        return 1;
    }

    // C++17 if-init: 충전기 연결 여부를 bool로 캐시
    if (const bool plugged = (charger == 1); !plugged && battery < 5) {
        std::cerr << "[CRITICAL] shutting down\n";
        return 2;
    } else if (!plugged && battery < 20) {
        std::cout << "LOW_POWER_MODE\n";
    } else if (plugged && battery < 100) {
        std::cout << "CHARGING\n";
    } else if (plugged && battery == 100) {
        std::cout << "FULLY_CHARGED\n";
    } else {
        std::cout << "NORMAL\n";
    }
    return 0;
}
```

**해설 / 임베디드 관점**
- if-init 안의 `plugged`는 모든 `else if` 블록에서 동일한 스코프로 보입니다. 변수의 수명을 if 체인에 한정시키는 좋은 사용 예입니다.
- 우선순위가 정해진 분기 규칙은 if-else 체인 구조가 가장 명확합니다. switch로는 표현하기 어렵습니다.
- 실제 BMS(Battery Management System)에서는 잔량 외에도 전압/전류/온도/내부저항을 함께 보지만, 조건문 학습 목적이므로 단순화했습니다.
- `int charger` 대신 `bool charger`로 입력받을 수도 있지만 `std::cin >> b`로 bool을 읽으면 "0"/"1"만 받고 "true"/"false"는 받지 못한다는 점을 인지하세요(`std::boolalpha` 매니퓰레이터로 활성화 가능).

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| `if (0 <= x <= 100)` | 항상 참. `(x >= 0 && x <= 100)`로 작성 |
| `if (x = 5)` | 대입 + 5 평가, 항상 참 |
| `switch` `break` 누락 | fall-through 버그 |
| `int` → `enum class` 암시적 변환 시도 | 컴파일 에러 (다행히도) |
| `if-else` 마지막 `else` 누락 | 미처리 입력 존재 가능성 |
