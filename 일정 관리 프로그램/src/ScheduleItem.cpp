#include "../include/ScheduleItem.h"

ScheduleItem::ScheduleItem(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
{
  this->userId = userId;
  this->id = id;
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
  cout << "User ID: " << userId
       << ", ID: " << id
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

void ScheduleItem::setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
{
  this->userId = userId;
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

int ScheduleItem::getUserId()
{
  return userId;
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

void ScheduleItem::setCreatedAt(const string &createdAt)
{
  this->createdAt = createdAt;
}

void ScheduleItem::setUpdatedAt(const string &updatedAt)
{
  this->updatedAt = updatedAt;
}

bool ScheduleItem::sameId(int userId)
{
  return this->userId == userId;
}

// 부모에서 자식의 getter 가져오기 위해 삽입(근데 객체지향에서는 권장하지 않음)
string ScheduleItem::getReminderMessage()
{
  return "";
}

string ScheduleItem::serializeCommon() const
{
  std::ostringstream oss;
  oss << getType() << '|'
      << userId << '|'
      << id << '|'
      << title << '|'
      << description << '|'
      << startDate << '|'
      << endDate << '|'
      << startTime << '|'
      << endTime << '|'
      << priority << '|'
      << isCompleted << '|'
      << createdAt << '|'
      << updatedAt;
  return oss.str();
}
