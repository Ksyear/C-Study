# Day 04 — 배열

> 학습 표준: C++17 / 컴파일러: clang++ (macOS)

---

## [핵심 요약]

C++에서 "배열"은 세 가지로 구분해야 합니다.

| 종류 | 헤더 | 크기 | 메모리 위치 | 권장 용도 |
|------|------|------|-------------|-----------|
| C-style 배열 `T arr[N]` | (없음) | 컴파일 타임 고정 | 스택/정적 | 펌웨어 저수준, 인터페이스 호환 |
| `std::array<T, N>` | `<array>` | 컴파일 타임 고정 | 스택/정적 | **임베디드에서 가장 추천** |
| `std::vector<T>` | `<vector>` | 런타임 가변 | **힙** | 호스트 측 PC 코드, 동적 크기 필요 시 |

**임베디드에서는 우선순위가 `std::array > C-style 배열 ≫ std::vector`**입니다. 동적 할당(`std::vector`)은 메모리 단편화와 결정성 문제를 일으킵니다.

---

## [개념 설명]

### 1. C-style 배열

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int sz = sizeof(arr) / sizeof(arr[0]);  // = 5
```

특징:
- **크기 정보를 타입에 포함**(`int[5]`)하지만, 함수에 전달되는 순간 **포인터로 decay**되어 크기 정보가 사라집니다.
- 함수 인자 `void f(int arr[])`는 실제로는 `void f(int* arr)`와 동일. `sizeof(arr)`는 포인터 크기(8 bytes on 64-bit)를 반환합니다.

### 2. `std::array<T, N>`

```cpp
#include <array>
std::array<int, 5> arr = {10, 20, 30, 40, 50};
arr.size();      // 5
arr[2];          // 30 (bounds check 없음)
arr.at(2);       // 30 (범위 초과 시 std::out_of_range 예외)
arr.front();     // 10
arr.back();      // 50
```

특징:
- C-style 배열을 **얇게 감싼 래퍼**. 런타임 오버헤드 거의 없음.
- 크기를 잊지 않음 — 함수 인자로 넘겨도 `size()` 호출 가능 (참조로 넘길 경우).
- `begin()`/`end()` 제공 → range-based for, STL 알고리즘 사용 가능.

### 3. `std::vector<T>`

```cpp
#include <vector>
std::vector<int> v = {10, 20, 30};
v.push_back(40);     // 끝에 추가
v.size();            // 4
v.capacity();        // >= size, 미리 할당된 용량
v.reserve(100);      // capacity를 100으로 확보 (재할당 회피)
```

특징:
- 힙에 메모리 할당, 자동 확장 (보통 2배씩).
- 확장 시 모든 요소를 새 메모리로 복사/이동 — **iterator 무효화**.
- 호스트 측 PC C++의 기본 배열로 가장 많이 쓰임.

### 4. 인덱싱과 안전성

```cpp
arr[i]    // 빠름, bounds check 없음 → UB 위험
arr.at(i) // 검사 후 예외 발생 (std::array, std::vector)
```

임베디드에서는 예외를 비활성화하는 경우가 많아 `at()`을 쓸 수 없습니다. 따라서 인덱스 검증은 **호출 측에서** 명시적으로 수행해야 합니다.

### 5. 메모리 레이아웃

세 종류 모두 원소가 **연속된 메모리**에 저장됩니다. 그래서 C API에 포인터 + 크기로 넘길 수 있습니다.

```cpp
std::array<float, 64> samples{};
hardware_dma_start(samples.data(), samples.size());  // C 함수에 raw pointer 전달
```

`data()` 멤버 함수가 첫 원소의 주소를 반환합니다.

---

## [예제 코드]

```cpp
// main.cpp
#include <iostream>
#include <array>
#include <vector>
#include <numeric>   // std::accumulate

// std::array를 const 참조로 받음 (복사 회피, 크기 정보 유지)
double average(const std::array<int, 5>& arr) {
    long long sum = 0;
    for (int v : arr) {
        sum += v;
    }
    return static_cast<double>(sum) / arr.size();
}

int main() {
    // 1. C-style 배열
    int c_arr[3] = {1, 2, 3};
    std::cout << "C-array size in bytes: " << sizeof(c_arr) << '\n';
    std::cout << "C-array elements: ";
    for (int i = 0; i < 3; ++i) {
        std::cout << c_arr[i] << ' ';
    }
    std::cout << '\n';

    // 2. std::array
    std::array<int, 5> samples = {12, 7, 23, 19, 5};
    std::cout << "std::array size: " << samples.size() << '\n';
    std::cout << "std::array elements: ";
    for (const auto& v : samples) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
    std::cout << "Average: " << average(samples) << '\n';

    // STL 알고리즘 사용 예
    int total = std::accumulate(samples.begin(), samples.end(), 0);
    std::cout << "Sum (accumulate): " << total << '\n';

    // 3. std::vector
    std::vector<int> v;
    v.reserve(10);  // 재할당 회피
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i * i);
    }
    std::cout << "vector elements: ";
    for (const auto& x : v) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
    std::cout << "vector size=" << v.size()
              << ", capacity=" << v.capacity() << '\n';

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
C-array size in bytes: 12
C-array elements: 1 2 3
std::array size: 5
std::array elements: 12 7 23 19 5
Average: 13.2
Sum (accumulate): 66
vector elements: 1 4 9 16 25
vector size=5, capacity=10
```

(C-array의 크기 12는 `sizeof(int) * 3` = `4 * 3` 기준이며, 플랫폼에 따라 다를 수 있습니다.)

---

## [동작 원리]

1. **C-style 배열의 decay**: `int arr[5]`를 `void f(int* p)`에 넘기면 컴파일러는 `&arr[0]`을 자동 전달합니다. 함수 내부에서는 단순 포인터이므로 `sizeof`는 포인터 크기만 반환합니다. 이것이 C-style 배열의 가장 큰 함정입니다.
2. **`std::array`는 zero-overhead 추상화**입니다. `clang++ -O2 -S`로 어셈블리를 확인하면 C-style 배열과 거의 동일한 코드가 생성됩니다. **크기를 잃지 않으면서 안전성을 얻는** 거래입니다.
3. **`std::vector`의 capacity vs size**:
   - `size()` : 현재 저장된 원소 개수
   - `capacity()` : 현재 할당된 메모리에 담을 수 있는 최대 개수
   - `push_back` 시 size > capacity가 되면 **재할당** 발생: 큰 메모리 새로 할당 → 원소 이동/복사 → 기존 메모리 해제.
   - 따라서 미리 `reserve`하면 재할당과 iterator 무효화를 막을 수 있습니다.
4. **`std::accumulate`는 STL 알고리즘**입니다. 컨테이너 종류에 무관하게 동작하며, 컴파일러가 인라인/벡터화 최적화를 수행할 수 있습니다.

---

## [임베디드 관점]

- **동적 할당 회피**: `std::vector`는 내부적으로 `new[]`/`delete[]`(또는 `malloc`/`free`)를 사용합니다. MCU에서 힙 단편화가 심해지면 결국 할당 실패가 발생할 수 있어 **결정성을 해칩니다**. `std::array` 또는 정적 크기의 C-style 배열을 사용하세요.
- **링 버퍼(Ring Buffer)**: 임베디드의 단골 자료구조. `std::array`로 고정 크기 배열 + `head`/`tail` 인덱스 두 개로 구현. UART/SPI 수신 버퍼, 센서 샘플 큐 등에 사용.
- **DMA와의 인터페이스**: DMA는 raw pointer + 크기를 받습니다. `std::array<uint16_t, 256> adc_buf;` 후 `HAL_ADC_Start_DMA(&hadc, adc_buf.data(), adc_buf.size());` 형태로 사용 가능.
- **`uint8_t`/`uint16_t`/`uint32_t`**: 임베디드에서는 정확한 비트폭이 중요하므로 `<cstdint>`의 고정폭 정수 타입을 사용하세요. `int`는 플랫폼에 따라 16/32/64 bit이 됩니다.
  ```cpp
  #include <cstdint>
  std::array<uint16_t, 256> adc_samples{};
  ```
- **bounds check 비용**: `at()`은 매 호출마다 검사. 핫 루프 안에서는 `[]`를 쓰되 **루프 전에 한 번** 크기 검증을 수행하세요.

---

## [주의할 점]

1. **`int arr[N]`에서 N이 변수인 C99 VLA(가변 길이 배열)는 표준 C++가 아닙니다.** GCC/Clang의 확장이며, 표준 코드를 작성하려면 `std::array`(컴파일 타임 N) 또는 `std::vector`(런타임 N)를 사용하세요.
2. **배열 범위 초과 접근**: `arr[5]`인데 `arr[5]` 또는 `arr[-1]`은 UB입니다. 컴파일러가 잡지 못합니다.
3. **`sizeof(arr) / sizeof(arr[0])`은 함수 안에서 동작하지 않습니다**(decay 때문). C++17에서는 `std::size(arr)`(`<iterator>`)을 사용하면 둘 다 안전합니다.
4. **`std::vector<bool>`은 특수화**되어 일반 `bool`이 아닌 1비트 패킹 구조입니다. iterator가 보통의 `bool&`를 반환하지 않아 함정이 많습니다. 진짜 bool 배열이 필요하면 `std::vector<char>` 또는 `std::bitset`을 고려하세요.
5. **초기화 누락**: `int arr[5];` (default-initialize, 값 미정) vs `int arr[5]{};` (zero-initialize, 모두 0). 두 번째 형태를 습관화하세요.
6. **`push_back` 중 iterator 무효화**: vector의 capacity를 초과하는 push_back은 모든 기존 iterator/reference/pointer를 무효화합니다. 순회 중 추가하지 마세요.

---

## [연습 문제]

- 문제: [`Day04_problems.md`](./Day04_problems.md)
- 정답: [`Day04_solutions.md`](./Day04_solutions.md)
