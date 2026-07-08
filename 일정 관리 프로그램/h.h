#include <iostream>
#include <typeinfo>
// STL => vector, memory, algorithm
#include <vector>    // std::vector: 실제 일정 객체만 순서대로 보관하는 동적 배열 컨테이너
#include <memory>    // std::unique_ptr: 일정 객체를 자동으로 delete해 주는 단일 소유 스마트 포인터
#include <algorithm> // <algorithm>: 일정 검색·정렬에 사용하는 표준 알고리즘(find_if, sort 등)을 제공
#include <fstream>   // <fstream>: 일정 정보를 파일로 저장·불러오기 위한 파일 입출력 스트림 제공
#include <sstream>   // <sstream>: 문자열 한 줄을 공백 단위로 파싱할 때 쓰는 문자열 스트림 제공
#include <stdexcept> // <stdexcept>: 잘못된 상태에서 예외를 던질 때 쓰는 표준 예외 클래스(runtime_error 등)
#include <format>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
// 정규 표현식 => 복잡한 문자열 패턴을 검사하는 규칙
#include <regex>