# C++ Study — 10일 학습 로드맵

목표: 임베디드(방산/제어/센서/실시간) 방향의 C++ 기초 다지기
하루 학습 시간: 약 60분 (개념 20분 → 예제 따라치기 20분 → 문제 풀이 20분)
표준: **C++17** (필요 시 C++20 언급)
컴파일러: **clang++** (macOS)

---

## 학습 순서

| Day | 주제 | 키 키워드 |
|-----|------|-----------|
| Day01 | 입출력 | `std::cin`, `std::cout`, `std::cerr`, `<<`, `>>`, `\n` vs `std::endl` |
| Day02 | 조건문 | `if`, `else if`, `else`, `switch`, 삼항 연산자 |
| Day03 | 반복문 | `for`, `while`, `do-while`, range-based `for`, `break`, `continue` |
| Day04 | 배열 | C-style 배열, `std::array`, `std::vector` 기초 |
| Day05 | 함수 | 선언/정의, 값/참조/`const&` 전달, 오버로딩, 기본 인자 |
| Day06 | class · public/private · 캡슐화 | `class` vs `struct`, 접근 제어, 캡슐화 |
| Day07 | 생성자 · getter/setter | 기본/매개변수 생성자, 초기화 리스트, 접근자 패턴 |
| Day08 | 상속 · protected | `public` 상속, `protected`, slicing |
| Day09 | 다형성 (virtual / override / 추상 클래스) | `virtual`, `override`, 순수 가상 함수, vtable, 가상 소멸자 |
| Day10 | 예외 처리 | `try` / `catch` / `throw`, `std::runtime_error`, `noexcept` |

---

## 매일 학습 흐름 (1시간 분량)

각 Day 문서는 다음 형식으로 구성됩니다.

1. **[핵심 요약]** — 결론부터 한 줄로
2. **[개념 설명]** — 단계적 설명, 메모리/수명/컴파일 관점 연결
3. **[예제 코드]** — 컴파일 가능한 전체 코드 (C++17)
4. **[실행 방법]** — `clang++ -std=c++17 main.cpp -o main` 형식
5. **[실행 결과]** — 예상 출력
6. **[동작 원리]** — 왜 그렇게 동작하는지
7. **[임베디드 관점]** — MCU/RTOS/센서/레지스터/메모리 제약과의 연결
8. **[주의할 점]** — 자주 하는 실수, 안티 패턴, UB 가능성
9. **[연습 문제]** — 별도 `DayNN_problems.md` 파일에 분리

연습 문제 정답은 `DayNN_solutions.md`에 별도로 제공되지만, **먼저 직접 풀어보고 나서** 확인하세요.

---

## 파일 구조

```
C++ Study/
├── INDEX.md                  # 본 문서
├── Day01_입출력.md
├── Day01_problems.md
├── Day01_solutions.md
├── Day02_조건문.md
├── Day02_problems.md
├── Day02_solutions.md
... (Day10까지 동일 패턴)
```

---

## 권장 학습 환경 (macOS)

```bash
# 컴파일러 버전 확인
clang++ --version

# 권장 컴파일 옵션 (학습 단계에서는 경고를 적극 활성화)
clang++ -std=c++17 -Wall -Wextra -Wpedantic -O0 -g main.cpp -o main
./main
```

- `-Wall -Wextra -Wpedantic` : 경고를 통한 학습 효과 극대화
- `-O0 -g` : 디버깅 정보 포함, 최적화 없음 (디버거에서 변수 추적 용이)

---

## 학습 후 다음 단계 (이 로드맵 이후)

본 10일 과정 이후 다루기를 권장하는 주제:

- `const`, `constexpr`, `static`의 정확한 의미
- 참조자(reference) 심화, 포인터와의 비교
- 복사/이동 생성자, 복사/이동 대입 연산자 (Rule of Five)
- RAII, 스마트 포인터 (`std::unique_ptr`, `std::shared_ptr`)
- 템플릿 기초
- `enum class`, 비트 연산
- 구조체 패킹과 정렬, `alignas`, `alignof`
- 임베디드 핵심: `volatile`, `atomic`, ISR 관점의 C++
- 멀티스레딩 기초 (`std::thread`, `std::mutex`)
- CMake 기초

---

## 사용 방법

- `Day01_입출력.md` 부터 순서대로 진행
- 예제 코드는 반드시 **직접 타이핑하여** 컴파일 → 실행
- `[연습 문제]` 풀이 전에 본문 예제를 변형해 보는 습관을 들일 것
- "정답 확인해줘"라고 요청하면 작성한 코드를 검토받을 수 있음
