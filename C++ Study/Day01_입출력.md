# Day 01 — 입출력 (I/O)

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

C++의 표준 입출력은 **스트림(stream) 객체**인 `std::cin`, `std::cout`, `std::cerr`와 **시프트 연산자 오버로딩**(`<<`, `>>`)을 사용합니다. C의 `printf` / `scanf`와 달리 타입 안전(type-safe)하며, 컴파일 시 타입 검사가 가능합니다.

---

## [개념 설명]

### 1. 헤더와 네임스페이스

표준 입출력 객체는 `<iostream>` 헤더에 정의되어 있으며, 모두 `std` 네임스페이스 안에 있습니다.

```cpp
#include <iostream>
```

### 2. 주요 스트림 객체

| 객체 | 역할 | 버퍼링 |
|------|------|--------|
| `std::cin`  | 표준 입력 (stdin) | line-buffered |
| `std::cout` | 표준 출력 (stdout) | block-buffered (`endl`/플러시 시점에 출력) |
| `std::cerr` | 표준 에러 (stderr) | **unbuffered** (즉시 출력) |
| `std::clog` | 표준 로그 (stderr) | block-buffered |

방산/제어 시스템 로깅에서는 에러 메시지를 `std::cerr`로 보내는 것이 표준 관행입니다. 프로그램이 비정상 종료되어도 버퍼가 비어 있어 메시지가 손실되지 않기 때문입니다.

### 3. 연산자 방향

- `<<` (삽입 연산자): 데이터를 **스트림으로 흘려보낸다** (출력)
- `>>` (추출 연산자): 데이터를 **스트림에서 꺼낸다** (입력)

화살표 방향이 데이터 흐름과 일치한다고 외우면 잊지 않습니다.

### 4. `std::endl` vs `'\n'`

둘 다 줄바꿈을 출력하지만 동작이 다릅니다.

- `'\n'` : 줄바꿈 문자 1개를 버퍼에 쓴다
- `std::endl` : 줄바꿈 문자 + **버퍼 플러시**(flush) 수행

**플러시는 비용이 큽니다.** 임베디드 환경에서 시리얼 출력을 매번 플러시하면 UART 송신을 차단(block)할 수 있습니다. 따라서 **기본은 `'\n'` 사용**, 즉시 출력이 꼭 필요할 때만 `std::endl` 또는 `std::flush`를 사용합니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <string>

int main() {
    // 1. 출력
    std::cout << "Hello, C++!" << '\n';

    // 2. 정수 입력
    int age = 0;
    std::cout << "Enter your age: ";
    std::cin >> age;

    // 3. 문자열 입력 (공백 없는 한 단어)
    std::string name;
    std::cout << "Enter your name: ";
    std::cin >> name;

    // 4. 여러 값 연쇄 출력
    std::cout << "Name=" << name
              << ", Age=" << age
              << '\n';

    // 5. 에러 스트림 사용
    if (age < 0) {
        std::cerr << "[ERROR] Negative age is invalid.\n";
        return 1;
    }

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
Hello, C++!
Enter your age: 23
Enter your name: Seungyeon
Name=Seungyeon, Age=23
```

> 음수 입력 시(`-1`) `std::cerr`로 에러 메시지가 출력되고 종료 코드 1로 종료됩니다.

---

## [동작 원리]

1. `std::cout`은 `std::ostream` 타입의 전역 객체이며, `operator<<`가 각 기본 타입(`int`, `double`, `const char*`, `std::string` 등)에 대해 오버로딩되어 있습니다. 컴파일러는 인자 타입을 보고 적절한 오버로드를 선택합니다.
2. `<<`는 **좌측 피연산자(스트림)에 대한 참조**를 반환합니다. 그래서 `std::cout << a << b << c`처럼 연쇄(chaining)할 수 있습니다.
3. `std::cin >> age`는 입력 버퍼에서 공백/줄바꿈을 만나기 전까지 토큰을 읽어 정수로 파싱하여 `age`에 대입합니다.
4. 파싱 실패 시 스트림은 **fail 상태**가 되어 이후 입력 시도가 모두 실패합니다. 복구하려면 `std::cin.clear()` + `std::cin.ignore(...)`가 필요합니다.

---

## [임베디드 관점]

- **MCU의 시리얼 출력**: STM32, NXP, TI 등의 MCU에서 `printf` 리디렉션 또는 UART 드라이버를 사용하는 경우가 많습니다. C++ 환경에서는 `std::cout`을 `_write` syscall로 우회하여 UART로 연결할 수 있지만, **`<iostream>` 자체가 동적 할당과 locale을 끌어들여 Flash/RAM을 크게 차지**합니다. ARM Cortex-M 같은 작은 MCU에서는 보통 경량 `printf` 구현(예: `tinyprintf`)을 선호합니다.
- **플러시 비용**: ISR이나 실시간 task에서 `std::endl`을 호출하면 UART 전송 완료까지 블로킹될 수 있습니다. 출력 자체를 ISR에서 하지 말 것.
- **`std::cin`은 호스트 시스템에서만 의미가 있음**: MCU에는 stdin이 없으므로 사용자 입력은 보통 GPIO/UART 인터럽트로 처리합니다.
- **로깅 분리**: PC 측 호스트 도구(예: 펌웨어 빌드 스크립트, 시뮬레이터)에서는 정상 출력은 `std::cout`, 진단/에러는 `std::cerr`로 분리하여 파이프라인에서 필터링하기 쉽게 만듭니다.

---

## [주의할 점]

1. **`using namespace std;` 사용 금지.** 명시적으로 `std::cout`처럼 표기하세요. 헤더 파일에 `using namespace std;`가 들어가면 의도치 않은 이름 충돌(예: 사용자 정의 `count` vs `std::count`)을 일으킵니다.
2. **`endl`은 디폴트가 아닙니다.** 매 라인마다 `std::endl`을 쓰는 코드는 안티 패턴입니다. 줄바꿈은 `'\n'`, 플러시가 필요할 때만 `std::endl` 또는 `std::flush`를 사용하세요.
3. **`std::cin >> str` 은 공백을 구분자로 사용**합니다. "Hong Gildong" 같이 공백 포함 문자열은 `std::getline(std::cin, str)`로 받아야 합니다.
4. **혼용 시 주의**: `std::cin >> age` 후 `std::getline`을 호출하면 입력 버퍼에 남은 `'\n'` 때문에 의도와 달리 빈 줄이 읽힙니다. 사이에 `std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');`을 넣으세요.
5. **`scanf`/`printf`와 혼용 금지**: C와 C++ 입출력을 섞으면 버퍼링 순서가 어긋날 수 있습니다. 굳이 섞어야 한다면 `std::ios::sync_with_stdio(true)` 상태를 유지해야 하며, 성능 최적화 목적의 `sync_with_stdio(false)`를 적용한 코드와는 절대 섞지 마세요.

---

## [연습 문제]

연습 문제는 별도 파일을 참고하세요.

- 문제: [`Day01_problems.md`](./Day01_problems.md)
- 정답: [`Day01_solutions.md`](./Day01_solutions.md) — **먼저 직접 풀어본 뒤 확인할 것**
