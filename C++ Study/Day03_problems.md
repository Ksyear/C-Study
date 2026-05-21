# Day 03 — 연습 문제 (반복문)

---

## 문제 1 (쉬움) — 1부터 N까지 합

양의 정수 N을 입력받아 1부터 N까지의 합을 출력하세요.

```
Enter N: 10
Sum = 55
```

요구사항:
- `for` 반복문 사용
- N이 0 이하이면 에러 출력 후 종료

---

## 문제 2 (중간) — 소수(prime) 판정

정수 N을 입력받아 2 이상이고 N 이하인 소수를 모두 한 줄에 공백으로 구분해 출력하세요.

```
Enter N: 30
2 3 5 7 11 13 17 19 23 29
```

요구사항:
- 이중 `for` 루프 또는 함수 분리 (Day05에서 함수 배우므로 일단 main 안에서 처리 가능)
- 효율성을 위해 약수 검사는 `i * i <= n`까지만 (`sqrt` 호출 회피)

---

## 문제 3 (임베디드 응용) — 단순 PWM 듀티 사이클 시뮬레이션

가상의 PWM 출력을 시뮬레이션합니다. 다음 입력을 받으세요.

- 총 사이클 수 `cycles` (양의 정수)
- 듀티 사이클 `duty` (0~100, 정수, %)

각 사이클은 100 ticks로 구성된다고 가정하고, 다음과 같이 출력하세요.

- 매 사이클마다: tick 0 ~ tick(duty-1)은 `H`, tick duty ~ tick 99는 `L`를 1개의 문자열로 만들어 출력
- 모든 사이클이 끝난 뒤 `H` 개수의 총합, `L` 개수의 총합, H 비율 출력

예시 (`cycles=2, duty=30`):
```
Cycle 1: HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
Cycle 2: HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
Total H = 60, L = 140, H ratio = 30.0%
```

요구사항:
- 중첩 반복문 사용
- duty 범위 검증
- 100 ticks의 한 사이클은 가독성을 위해 한 줄에 출력 (총 100문자)
- 비율은 소수점 한 자리까지

> 힌트: 실제 임베디드 PWM은 타이머 하드웨어가 처리하지만, **로직 검증을 위해 소프트웨어로 흉내내는 시뮬레이션**은 종종 작성합니다. 입력값으로 듀티가 0 또는 100인 극단 케이스도 동작해야 합니다.
