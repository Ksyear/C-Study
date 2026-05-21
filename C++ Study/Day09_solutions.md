# Day 09 — 연습 문제 정답 (다형성)

---

## 문제 1 정답

```cpp
#include <iostream>

class Shape {
public:
    virtual double area() const = 0;
    virtual const char* name() const = 0;
    virtual ~Shape() = default;
};

class Triangle : public Shape {
private:
    double base_, height_;
public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double area() const override { return 0.5 * base_ * height_; }
    const char* name() const override { return "Triangle"; }
};

class Square : public Shape {
private:
    double side_;
public:
    explicit Square(double side) : side_(side) {}
    double area() const override { return side_ * side_; }
    const char* name() const override { return "Square"; }
};

int main() {
    Triangle t(3.0, 4.0);
    Square sq(5.0);
    Shape* shapes[] = { &t, &sq };
    for (Shape* s : shapes) {
        std::cout << s->name() << ": " << s->area() << '\n';
    }
    return 0;
}
```

**해설**
- `Shape`는 두 순수 가상 함수를 가지므로 추상 클래스. 직접 `Shape s;`를 만들 수 없음 → 컴파일 에러.
- `Triangle`/`Square`는 두 함수 모두 override하므로 인스턴스화 가능.
- `Shape* shapes[]`처럼 base 포인터 배열에 derived 객체의 주소를 담아 다형적으로 호출. 가장 기본적인 다형성 패턴입니다.

---

## 문제 2 정답

**진단**:

1. `FileLogger::log`의 매개변수가 `std::string`(값 전달). base의 `log`는 `const std::string&` (참조). 시그니처 불일치로 **오버라이딩이 아니라 새로운 함수**가 됩니다.
2. `FileLogger::log`에 `const`가 빠져 있음. base는 `const` 멤버 함수, derived는 non-const → 역시 다른 함수.

`override` 키워드를 붙이면 컴파일러가 이를 잡아 컴파일 에러를 발생시킵니다.

**수정된 코드**:

```cpp
#include <iostream>
#include <string>

class Logger {
public:
    virtual void log(const std::string& msg) const {
        std::cout << "[LOG] " << msg << '\n';
    }
    virtual ~Logger() = default;
};

class FileLogger : public Logger {
public:
    void log(const std::string& msg) const override {
        std::cout << "[FILE] " << msg << '\n';
    }
};

int main() {
    FileLogger fl;
    Logger* p = &fl;
    p->log("hello");
    return 0;
}
```

**출력**:
```
[FILE] hello
```

**해설**
- 원래 코드는 `p->log("hello")`에서 base 포인터를 통해 호출했으므로 **base의 `log`**가 호출되어 `[LOG] hello`가 출력되었을 것입니다.
- 시그니처를 일치시키고 `override`를 붙이면 진짜 오버라이딩이 되어 `[FILE] hello`가 출력됩니다.
- 교훈: **오버라이딩하려는 모든 함수에 `override`를 붙여 컴파일러의 도움을 받으세요**. C++11 이전 코드에서는 이런 버그가 흔했습니다.

---

## 문제 3 정답

```cpp
#include <iostream>
#include <memory>
#include <vector>

class ISensor {
public:
    virtual bool init() = 0;
    virtual double read() = 0;
    virtual const char* type() const = 0;
    virtual ~ISensor() = default;
};

class FakeTempSensor : public ISensor {
private:
    int call_count_ = 0;
public:
    bool init() override {
        std::cout << "[" << type() << "] init OK\n";
        return true;
    }
    double read() override {
        ++call_count_;
        return 25.0 + call_count_;
    }
    const char* type() const override { return "Temp"; }

    ~FakeTempSensor() override {
        std::cout << "[" << type() << "] dtor\n";
    }
};

class FakePressureSensor : public ISensor {
private:
    int call_count_ = 0;
public:
    bool init() override {
        std::cout << "[" << type() << "] init OK\n";
        return true;
    }
    double read() override {
        ++call_count_;
        return 100.0 - call_count_ * 0.5;
    }
    const char* type() const override { return "Pressure"; }

    ~FakePressureSensor() override {
        std::cout << "[" << type() << "] dtor\n";
    }
};

int main() {
    std::vector<std::unique_ptr<ISensor>> sensors;
    sensors.push_back(std::make_unique<FakeTempSensor>());
    sensors.push_back(std::make_unique<FakePressureSensor>());

    for (auto& s : sensors) {
        s->init();
    }

    for (auto& s : sensors) {
        for (int i = 1; i <= 3; ++i) {
            std::cout << "[" << s->type() << "] sample " << i
                      << ": " << s->read() << '\n';
        }
    }

    // sensors가 스코프를 벗어나면 unique_ptr이 자동으로 delete
    // 가상 소멸자 덕분에 두 derived의 dtor가 모두 호출됨
    return 0;
}
```

**해설 / 임베디드 관점**
- `ISensor`처럼 인터페이스만 정의하는 클래스를 보통 **인터페이스 클래스**라고 부르며, 이름 앞에 `I`를 붙이는 컨벤션이 흔합니다(C# 영향).
- 가상 소멸자 검증 포인트: 만약 `ISensor`의 소멸자가 `virtual`이 아니었다면, `unique_ptr<ISensor>`이 소멸 시 `ISensor::~ISensor()`만 호출하고 derived의 소멸자는 호출하지 않습니다. 그러면 derived가 가진 자원(파일 핸들, 동적 메모리 등)이 누수됩니다.
- derived의 소멸자에 `override`를 붙일 수 있습니다(C++11~). 이것은 명시적이고 안전합니다.
- 임베디드 응용:
  - 실제 펌웨어에서는 `std::vector` + `std::unique_ptr` 조합이 힙 할당을 동반하므로 회피합니다. 대신:
    - 정적 객체를 미리 만들어두고 `ISensor* sensors[N]`로 포인터만 모음.
    - 또는 `std::array<std::reference_wrapper<ISensor>, N>` 사용.
  - 인터페이스 추상화 자체는 **하드웨어 종류와 무관한 상위 알고리즘**(예: 센서 fusion, 데이터 로깅)을 작성할 때 유용합니다.
  - 단, 모든 호출이 가상 함수가 되어 vtable lookup 비용이 누적될 수 있습니다. 매우 짧은 hot loop에서는 CRTP나 템플릿으로 대체 검토.

---

## 자주 하는 실수 정리

| 실수 | 결과 |
|------|------|
| 가상 소멸자 누락 | `delete base_ptr` 시 derived 소멸자 미호출, 자원 누수 |
| `override` 누락 + 시그니처 차이 | 새 함수로 인식, base 함수가 호출됨 |
| 생성자/소멸자에서 가상 함수 호출 | 부모 버전이 호출되어 의도와 다름 |
| 슬라이싱 (값 복사) | derived 부분 손실 |
| 추상 클래스 인스턴스화 시도 | 컴파일 에러 |
| 모든 가상 함수에 `final`을 안 붙임 | devirtualization 기회 상실 |
| ISR에서 가상 함수 호출 | vtable lookup 비용, 캐시 미스 |
| 다이아몬드 다중 상속 | base 두 번 포함, 모호성 |
