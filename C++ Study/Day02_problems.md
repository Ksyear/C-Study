# Day 02 — 연습 문제 (조건문)

---

## 문제 1 (쉬움) — 학점 판정

정수 점수(0~100)를 입력받아 다음 기준으로 학점을 출력하세요.

| 점수 | 학점 |
|------|------|
| 90 이상 | A |
| 80 이상 90 미만 | B |
| 70 이상 80 미만 | C |
| 60 이상 70 미만 | D |
| 그 외 | F |

요구사항:
- `if`-`else if`-`else` 사용
- 입력값이 0~100 범위를 벗어나면 `std::cerr`로 에러 출력 후 종료 코드 1로 종료

---

## 문제 2 (중간) — 상태 머신 (`enum class` + `switch`)

다음 상태를 가진 단순 LED 제어기를 구현하세요.

```cpp
enum class LedState { Off, On, Blink, Error };
```

사용자로부터 정수 명령(0=Off, 1=On, 2=Blink, 3=Error)을 입력받아 `LedState`로 변환한 뒤, `switch`로 분기해 다음 메시지를 출력하세요.

| 상태 | 출력 |
|------|------|
| Off | `LED OFF` |
| On | `LED ON` |
| Blink | `LED BLINKING at 1 Hz` |
| Error | `[ERROR] LED driver fault` (cerr로 출력) |

요구사항:
- 0~3 외의 입력은 에러 처리
- `enum class` 사용 (`enum` 금지)
- `switch` 안에서 가능한 모든 case를 명시 (default 사용 가능)

---

## 문제 3 (임베디드 응용) — 배터리 상태 + 충전기 연결 조건 분기

다음 두 입력을 받아 동작 모드를 결정하세요.

- 배터리 잔량(`int`, 0~100, %)
- 충전기 연결 여부(`int`, 0=disconnected, 1=connected)

판정 규칙 (위에서부터 우선):

1. 잔량 < 5% 그리고 충전기 disconnected → `CRITICAL_SHUTDOWN` (cerr로 출력)
2. 잔량 < 20% 그리고 충전기 disconnected → `LOW_POWER_MODE`
3. 충전기 connected 그리고 잔량 < 100% → `CHARGING`
4. 충전기 connected 그리고 잔량 == 100% → `FULLY_CHARGED`
5. 그 외 → `NORMAL`

요구사항:
- C++17 if-init 문법(`if (auto x = ...; cond)`)을 한 번 이상 사용해 볼 것
- 잘못된 입력값(범위 외)도 cerr로 처리

예시:
```
Enter battery percent and charger(0/1): 3 0
[CRITICAL] shutting down
```

```
Enter battery percent and charger(0/1): 100 1
FULLY_CHARGED
```

> 힌트: 조건의 **우선순위**가 중요합니다. 위쪽에서 우선 매칭되는 조건을 먼저 처리하면 아래 조건들은 자동으로 제외됩니다.
