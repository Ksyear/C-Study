# Day 02 — 조건문

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

조건문은 **불리언 표현식의 결과(true/false)**에 따라 실행 흐름을 분기합니다. C++에서는 `if` / `else if` / `else`, `switch`, 삼항 연산자 `?:` 세 가지가 기본이며, C++17부터 `if`/`switch`에 **초기화식**을 직접 넣을 수 있는 문법이 추가되었습니다.

---

## [개념 설명]

### 1. `if` / `else if` / `else`

```cpp
if (condition1) {
    // condition1이 true일 때
} else if (condition2) {
    // condition1이 false, condition2가 true일 때
} else {
    // 모두 false일 때
}
```

- 조건은 **`bool`로 변환 가능한 표현식**이어야 합니다. `int`, 포인터 등도 묵시적으로 `bool`로 변환됩니다 (0/`nullptr`이 `false`, 그 외 `true`).
- C에서 흔히 보던 `if (ptr != NULL)` 대신 C++에서는 `if (ptr != nullptr)` 또는 `if (ptr)`로 씁니다. **`NULL`은 사용 금지**.

### 2. C++17 초기화식이 포함된 `if`

```cpp
if (int rc = read_sensor(); rc == 0) {
    // 성공 경로
} else {
    std::cerr << "read_sensor failed: " << rc << '\n';
}
// 여기서 rc는 사용 불가 (스코프 종료)
```

장점: `rc`의 **수명(lifetime)을 if 블록으로 제한**하여 캡슐화. 임베디드 코드에서 status 코드를 다룰 때 매우 유용합니다.

### 3. `switch`

```cpp
switch (value) {
    case 1:
        // ...
        break;
    case 2:
    case 3:
        // 2 또는 3
        break;
    default:
        // 그 외
        break;
}
```

- 비교 대상은 **정수형(`int`, `char`, `enum`, `enum class`)**만 가능. `float`/`double`/`std::string`은 사용 불가.
- `break`를 빠뜨리면 **fall-through**가 발생해 다음 `case`로 실행이 이어집니다. 의도된 fall-through라면 C++17의 `[[fallthrough]];` 속성으로 명시하세요.

```cpp
switch (state) {
    case INIT:
        init_hw();
        [[fallthrough]];  // 컴파일러 경고 억제
    case RUNNING:
        run();
        break;
    case ERROR:
        log_error();
        break;
}
```

### 4. 삼항 연산자 `?:`

```cpp
int abs_val = (x < 0) ? -x : x;
```

- **표현식**이므로 값을 반환합니다. `if`는 문장이라 값을 반환하지 않습니다.
- 가독성이 떨어지는 중첩 삼항(`a ? b : c ? d : e`)은 피하고 `if`-`else`로 풀어 쓰는 편이 안전합니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>

enum class FanLevel { Off, Low, Mid, High };

const char* level_to_str(FanLevel lv) {
    switch (lv) {
        case FanLevel::Off:  return "OFF";
        case FanLevel::Low:  return "LOW";
        case FanLevel::Mid:  return "MID";
        case FanLevel::High: return "HIGH";
    }
    return "UNKNOWN";  // 모든 case가 cover되었지만 컴파일러 경고 회피용
}

FanLevel decide_fan(double temperature_c) {
    if (temperature_c < 30.0) {
        return FanLevel::Off;
    } else if (temperature_c < 50.0) {
        return FanLevel::Low;
    } else if (temperature_c < 70.0) {
        return FanLevel::Mid;
    } else {
        return FanLevel::High;
    }
}

int main() {
    double temp = 0.0;
    std::cout << "Enter temperature(C): ";
    std::cin >> temp;

    // 입력 검증을 if-init 으로 깔끔하게
    if (bool ok = std::cin.good(); !ok) {
        std::cerr << "[ERROR] invalid input\n";
        return 1;
    }

    FanLevel level = decide_fan(temp);
    std::cout << "Fan level: " << level_to_str(level) << '\n';

    // 삼항 연산자 예시
    const char* state = (temp >= 70.0) ? "CRITICAL" : "NORMAL";
    std::cout << "State: " << state << '\n';
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
Enter temperature(C): 65
Fan level: MID
State: NORMAL
```

```
Enter temperature(C): 85
Fan level: HIGH
State: CRITICAL
```

---

## [동작 원리]

1. `decide_fan` 함수는 `if`-`else if` 체인으로 입력 범위를 좁혀가며 분기합니다. **위에서부터 평가되며, 처음 true가 되는 가지가 실행**된 뒤 나머지는 평가되지 않습니다(short-circuit).
2. `level_to_str`은 `switch`로 enum 값을 문자열로 매핑합니다. `enum class`는 정수로 자동 변환되지 않지만 `switch` 안에서는 자연스럽게 사용 가능합니다.
3. `bool ok = std::cin.good(); !ok` 구문은 C++17 if-init 문법으로, `ok`의 스코프가 if 블록에 한정됩니다.
4. `enum class`의 `switch`는 컴파일러가 모든 case를 cover하는지 검사할 수 있습니다(`-Wswitch-enum` 활성화 시). **default를 두지 않고 모든 case를 명시**하면 enum이 확장됐을 때 컴파일러가 경고로 알려줍니다.

---

## [임베디드 관점]

- **상태 머신(state machine)**: MCU 펌웨어는 본질적으로 상태 머신이며 `switch`로 구현하는 경우가 많습니다. `enum class`로 상태를 정의하고 `switch`로 분기하면 상태 추가 시 컴파일러가 누락된 case를 잡아줍니다.
- **분기 예측(branch prediction)**: Cortex-M 같은 임베디드 CPU는 PC급 CPU보다 분기 예측이 단순합니다. 따라서 **빈도가 높은 케이스를 위쪽에 배치**하는 것이 성능에 유리합니다. 단, 이것은 마이크로 최적화이므로 측정 후 적용하세요.
- **`switch` jump table**: 컴파일러는 case 값이 조밀하면 jump table을 생성합니다. 이는 if-else 체인보다 빠르지만, Flash 메모리에 jump table을 저장하므로 메모리가 빠듯하면 trade-off가 됩니다.
- **부동소수점 비교**: 임베디드에서 `if (x == 1.0)` 같은 부동소수점 동등 비교는 거의 항상 잘못된 것입니다. `if (std::fabs(x - 1.0) < kEpsilon)` 형식으로 비교하거나 가능하면 정수/고정소수점 연산을 사용하세요.

---

## [주의할 점]

1. **`=` vs `==`**: `if (x = 0)`은 대입 후 평가입니다. 항상 false (또는 nonzero일 때 true). 컴파일러 경고를 활성화하면 잡힙니다.
2. **`switch` fall-through**: `break` 누락은 가장 흔한 버그입니다. clang은 `-Wimplicit-fallthrough`로 경고를 내며, 의도적이면 `[[fallthrough]];`를 명시하세요.
3. **`switch`에 `std::string`/`double` 사용 불가**: 컴파일 에러입니다. `if`-`else if` 체인 또는 `std::unordered_map`을 사용하세요.
4. **삼항 연산자의 타입 통일**: `cond ? 1 : 1.0`은 결과 타입이 `double`로 promotion됩니다. 의도하지 않은 암시적 변환을 만들 수 있습니다.
5. **`if`-`else if` 체인의 범위 누락**: `if (x < 10) ... else if (x > 20) ...`처럼 작성하면 `10 <= x <= 20` 구간이 누락됩니다. **모든 가능한 입력을 cover하는지** 항상 확인하세요. `else` 블록을 두는 습관이 좋습니다.
6. **`enum class`와 정수 비교 금지**: `if (level == 2)`는 컴파일 에러. `if (level == FanLevel::Mid)`로 명시해야 합니다. 이것이 `enum class`의 장점입니다.

---

## [연습 문제]

- 문제: [`Day02_problems.md`](./Day02_problems.md)
- 정답: [`Day02_solutions.md`](./Day02_solutions.md)
