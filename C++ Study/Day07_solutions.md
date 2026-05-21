# Day 07 — 연습 문제 정답 (생성자 · getter/setter)

---

## 문제 1 정답

```cpp
#include <iostream>

class Point {
private:
    double x_ = 0.0;
    double y_ = 0.0;

public:
    Point() : Point(0.0, 0.0) {}                       // 위임 생성자
    Point(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }
    void set_x(double x) { x_ = x; }
    void set_y(double y) { y_ = y; }
};

int main() {
    Point p1;
    Point p2(3.0, 4.0);
    std::cout << "p1=(" << p1.x() << ", " << p1.y() << ")\n";
    std::cout << "p2=(" << p2.x() << ", " << p2.y() << ")\n";

    p1.set_x(5.0);
    p1.set_y(7.0);
    std::cout << "p1 after set=(" << p1.x() << ", " << p1.y() << ")\n";
    return 0;
}
```

**해설**
- 위임 생성자 덕분에 기본값 초기화 로직이 한 곳에 모입니다.
- NSDMI(`= 0.0`)와 위임 생성자가 둘 다 적용된 경우, 생성자가 우선합니다. 따라서 위 코드에서 `Point()`는 `Point(0.0, 0.0)`을 호출해 NSDMI를 덮어씁니다(같은 결과이지만 동작 원리는 다름).

---

## 문제 2 정답

```cpp
#include <iostream>

class Rectangle {
private:
    int x_ = 0;
    int y_ = 0;
    int w_ = 0;
    int h_ = 0;

    static int clamp_non_negative(int v, const char* name) {
        if (v < 0) {
            std::cerr << "[ERROR] " << name << " must be >= 0, got "
                      << v << ", clamped to 0\n";
            return 0;
        }
        return v;
    }

public:
    Rectangle(int x, int y, int w, int h)
        : x_(x),
          y_(y),
          w_(clamp_non_negative(w, "width")),
          h_(clamp_non_negative(h, "height")) {}

    int x()      const { return x_; }
    int y()      const { return y_; }
    int width()  const { return w_; }
    int height() const { return h_; }

    int area()      const { return w_ * h_; }
    int perimeter() const { return 2 * (w_ + h_); }

    bool contains(int px, int py) const {
        return (px >= x_) && (px <= x_ + w_)
            && (py >= y_) && (py <= y_ + h_);
    }

    void resize(int w, int h) {
        w_ = clamp_non_negative(w, "width");
        h_ = clamp_non_negative(h, "height");
    }

    void move_to(int nx, int ny) {
        x_ = nx;
        y_ = ny;
    }
};

int main() {
    Rectangle r(0, 0, 10, 5);
    std::cout << "area=" << r.area()
              << ", perimeter=" << r.perimeter() << '\n';
    std::cout << std::boolalpha;
    std::cout << "contains(5,3)=" << r.contains(5, 3) << '\n';
    std::cout << "contains(11,3)=" << r.contains(11, 3) << '\n';

    r.resize(20, 10);
    r.move_to(5, 5);
    std::cout << "after resize+move: ("
              << r.x() << "," << r.y() << ","
              << r.width() << "," << r.height() << ")\n";
    return 0;
}
```

**해설**
- 검증 로직(clamp)을 static 멤버 함수로 분리해 생성자 초기화 리스트에서 호출했습니다. 초기화 리스트 안에서 임의의 함수를 호출할 수 있다는 점이 중요합니다.
- `w_(clamp_non_negative(w, "width"))`로 받기 때문에, 멤버 변수에 검증된 값만 들어갑니다. 멤버 초기화 후 본문에서 수정하는 방식보다 안전합니다.
- `contains` 경계 포함 여부는 사양에 따라 다릅니다. 본 풀이는 포함(`<=`). 실제 GUI 라이브러리는 `<`(반열린 구간)를 쓰는 경우가 많습니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <array>
#include <cstddef>

class TempLogger {
private:
    static constexpr std::size_t kCap = 8;
    std::array<double, kCap> buf_{};
    std::size_t head_ = 0;       // 다음 쓰기 위치
    std::size_t count_ = 0;      // 현재 저장된 유효 개수 (<=kCap)
    double      threshold_;
    double      last_ = 0.0;
    std::size_t alert_count_ = 0;

public:
    explicit TempLogger(double threshold) : threshold_(threshold) {}

    void feed(double c) {
        buf_[head_] = c;
        head_ = (head_ + 1) % kCap;
        if (count_ < kCap) ++count_;
        last_ = c;
        if (c > threshold_) {
            std::cerr << "[ALERT] over threshold: " << c << '\n';
            ++alert_count_;
        }
    }

    double last() const { return last_; }

    double average() const {
        if (count_ == 0) return 0.0;
        double sum = 0.0;
        for (std::size_t i = 0; i < count_; ++i) {
            sum += buf_[i];
        }
        return sum / static_cast<double>(count_);
    }

    std::size_t alert_count() const { return alert_count_; }

    void reset() {
        head_ = 0;
        count_ = 0;
        alert_count_ = 0;
        // last_와 buf_의 값은 다음 feed로 덮이므로 의미 없음
    }
};

int main() {
    TempLogger logger(85.0);

    const double samples[] = {70, 75, 86, 80, 78, 90, 82, 84, 79, 88};
    for (double s : samples) {
        logger.feed(s);
    }

    std::cout << "last=" << logger.last()
              << ", avg=" << logger.average()
              << ", alerts=" << logger.alert_count() << '\n';
    return 0;
}
```

**해설 / 임베디드 관점**
- `explicit`이 없다면 `void func(TempLogger);`에 `func(80.0)`을 그냥 넘길 수 있어 의도하지 않은 객체 생성이 발생합니다. 단일 인자 생성자에는 거의 항상 `explicit`을 붙이세요.
- 링 버퍼 평균 계산이 정확히 "최근 N개"인지에 주의해야 합니다. 위 풀이는 `buf_[0..count_)`의 평균을 구하는데, 8개 이상 feed된 시점에는 이것이 곧 "가장 최근 8개"의 평균입니다(인덱스 의미상). 만약 7개만 feed됐다면 7개의 평균을 반환합니다.
- 정확히 "마지막 8개"의 평균이 항상 필요하다면 인덱스 계산이 복잡해지지만, 본 문제처럼 ring buffer 내용 자체가 곧 최근 N개가 되므로 단순화 가능합니다.
- 실제 임베디드 디지털 필터에서는 매 feed마다 합을 갱신(`sum += new - old`)하면 평균을 O(1)에 구할 수 있습니다(Day04 문제 2 참고). 학습용으로는 O(N)으로 두었습니다.
- `std::size_t`는 unsigned 타입이라 음수 비교 시 함정이 있습니다. `i - 1` 같은 표현은 `i == 0`일 때 매우 큰 값이 되므로 주의.
- 8개 이상 feed되면 가장 오래된 값이 덮어쓰기 되어 사라집니다. 이것이 ring buffer의 정상 동작입니다. 임베디드 로깅에서 RAM 사용량을 일정하게 유지하기 위해 흔히 쓰는 패턴.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| 초기화 리스트 순서가 멤버 선언 순서와 불일치 | UB 가능성 |
| 생성자 본문에서 멤버 대입 (값 타입이 아닌 경우) | 멤버 두 번 일하기 |
| 단일 인자 생성자에 `explicit` 누락 | 묵시적 변환 |
| 큰 객체 getter가 값 반환 | 복사 비용 |
| 생성자에서 가상 함수 호출 | 의도와 다른 동작 |
| 다른 생성자 정의 후 기본 생성자 미정의 | 기본 생성자 자동 생성 안 됨 |
| `std::move` 후 변수 사용 | 빈 상태 사용, 미정의는 아니지만 잘못된 값 |
