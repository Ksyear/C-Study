#include "GeneralSchedule.h"

#include <iostream>

GeneralSchedule::GeneralSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
    : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
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

void GeneralSchedule::setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
{
  ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
  this->category = category;
  this->place = place;
  this->memo = memo;
}

void GeneralSchedule::markAsCompleted()
{
  ScheduleItem::markAsCompleted();
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
