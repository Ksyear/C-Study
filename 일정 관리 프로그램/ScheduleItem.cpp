#include "ScheduleItem.h"
#include <iostream>

using namespace std::chrono;
using namespace std;

// 해야할 일: id 바뀌지 않게 할 것 그리고 배열은 계속 옮길것 그리고 id 관련으로 검색하던거는 배열리스트로 변경할것

// 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
ScheduleItem::ScheduleItem(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
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

void ScheduleItem::displayAllSchedules()
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

void ScheduleItem::setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
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

void ScheduleItem::markAsCompleted()
{
  setIsCompleted(true);
}

int ScheduleItem::getId()
{
  return id;
}

void ScheduleItem::setId(int id)
{
  this->id = id;
}

string ScheduleItem::getTitle()
{
  return title;
}

string ScheduleItem::getDescription()
{
  return description;
}

string ScheduleItem::getStartDate()
{
  return startDate;
}

string ScheduleItem::getEndDate()
{
  return endDate;
}

string ScheduleItem::getStartTime()
{
  return startTime;
}

string ScheduleItem::getEndTime()
{
  return endTime;
}

string ScheduleItem::getPriority()
{
  return priority;
}

std::time_t ScheduleItem::getCreatedAt()
{
  return createdAt;
}

std::time_t ScheduleItem::getUpdatedAt()
{
  return updatedAt;
}

bool ScheduleItem::getIsCompleted()
{
  return isCompleted;
}

void ScheduleItem::setIsCompleted(bool isCompleted)
{
  this->isCompleted = isCompleted;
}

// 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
string ScheduleItem::getReminderMessage()
{
  return "";
}
