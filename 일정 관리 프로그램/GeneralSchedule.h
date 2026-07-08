#pragma once

#include "ScheduleItem.h"

class GeneralSchedule : public ScheduleItem
{
public:
  GeneralSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo);

  void displayAllSchedules() override;

  using ScheduleItem::setInfo;
  void setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo);

  void markAsCompleted() override;

  string getType() const override;

  string serialize() const override;

  string getCategory();

  string getPlace();

  string getMemo();

private:
  string category, place, memo;
  system_clock::time_point now = system_clock::now();
};
