#pragma once

#include "ScheduleItem.cpp"

class GeneralSchedule : public ScheduleItem
{
public:
  GeneralSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
      : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->category = category;
    this->place = place;
    this->memo = memo;
  }

  void displayAllSchedules() override
  {
    ScheduleItem::displayAllSchedules();
    cout << ", Category: " << category
         << ", Place: " << place
         << ", Memo: " << memo;
  }

  using ScheduleItem::setInfo;
  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
  {
    ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
    this->category = category;
    this->place = place;
    this->memo = memo;
  }

  void markAsCompleted() override
  {
    ScheduleItem::markAsCompleted();
  }

  string getCategory()
  {
    return category;
  }

  string getPlace()
  {
    return place;
  }

  string getMemo()
  {
    return memo;
  }

private:
  string category, place, memo;
  system_clock::time_point now = system_clock::now();
};
