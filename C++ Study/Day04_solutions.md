# Day 04 — 연습 문제 정답 (배열)

---

## 문제 1 정답

```cpp
#include <iostream>
#include <array>
#include <limits>

int main() {
    std::array<int, 10> data = {45, -3, 27, 18, 0, 99, -27, 56, 12, 8};

    int max_v = std::numeric_limits<int>::min();
    int min_v = std::numeric_limits<int>::max();
    for (const auto& v : data) {
        if (v > max_v) max_v = v;
        if (v < min_v) min_v = v;
    }
    std::cout << "Max = " << max_v << ", Min = " << min_v << '\n';
    return 0;
}
```

**해설**
- `<limits>`의 `std::numeric_limits<T>::min()`/`max()`는 타입별 최소/최대값 상수입니다. 매크로 `INT_MIN`/`INT_MAX` (`<climits>`)도 사용 가능하지만 C++에서는 `numeric_limits`가 표준입니다.
- 더 간결하게는 STL 알고리즘: `auto [mn, mx] = std::minmax_element(data.begin(), data.end()); std::cout << *mx << ' ' << *mn;` (`<algorithm>`, C++17 구조적 바인딩).
- 임베디드 응용: 센서 보정 시 측정값의 범위(min/max)를 추출하는 패턴입니다.

---

## 문제 2 정답

```cpp
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    int N = 0, W = 0;
    std::cout << "Enter N and W: ";
    std::cin >> N >> W;

    if (W <= 0 || N <= 0 || W > N) {
        std::cerr << "[ERROR] invalid N or W\n";
        return 1;
    }

    std::vector<double> data;
    data.reserve(N);
    std::cout << "Enter " << N << " doubles:\n";
    for (int i = 0; i < N; ++i) {
        double x = 0.0;
        std::cin >> x;
        data.push_back(x);
    }

    std::cout << "Moving averages (W=" << W << "):";
    std::cout << std::fixed << std::setprecision(2);

    // 누적합 캐싱으로 O(N) 계산
    double window_sum = 0.0;
    for (int i = 0; i < W; ++i) {
        window_sum += data[i];
    }
    std::cout << ' ' << (window_sum / W);

    for (int i = W; i < N; ++i) {
        window_sum += data[i] - data[i - W];
        std::cout << ' ' << (window_sum / W);
    }
    std::cout << '\n';
    return 0;
}
```

**해설**
- 순진하게 매번 W개를 더하면 O(N·W)이지만, **누적합 캐싱(sliding window sum)** 기법으로 O(N)에 처리 가능합니다. 임베디드의 디지털 필터(이동 평균 필터, MA filter)에서 동일한 기법이 사용됩니다.
- `data.reserve(N)`로 vector 재할당을 막았습니다. N개를 받기로 알고 있으므로 미리 확보하는 것이 효율적입니다.
- `std::fixed`/`std::setprecision(2)`는 한 번 설정하면 이후 모든 부동소수점 출력에 적용됩니다.
- 부동소수점 누적은 **오차 누적** 문제가 있습니다(특히 long-running). 정밀도가 중요한 임베디드 신호 처리는 고정소수점(fixed-point) 또는 Kahan summation을 고려합니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <array>
#include <cstddef>

int main() {
    constexpr std::size_t kCap = 8;
    std::array<int, kCap> buf{};
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;

    while (true) {
        std::cout << "Cmd: ";
        char op = '\0';
        std::cin >> op;
        if (!std::cin) break;

        if (op == 'P') {
            int val = 0;
            std::cin >> val;
            if (count == kCap) {
                std::cerr << "[FULL]\n";
                continue;
            }
            buf[head] = val;
            head = (head + 1) & (kCap - 1);  // wrap by mask
            ++count;
        } else if (op == 'G') {
            if (count == 0) {
                std::cerr << "[EMPTY]\n";
                continue;
            }
            int val = buf[tail];
            tail = (tail + 1) & (kCap - 1);
            --count;
            std::cout << "Pop: " << val << '\n';
        } else if (op == 'S') {
            std::cout << "buf: [";
            for (std::size_t i = 0; i < kCap; ++i) {
                std::cout << buf[i];
                if (i + 1 < kCap) std::cout << ' ';
            }
            std::cout << "] head=" << head
                      << " tail=" << tail
                      << " count=" << count << '\n';
        } else if (op == 'Q') {
            break;
        } else {
            std::cerr << "[ERROR] unknown command: " << op << '\n';
        }
    }
    return 0;
}
```

**해설 / 임베디드 관점**
- `head = (head + 1) & (kCap - 1)` 는 `% kCap`과 동일한 결과지만 **CPU 명령어 1개**(AND)로 끝납니다. modulo는 보통 DIV 명령어를 사용해 훨씬 비쌉니다. 이 트릭은 **kCap이 2의 거듭제곱일 때만** 유효합니다.
- `count`를 별도로 유지한 이유: head == tail이 "비어있다"와 "꽉 찼다" 모두를 의미할 수 있어 구분이 어렵기 때문입니다. 대안으로 capacity를 N+1로 하고 한 칸을 비워두는 방식이 있지만 메모리 효율이 낮습니다.
- 실제 임베디드 UART 수신 링 버퍼는 ISR이 producer, main loop이 consumer 형태로 자주 쓰입니다. 이때 `head`/`tail`은 **공유 변수**이므로 `std::atomic<std::size_t>` 또는 race condition 대비책이 필요합니다(이후 학습 주제).
- `std::cin >> op`가 실패하면 EOF나 잘못된 입력. `if (!std::cin) break;`로 빠져나가야 무한 루프 방지.
- `constexpr std::size_t kCap = 8;` 이라는 명명 컨벤션: 임베디드 코드에서 매크로 대신 `constexpr`을 사용하는 것이 안전합니다.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| C-style 배열을 함수에 전달 후 `sizeof` 사용 | 포인터 크기만 반환 (decay) |
| `std::vector` 순회 중 `push_back` | iterator 무효화 → UB |
| `arr[i]`에서 i 범위 미검증 | UB |
| `int arr[N]` with N as variable | C99 VLA, 비표준 C++ |
| `std::vector<bool>` 사용 | 비트 패킹된 특수화, 함정 |
| `int` 인덱스로 `size_t` 비교 | 부호 비교 경고 |
| `buf[head++]` (head 검증 누락) | 버퍼 오버플로우 |
