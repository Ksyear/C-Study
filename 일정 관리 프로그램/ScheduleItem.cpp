#include "ScheduleItem.h"

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
  zoned_time zt{locate_zone("Asia/Seoul"), now};

  this->createdAt = std::format("{:%Y-%m-%d %H:%M}", zt);
  this->updatedAt = std::format("{:%Y-%m-%d %H:%M}", zt);
  
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
  zoned_time zt{locate_zone("Asia/Seoul"), now};
  this->updatedAt = std::format("{:%Y-%m-%d %H:%M}", zt);
}

void ScheduleItem::markAsCompleted()
{
  setIsCompleted(true);
}

int ScheduleItem::getId()
{
  return id;
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

string ScheduleItem::getCreatedAt()
{
  return createdAt;
}

string ScheduleItem::getUpdatedAt()
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
