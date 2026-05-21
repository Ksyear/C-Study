# Day 01 — 연습 문제 (입출력)

각 문제마다 `clang++ -std=c++17 -Wall -Wextra main.cpp -o main` 으로 컴파일 가능한 **완전한 코드**를 작성하세요.

---

## 문제 1 (쉬움) — 인사 출력

사용자로부터 이름(한 단어)을 입력받아 다음 형식으로 출력하는 프로그램을 작성하세요.

```
Enter your name: Seungyeon
Hello, Seungyeon!
```

요구사항:
- `std::cin`, `std::cout` 사용
- `std::string` 사용
- 줄바꿈은 `'\n'` 사용 (`std::endl` 금지)

---

## 문제 2 (중간) — 두 정수의 사칙연산

두 정수를 입력받아 `a + b`, `a - b`, `a * b`, `a / b`, `a % b`를 각각 한 줄씩 출력하세요.

```
Enter two integers: 10 3
10 + 3 = 13
10 - 3 = 7
10 * 3 = 30
10 / 3 = 3
10 % 3 = 1
```

요구사항:
- `b == 0`이면 나눗셈/나머지 출력 대신 `std::cerr`로 에러를 출력하고 종료 코드 1로 종료
- 한 줄에 두 정수를 공백으로 구분해 입력받을 것 (`std::cin >> a >> b`)

---

## 문제 3 (임베디드 응용) — 센서 값 입력과 임계치 판정

가상의 온도 센서로부터 측정값(섭씨, `double`)을 5회 입력받아 다음 정보를 출력하세요.

- 5개 값의 평균
- 임계치 `85.0°C`를 초과한 횟수
- 임계치 초과 시점마다 즉시 `std::cerr`로 다음 형식의 경고를 출력:
  `[WARN] sample #N exceeded threshold: <value> C`

예시:
```
Enter 5 temperature samples:
70.5 82.0 86.3 90.1 75.0
[WARN] sample #3 exceeded threshold: 86.3 C
[WARN] sample #4 exceeded threshold: 90.1 C
Average: 80.78 C
Exceeded count: 2
```

요구사항:
- 5개 입력을 받기 위해 `for` 반복문 사용 (배열은 아직 안 배웠으므로 변수 누적만 사용)
- 경고 출력은 `std::cerr`, 정상 출력은 `std::cout`
- `endl` 사용 금지, `'\n'` 사용

> 힌트: 임베디드 시스템에서 센서 데이터를 처리할 때 **모든 샘플을 저장하지 않고 누적값/카운터만 유지**하는 것이 메모리 절약의 기본 패턴입니다.
