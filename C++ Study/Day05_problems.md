# Day 05 — 연습 문제 (함수)

---

## 문제 1 (쉬움) — 최대공약수(GCD)

두 양의 정수를 입력받아 최대공약수를 반환하는 함수 `int gcd(int a, int b)`를 작성하고 main에서 호출하세요.

요구사항:
- 유클리드 호제법 사용 (반복 또는 재귀)
- 0 이하 입력은 main에서 검증, gcd 함수 안에서는 양수만 가정

예시:
```
Enter a b: 24 36
gcd = 12
```

---

## 문제 2 (중간) — 함수 오버로딩과 const 참조

다음 세 가지 `print_info` 함수를 오버로딩으로 작성하세요.

```cpp
void print_info(int v);                            // "int: <v>" 출력
void print_info(double v);                         // "double: <v>" 출력
void print_info(const std::string& v);             // "string: <v>" 출력
void print_info(const std::vector<int>& v);        // "vector(N): <v0> <v1> ..." 출력 (N은 size)
```

요구사항:
- 큰 객체는 반드시 `const T&`로 전달
- `[[nodiscard]]`가 아닌 void 함수
- main에서 각 오버로드를 한 번씩 호출

예시 출력:
```
int: 42
double: 3.14
string: hello
vector(3): 1 2 3
```

---

## 문제 3 (임베디드 응용) — 센서 데이터 정규화 함수

다음 시그니처의 함수를 작성하세요.

```cpp
// raw: 0~4095 (12-bit ADC 값)
// vref: 기준 전압 (V), 기본값 3.3
// 반환: 변환된 전압 (V), 0.0~vref 범위
double adc_to_voltage(uint16_t raw, double vref = 3.3);

// 여러 ADC 샘플을 한꺼번에 변환하여 out_v에 저장
// 입력: in_raw (참조로 받음), out_v (참조로 받아 채움)
// 반환: 처리한 샘플 수
std::size_t convert_all(const std::vector<uint16_t>& in_raw,
                        std::vector<double>& out_v,
                        double vref = 3.3);
```

요구사항:
- `<cstdint>` include로 `uint16_t` 사용
- `raw`가 4095를 초과하면 `std::cerr`로 경고 후 4095로 클램프
- main에서 5개 raw 값(예: 0, 1024, 2048, 3072, 4095)을 vector에 넣고 변환, 결과 출력

예시 출력 (소수점 셋째 자리):
```
raw=0    -> 0.000 V
raw=1024 -> 0.825 V
raw=2048 -> 1.650 V
raw=3072 -> 2.475 V
raw=4095 -> 3.300 V
processed = 5
```

> 힌트: 12-bit ADC의 변환 공식은 `V = raw * Vref / 4095`. 임베디드에서 이런 변환 함수는 매우 자주 작성합니다. 함수 시그니처를 어떻게 설계할지(`uint16_t` vs `int`, 참조 vs 포인터, 기본 인자 사용 여부)가 핵심입니다.
