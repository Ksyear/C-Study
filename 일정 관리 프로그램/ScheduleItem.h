#pragma once

#include <string>
#include <chrono>
#include <ctime>

using namespace std::chrono;
using namespace std;

// id 수정하지 않고도 되게 코드 수정 필요
class ScheduleItem
{
public:
  ScheduleItem(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now);

  virtual void displayAllSchedules();

  virtual void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now);

  virtual void markAsCompleted();

  int getId();

  void setId(int id);

  string getTitle();

  string getDescription();

  string getStartDate();

  string getEndDate();

  string getStartTime();

  string getEndTime();

  string getPriority();

  std::time_t getCreatedAt();

  std::time_t getUpdatedAt();

  bool getIsCompleted();

  void setIsCompleted(bool isCompleted);

  // 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
  virtual string getReminderMessage();

private:
  int id = 0;
  string priority, title, description;
  string startDate, endDate, startTime, endTime;
  std::time_t createdAt, updatedAt; // 이런시긍로 startDate을 
  bool isCompleted, conflict;
};