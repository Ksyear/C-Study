#pragma once

#include "ScheduleItem.h"

class ReminderSchedule : public ScheduleItem
{
public:
  ReminderSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType);

  void displayAllSchedules() override;

  void setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType);

  void markAsCompleted() override;

  string getReminderMessage() override;

  string getType() const override;

  string serialize() const override;

  bool getIsReminderSent();

  void setIsReminderSent(bool isReminderSent);

private:
  string reminderTime, reminderMessage, notificationType;
  system_clock::time_point now = system_clock::now();
  bool isReminderSent;
};
