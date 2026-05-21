# Day 07 — 연습 문제 (생성자 · getter/setter)

---

## 문제 1 (쉬움) — `Point` 클래스

2D 점을 표현하는 클래스를 작성하세요.

```cpp
class Point {
public:
    Point();                       // (0, 0)
    Point(double x, double y);     // (x, y)
    double x() const;
    double y() const;
    void   set_x(double x);
    void   set_y(double y);
};
```

요구사항:
- 두 생성자 사이에 위임 생성자 사용 (`Point() : Point(0.0, 0.0) {}`)
- main에서 `Point p1; Point p2(3.0, 4.0);`을 생성하고 좌표 출력
- setter는 별다른 제약 검증 없이 그냥 대입 (단, const 정확성 유지)

예시 출력:
```
p1=(0, 0)
p2=(3, 4)
p1 after set=(5, 7)
```

---

## 문제 2 (중간) — `Rectangle` 클래스

정수 좌표의 사각형을 표현하세요.

```cpp
class Rectangle {
public:
    Rectangle(int x, int y, int w, int h);   // 좌상단(x,y), 너비/높이
    int x() const;
    int y() const;
    int width() const;
    int height() const;
    int area() const;       // w * h
    int perimeter() const;  // 2*(w + h)
    bool contains(int px, int py) const;
    void resize(int w, int h);   // 음수면 거부
    void move_to(int nx, int ny);
};
```

요구사항:
- 생성자에서 w 또는 h가 음수면 `std::cerr` 출력 후 해당 값을 0으로 클램프
- 모든 멤버 변수 private, NSDMI로 안전 초기값 보장
- `contains(px, py)`는 사각형 경계 포함 (x <= px <= x+w, y <= py <= y+h)
- main에서 `Rectangle r(0, 0, 10, 5)`를 만들고 area, perimeter, contains(5, 3), contains(11, 3) 출력 후 resize(20, 10), move_to(5, 5) 호출, 최종 좌표/크기 출력

예시 출력:
```
area=50, perimeter=30
contains(5,3)=true
contains(11,3)=false
after resize+move: (5,5,20,10)
```

---

## 문제 3 (임베디드 응용) — `TempLogger` (이동 평균 + 임계치 알람)

가상의 온도 로거 클래스를 작성하세요.

```cpp
class TempLogger {
public:
    TempLogger(double threshold);  // 임계치, explicit으로 묵시변환 차단
    void feed(double c);           // 새 측정값 추가
    double last() const;
    double average() const;        // 최근 N개의 평균 (N은 내부에서 결정)
    std::size_t alert_count() const;
    void reset();                  // 누적값/카운트 초기화
};
```

요구사항:
- 내부적으로 `std::array<double, 8>`을 ring buffer로 사용 (가장 최근 8개 저장)
- `feed(c)` 호출 시:
  - 링 버퍼에 값 저장
  - c가 threshold를 초과하면 `[ALERT] over threshold: c` 출력 + `alert_count_` 증가
- `average()`는 현재 저장된 유효 샘플의 평균 (8개 미만일 때도 동작)
- `reset()`은 alert_count, 링 버퍼 인덱스/count를 모두 0으로
- 생성자에 `explicit` 적용
- main에서 다음 시나리오:
  - `TempLogger logger(85.0);`
  - 측정값 10개: 70, 75, 86, 80, 78, 90, 82, 84, 79, 88
  - 마지막에 last, average, alert_count 출력

예시 출력 (대략):
```
[ALERT] over threshold: 86
[ALERT] over threshold: 90
[ALERT] over threshold: 88
last=88, avg=83.375, alerts=3
```

> 힌트: 링 버퍼는 8칸이지만 10개를 feed하면 마지막 8개(78, 90, 82, 84, 79, 88과 그 이후)만 평균에 포함됩니다. average 계산은 현재 저장된 유효 샘플 개수로 나누세요.
