# Day 04 — 연습 문제 (배열)

---

## 문제 1 (쉬움) — std::array 최대/최소

`std::array<int, 10>`을 다음 값으로 초기화하고, 최대값과 최소값을 찾아 출력하세요.

```cpp
std::array<int, 10> data = {45, -3, 27, 18, 0, 99, -27, 56, 12, 8};
```

요구사항:
- range-based for 사용
- `std::numeric_limits<int>::min()` / `max()`를 활용하거나, 또는 첫 원소로 초기화 후 비교
- 결과 형식: `Max = 99, Min = -27`

---

## 문제 2 (중간) — 슬라이딩 윈도우 평균

`std::vector<double>` 입력을 받아 **이동 평균(moving average)**을 출력하는 프로그램을 작성하세요.

- 사용자로부터 데이터 개수 N과 윈도우 크기 W를 입력
- 그 다음 N개의 `double` 값을 입력
- 각 위치 i (i >= W-1)에서 `[i-W+1, i]` 범위 평균을 계산
- 결과를 한 줄에 출력 (소수점 둘째 자리)

예시 (`N=6, W=3, data=1 2 3 4 5 6`):
```
Moving averages (W=3): 2.00 3.00 4.00 5.00
```

요구사항:
- `std::vector<double>` 사용
- `<iomanip>`로 소수점 자리수 설정
- W > N 또는 W <= 0이면 에러 처리

---

## 문제 3 (임베디드 응용) — 링 버퍼(Ring Buffer) 구현

크기 8인 `std::array<int, 8>`을 기반으로 **링 버퍼**를 구현하세요.

요구사항:
- main 함수 안에 다음 변수를 선언:
  ```cpp
  std::array<int, 8> buf{};
  std::size_t head = 0;  // 다음 push 위치
  std::size_t tail = 0;  // 다음 pop 위치
  std::size_t count = 0; // 현재 저장된 요소 수
  ```
- 사용자로부터 명령을 반복 입력받음:
  - `P <값>` : 푸시 (count == 8이면 `[FULL]` 에러)
  - `G`     : 팝 (count == 0이면 `[EMPTY]` 에러, 아니면 값 출력)
  - `S`     : 현재 buf 내용을 head/tail 표시와 함께 출력
  - `Q`     : 종료
- 입력 순환 처리는 `while` 또는 `do-while` 사용

예시 세션:
```
Cmd: P 10
Cmd: P 20
Cmd: P 30
Cmd: S
buf: [10 20 30 0 0 0 0 0] head=3 tail=0 count=3
Cmd: G
Pop: 10
Cmd: S
buf: [10 20 30 0 0 0 0 0] head=3 tail=1 count=2
Cmd: Q
```

(`G` 후에도 buf 배열의 값은 그대로 두어도 됩니다. 실제 링 버퍼는 데이터를 0으로 지우지 않습니다.)

> 힌트: head/tail은 모두 `% 8` (또는 `& 0x07` since 8은 2의 거듭제곱)로 wrap-around. 임베디드에서는 마스크 연산이 modulo보다 빠릅니다.
