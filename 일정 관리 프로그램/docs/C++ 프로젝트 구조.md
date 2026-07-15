## 조사 범위

- 언어·빌드 기준 => **C++17**, CMake의 타깃 중심 빌드 구성
- 구성 대상 => 실행 파일, 재사용 라이브러리, 공개 헤더, 내부 구현, 테스트, 문서, 빌드 산출물
- 기본 원칙 => 선언과 구현 분리, 소스 트리와 빌드 트리 분리, 디렉터리별 `CMakeLists.txt` 연결, 타깃 단위 의존성 표현
- 아래 구조 => CMake가 강제하는 단일 규격이 아닌 확장 가능한 구성 예시

## 전체 디렉터리 구조

```text
my_project/
├── CMakeLists.txt                 # 최상위 빌드 진입점
├── CMakePresets.json              # 프로젝트 공용 구성·빌드·테스트 프리셋
├── CMakeUserPresets.json          # 개발자 로컬 프리셋, 버전 관리 제외
├── README.md                      # 프로젝트 설명과 사용 방법
├── LICENSE                        # 배포 조건
├── .gitignore                     # build 등 생성 파일 제외
├── cmake/                         # 프로젝트 전용 CMake 모듈·함수
├── include/
│   └── my_project/
│       └── calculator.h           # 외부에 공개할 인터페이스
├── src/
│   ├── CMakeLists.txt
│   ├── calculator.cpp             # 라이브러리 구현
│   └── detail/                    # 외부에 공개하지 않는 내부 헤더
├── app/
│   ├── CMakeLists.txt
│   └── main.cpp                   # 실행 파일 진입점
├── tests/
│   ├── CMakeLists.txt
│   └── calculator_test.cpp        # 테스트 코드
├── examples/                      # 라이브러리 사용 예제
├── docs/                          # 설계·API 문서
└── build/                         # 생성 빌드 시스템과 산출물, 버전 관리 제외
```

## 디렉터리와 파일 역할

| 경로 | 역할 | 관리 기준 |
| --- | --- | --- |
| `CMakeLists.txt` | 프로젝트 선언, 하위 디렉터리 연결, 테스트 활성화 | 소스 트리 최상위에 배치 |
| `include/my_project/` | 소비자가 포함하는 공개 헤더 | 설치·배포 대상 |
| `src/` | 라이브러리 구현과 내부 전용 헤더 | 라이브러리 타깃 구성 |
| `app/` | `main()`과 실행 파일 전용 코드 | 라이브러리 타깃을 링크 |
| `tests/` | 테스트 실행 파일과 CTest 등록 | 프로덕션 라이브러리 타깃을 링크 |
| `examples/` | 공개 인터페이스 사용 예시 | 필요 시 별도 타깃 구성 |
| `cmake/` | 반복 사용하는 CMake 함수·모듈 | 최상위 또는 하위 `CMakeLists.txt`에서 포함 |
| `docs/` | 빌드에 직접 참여하지 않는 설계·API 문서 | 소스와 분리 |
| `build/` | 오브젝트 파일, 라이브러리, 실행 파일, 생성 빌드 시스템 | 삭제 후 재생성 가능, 버전 관리 제외 |
| `CMakePresets.json` | 팀·CI가 공유하는 CMake 설정 | 버전 관리 포함 가능 |
| `CMakeUserPresets.json` | 개인 환경 전용 CMake 설정 | 버전 관리 제외 |

## 공개 인터페이스와 내부 구현 분리

공개 헤더(Public Header): **여러 소스 파일이나 외부 소비자가 사용하는 선언을 제공하는 인터페이스 파일**

구현 파일(Implementation File): **공개 헤더의 선언에 대응하는 정의를 두는 `.cpp` 파일**

- C++ Core Guidelines의 소스 파일 기준 => 선언은 인터페이스, 정의는 구현으로 구분
- 여러 소스 파일에서 사용하는 선언 => 헤더 파일에 배치
- `.cpp` 파일 => 자신이 구현하는 인터페이스 헤더를 직접 포함, 선언과 정의의 불일치를 컴파일 단계에서 확인
- 공개 헤더 => 필요한 의존 헤더를 직접 포함하는 자체 완결형(self-contained) 구성
- 외부에 공개하지 않는 선언 => `src/` 또는 `src/detail/`의 내부 헤더에 배치
- 헤더 파일 이름 확장자 => 프로젝트의 기존 규칙이 없다면 인터페이스 파일에 `.h`, 코드 파일에 `.cpp` 사용
- 공개 헤더 설치 경로 => `include/my_project/`처럼 프로젝트별 하위 디렉터리 사용

```cpp
// include/my_project/calculator.h
#ifndef MY_PROJECT_CALCULATOR_H
#define MY_PROJECT_CALCULATOR_H

namespace my_project {
int add(int lhs, int rhs);
}

#endif
```

```cpp
// src/calculator.cpp
#include "my_project/calculator.h"

namespace my_project {
int add(int lhs, int rhs) {
    return lhs + rhs;
}
}
```

## CMake 타깃과 하위 디렉터리 구성

타깃(Target): **CMake가 빌드하는 실행 파일·라이브러리 또는 소비자에게 사용 요구사항을 전달하는 단위**

- 최상위 `CMakeLists.txt` => 프로젝트 전체 진입점, `add_subdirectory()`로 하위 `CMakeLists.txt` 처리
- 하위 `CMakeLists.txt`의 상대 경로 => 해당 하위 디렉터리를 기준으로 해석
- 라이브러리 타깃 => 구현 소스, 공개 include 경로, 필요한 C++ 표준 기능 보유
- 실행 파일·테스트 타깃 => 소스 파일 직접 공유 대신 라이브러리 타깃 링크
- 별칭 타깃(Alias Target) => `my_project::core`처럼 네임스페이스가 포함된 일관된 링크 이름 제공

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.23)
project(my_project VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(app)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
```

```cmake
# src/CMakeLists.txt
add_library(my_project_core calculator.cpp)
add_library(my_project::core ALIAS my_project_core)

target_compile_features(my_project_core PUBLIC cxx_std_17)
target_include_directories(my_project_core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
```

```cmake
# app/CMakeLists.txt
add_executable(my_project_app main.cpp)
target_link_libraries(my_project_app PRIVATE my_project::core)
```

## 의존성 전파 범위

CMake 사용 요구사항(Usage Requirements): **타깃을 올바르게 컴파일·링크하기 위해 소비자에게 전달되는 include 경로, 컴파일 기능, 정의, 옵션, 링크 의존성**

| 범위 | 현재 타깃 빌드에 사용 | 소비자에게 전파 | 적용 기준 |
| --- | --- | --- | --- |
| `PRIVATE` | O | X | 구현에서만 필요한 항목 |
| `PUBLIC` | O | O | 구현과 공개 인터페이스 모두에 필요한 항목 |
| `INTERFACE` | X | O | 현재 타깃 구현에는 필요 없고 소비자에게만 필요한 항목 |

- 공개 헤더에서 사용하는 라이브러리 의존성 => `PUBLIC`
- `.cpp` 구현에서만 사용하는 라이브러리 의존성 => `PRIVATE`
- 헤더 전용 타깃의 소비자 요구사항 => `INTERFACE`
- `BUILD_INTERFACE` => 같은 빌드 트리에서 타깃을 사용할 때 적용
- `INSTALL_INTERFACE` => 설치·내보내기 된 타깃을 사용할 때 적용
- 설치 인터페이스의 의존 라이브러리 include 경로 => 빌드한 시스템의 절대 경로를 직접 기록하지 않고 의존 타깃이 자체 위치를 전달하도록 구성

## 소스 트리와 빌드 트리 분리

소스 트리(Source Tree): **소스 코드와 `CMakeLists.txt`가 위치하는 디렉터리**

빌드 트리(Build/Binary Tree): **CMake가 생성한 빌드 시스템, 오브젝트 파일, 라이브러리, 실행 파일이 위치하는 디렉터리**

- Out-of-source Build => 소스 디렉터리와 빌드 디렉터리를 분리하는 방식
- 효과 => 소스 트리 오염 방지, 빌드 디렉터리 삭제만으로 산출물 정리, 한 소스 트리에서 여러 도구 체인·구성의 빌드 트리 유지
- `-S` => 최상위 `CMakeLists.txt`가 있는 소스 디렉터리 지정
- `-B` => 생성 빌드 시스템과 산출물을 둘 빌드 디렉터리 지정

```sh
cmake -S . -B build
cmake --build build
```

- 생성된 빌드 시스템 => 특정 머신과 구성에 종속, 직접 수정하지 않고 CMake 입력 파일에서 변경
- 다른 generator를 사용할 때 => 동일한 빌드 디렉터리 재사용 불가

## 테스트 구조

CTest: **CMake 프로젝트의 테스트 명령을 검색·실행하고 종료 코드로 성공과 실패를 보고하는 도구**

- 최상위 `include(CTest)` => 기본값이 `ON`인 `BUILD_TESTING` 옵션 생성, 활성 상태에서 `enable_testing()` 호출
- `tests/CMakeLists.txt` => 테스트 실행 파일 생성, 검사할 라이브러리 링크, `add_test()`로 CTest 등록
- `add_test()`의 실행 결과 => 종료 코드 `0`이면 통과, 0이 아니면 실패
- 테스트 코드 위치 => 프로덕션 `src/`와 분리

```cmake
# tests/CMakeLists.txt
add_executable(calculator_test calculator_test.cpp)
target_link_libraries(calculator_test PRIVATE my_project::core)

add_test(NAME calculator_test COMMAND calculator_test)
```

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## CMake 프리셋 구조

CMake Preset: **반복 사용하는 구성·빌드·테스트 설정을 프로젝트 루트의 JSON 파일에 저장하는 방식**

1. `CMakePresets.json`: 프로젝트 공용 설정 => 팀과 CI에서 공유, 버전 관리 가능
2. `CMakeUserPresets.json`: 개발자 개인 설정 => 로컬 환경 전용, 버전 관리 제외

```json
{
  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 23,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "debug",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/debug",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug",
      "configurePreset": "debug"
    }
  ],
  "testPresets": [
    {
      "name": "debug",
      "configurePreset": "debug",
      "output": {
        "outputOnFailure": true
      }
    }
  ]
}
```

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## 설치 결과 구조

설치 트리(Install Tree): **빌드 결과 중 소비자에게 제공할 실행 파일·라이브러리·공개 헤더를 설치 prefix 아래에 배치한 구조**

| 산출물 | `GNUInstallDirs` 변수 | 기본 디렉터리 |
| --- | --- | --- |
| 실행 파일 | `CMAKE_INSTALL_BINDIR` | `bin` |
| 공유·정적 라이브러리 | `CMAKE_INSTALL_LIBDIR` | `lib` |
| 공개 헤더 | `CMAKE_INSTALL_INCLUDEDIR` | `include` |

- `install()` => 타깃, 파일, 디렉터리, export 등의 설치 규칙 정의
- 설치 목적지 => 절대 경로보다 설치 prefix 기준 상대 경로 사용, `cmake --install`의 `--prefix`와 재배치 가능한 패키지 지원
- 공개 헤더 => `include/my_project/` 구조를 유지해 설치
- 빌드 트리와 설치 트리 => 개발 중간 산출물과 최종 소비 파일을 분리

```cmake
# 최상위 CMakeLists.txt에 추가하는 기본 설치 규칙
include(GNUInstallDirs)

install(TARGETS my_project_core
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS my_project_app
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/my_project
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
```

```sh
cmake --install build --prefix ./install
```

```text
install/
├── bin/
│   └── my_project_app
├── lib/
│   └── <my_project_core 라이브러리>
└── include/
    └── my_project/
        └── calculator.h
```

## 구조 확장 기준

1. 실행 파일과 재사용 코드 분리 => `main.cpp`는 `app/`, 재사용 구현은 라이브러리 타깃인 `src/`
2. 인터페이스와 구현 분리 => 공개 헤더는 `include/my_project/`, 내부 구현과 비공개 헤더는 `src/`
3. 컴포넌트 증가 => 컴포넌트별 하위 디렉터리와 `CMakeLists.txt` 추가, 최상위에서 `add_subdirectory()` 연결
4. 의존성 추가 => 전역 include·컴파일 옵션 대신 타깃별 사용 요구사항과 전파 범위 지정
5. 테스트 추가 => 프로덕션 소스를 복제하지 않고 검사 대상 라이브러리 타깃 링크
6. 빌드 구성 증가 => 소스 트리 복제 대신 별도 빌드 디렉터리 또는 CMake Preset 추가
7. 배포 대상 구분 => 공개 헤더·라이브러리·실행 파일만 설치 트리에 포함

## 참고 문서

- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [CMake User Interaction Guide](https://cmake.org/cmake/help/latest/guide/user-interaction/index.html)
- [CMake Buildsystem Manual](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html)
- [CMake `add_subdirectory()`](https://cmake.org/cmake/help/latest/command/add_subdirectory.html)
- [CMake `target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html)
- [CMake Testing and CTest Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/Testing%20and%20CTest.html)
- [CMake Presets Manual](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)
- [CMake `install()`](https://cmake.org/cmake/help/latest/command/install.html)
- [CMake `GNUInstallDirs`](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html)
- [C++ Core Guidelines — Source files](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-source)
