#include "../include/GeneralSchedule.h"

GeneralSchedule::GeneralSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
    : ScheduleItem(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now)
{
  this->category = category;
  this->place = place;
  this->memo = memo;
}

void GeneralSchedule::displayAllSchedules()
{
  ScheduleItem::displayAllSchedules();
  cout << ", Category: " << category
       << ", Place: " << place
       << ", Memo: " << memo;
}

void GeneralSchedule::setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
{
  ScheduleItem::setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, now);
  this->category = category;
  this->place = place;
  this->memo = memo;
}

void GeneralSchedule::markAsCompleted()
{
  ScheduleItem::markAsCompleted();
}

string GeneralSchedule::getType() const
{
  return "GENERAL";
}

string GeneralSchedule::serialize() const
{
  std::ostringstream oss;
  oss << serializeCommon() << '|'
      << category << '|'
      << place << '|'
      << memo;
  return oss.str();
}

string GeneralSchedule::getCategory()
{
  return category;
}

string GeneralSchedule::getPlace()
{
  return place;
}

string GeneralSchedule::getMemo()
{
  return memo;
}
