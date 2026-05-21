# Day 10 — 연습 문제 (예외 처리)

---

## 문제 1 (쉬움) — 안전한 나눗셈

다음 함수를 작성하세요.

```cpp
int safe_div(int a, int b);
```

요구사항:
- `b == 0`이면 `std::runtime_error("division by zero")` 던짐
- 그 외엔 `a / b` 반환
- main에서 두 정수 입력 받아 try/catch로 호출하고 결과 또는 에러 메시지 출력

예시:
```
Enter a b: 10 0
[ERROR] division by zero
```

```
Enter a b: 10 3
result = 3
```

---

## 문제 2 (중간) — 사용자 정의 예외 + 다단 catch

다음 클래스를 작성하세요.

```cpp
class TempOutOfRange : public std::out_of_range {
public:
    TempOutOfRange(double value);   // 메시지: "temperature out of range: <value>"
    double value() const noexcept;
private:
    double value_;
};
```

다음 함수도 작성:

```cpp
double process_temp(double c);   // -50 ~ 200 외이면 TempOutOfRange 던짐, 그 외는 c * 1.8 + 32 (화씨 변환)
```

main에서:
- `std::vector<double> samples = {25.0, 150.0, -100.0, 300.0, 0.0};`
- 각 값에 대해 try/catch로 호출. catch 블록:
  - `TempOutOfRange` 잡으면 `[TOR] <value> degC out of range` 출력
  - `std::exception` 잡으면 `[EXC] <what>` 출력
- 두 catch가 모두 도달 가능하도록 순서를 적절히 배치

예시 출력:
```
F = 77
F = 302
[TOR] -100 degC out of range
[TOR] 300 degC out of range
F = 32
```

---

## 문제 3 (임베디드 응용) — RAII + 예외 안전성

다음 클래스를 작성하세요.

```cpp
class HwResource {
public:
    HwResource(const std::string& name);   // 생성자에서 "[acquire] <name>" 출력
    ~HwResource();                          // 소멸자에서 "[release] <name>" 출력
    void use(bool fail);                    // fail이 true이면 runtime_error 던짐
private:
    std::string name_;
};
```

main에서 다음 시나리오를 try/catch로 감싸 실행하세요:

1. `HwResource a("UART"); HwResource b("SPI"); HwResource c("I2C");` 순차 생성
2. `a.use(false);` `b.use(false);` `c.use(true);` 호출 (마지막에서 예외)
3. catch에서 메시지 출력

**검증 포인트**: 예외 발생 시점 이후의 코드가 실행되지 않아도, **세 자원 모두 정확히 release되는지** 출력 순서로 확인.

예시 출력:
```
[acquire] UART
[acquire] SPI
[acquire] I2C
[use OK] UART
[use OK] SPI
[release] I2C
[release] SPI
[release] UART
[caught] device fault
```

> 힌트: `HwResource c`가 예외를 던졌을 때, c는 **아직 본문에서 던졌으므로 c의 소멸자도 호출**됩니다 (생성자에서 던졌다면 c의 소멸자는 호출되지 않음). 본 문제에서는 `use()`에서 던지므로 c의 소멸자가 호출됩니다. 스택 풀기 순서를 잘 관찰하세요.

> 임베디드 관점: 실제 펌웨어에서 SPI/I2C 같은 자원은 RAII로 관리하면 어떤 경로로 함수가 종료되든(early return, 예외, abort) 자원이 해제됩니다. 하지만 `-fno-exceptions` 환경에서는 예외 대신 early return으로 대체합니다.
