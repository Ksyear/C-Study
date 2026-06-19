#pragma once

#include <iostream>
#include <format>
#include <string>
#include <chrono>
#include <ctime>

using namespace std::chrono;
using namespace std;

class ScheduleItem
{
public:
  ScheduleItem(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now);

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

  // 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
  virtual string getReminderMessage();

private:
  int id = 0;
  string priority, title, description;
  string startDate, endDate, startTime, endTime;
  string createdAt, updatedAt; // 이런시긍로 startDate을 
  bool isCompleted, conflict;
};