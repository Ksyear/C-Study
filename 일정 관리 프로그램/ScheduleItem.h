#pragma once

#include <iostream>
#include <format>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std::chrono;
using namespace std;

class ScheduleItem
{
public:
  ScheduleItem(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now);

  virtual ~ScheduleItem() = default;

  virtual void displayAllSchedules();

  virtual void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now);

  virtual void markAsCompleted();

  int getId();

  string getTitle();

  string getDescription();

  string getStartDate();

  string getEndDate();

  string getStartTime();

  string getEndTime();

  string getPriority();

  string getCreatedAt();

  string getUpdatedAt();

  bool getIsCompleted();

  void setIsCompleted(bool isCompleted);

  void setCreatedAt(const string &createdAt);

  void setUpdatedAt(const string &updatedAt);

  // 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
  // virtual 소멸자를 넣어야 메모리/자원 누수가 안 남 => `getType()` 같은 가상 함수로 다형성을 쓰고 있다면, 가상 소멸자까지 같이 넣는 게 C++ 쪽 관례
  virtual string getReminderMessage();

  virtual string getType() const = 0;

  // serializeCommon 뒤에 결합할 내용
  virtual string serialize() const = 0;

// 굳이 public 써서 외부에 노출할 필요없이 자식 애들과 결합하기만 할 애니까 protected 사용
protected:
  string serializeCommon() const;

private:
  int id = 0;
  string priority, title, description;
  string startDate, endDate, startTime, endTime;
  string createdAt, updatedAt; // 이런시긍로 startDate을
  bool isCompleted, conflict;
};
