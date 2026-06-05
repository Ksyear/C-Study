#pragma once

#include <iostream>
#include <string>
#include <typeinfo>
// 현재 시간 가져오기
#include <chrono>
#include <ctime>

using namespace std::chrono;
using namespace std;

class ScheduleItem
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleItem(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
  {
    this->id = ++idCounter;
    this->title = title;
    this->description = description;
    this->startDate = startDate;
    this->endDate = endDate;
    this->startTime = startTime;
    this->endTime = endTime;
    this->priority = priority;
    this->isCompleted = false;
    this->createdAt = system_clock::to_time_t(now); // 자동
  }

  virtual void displayAllSchedules()
  {
    cout << "ID: " << id
         << ", Title: " << title
         << ", Description: " << description
         << ", Start Date: " << startDate
         << ", End Date: " << endDate
         << ", Start Time: " << startTime
         << ", End Time: " << endTime
         << ", Priority: " << priority
         << ", Completed: " << (isCompleted ? "Yes" : "No")
         << ", Created At: " << createdAt // std::ctime(&createdAt)
         << ", Updated At: " << updatedAt; //std::ctime(&updatedAt);
  }

  virtual void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
  {
    this->title = title;
    this->description = description;
    this->startDate = startDate;
    this->endDate = endDate;
    this->startTime = startTime;
    this->endTime = endTime;
    this->priority = priority;
    this->updatedAt = system_clock::to_time_t(now); // 자동
  }

  virtual void markAsCompleted()
  {
    setIsCompleted(true);
  }

  int getId()
  {
    return id;
  }

  void setId(int id)
  {
    this->id = id;
  }

  string getTitle()
  {
    return title;
  }

  string getDescription()
  {
    return description;
  }

  string getStartDate()
  {
    return startDate;
  }

  string getEndDate()
  {
    return endDate;
  }

  string getStartTime()
  {
    return startTime;
  }

  string getEndTime()
  {
    return endTime;
  }

  string getPriority()
  {
    return priority;
  }

  std::time_t getCreatedAt()
  {
    return createdAt;
  }

  std::time_t getUpdatedAt()
  {
    return updatedAt;
  }

  bool getIsCompleted()
  {
    return isCompleted;
  }

  void setIsCompleted(bool isCompleted)
  {
    this->isCompleted = isCompleted;
  }

  // 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
  virtual string getReminderMessage()
  {
    return "";
  }

private:
  int id = 0;
  string priority, title, description;
  // YYYY-MM-DD, HH:MM
  string startDate, endDate, startTime, endTime;
  std::time_t createdAt, updatedAt;
  bool isCompleted, conflict;
};