#pragma once

#include "ScheduleItem.cpp"

class ReminderSchedule : public ScheduleItem
{
public:
  ReminderSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
      : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->reminderTime = reminderTime;
    this->reminderMessage = reminderMessage;
    this->notificationType = notificationType;
    this->isReminderSent = false;
  }

  void displayAllSchedules() override
  {
    ScheduleItem::displayAllSchedules();
    cout << ", Reminder Time: " << reminderTime
         << ", Reminder Message: " << reminderMessage
         << ", Notification Type: " << notificationType
         << ", Is Reminder Sent: " << isReminderSent;
  }

  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
  {
    ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
    this->reminderTime = reminderTime;
    this->reminderMessage = reminderMessage;
    this->notificationType = notificationType;
  }

  void markAsCompleted() override
  {
    ScheduleItem::markAsCompleted();
  }

  string getReminderMessage() override
  {
    return reminderMessage;
  }
  
  bool getIsReminderSent()
  {
    return isReminderSent;
  }

  void setIsReminderSent(bool isReminderSent)
  {
    this->isReminderSent = isReminderSent;
  }

private:
  string reminderTime, reminderMessage, notificationType;
  system_clock::time_point now = system_clock::now();
  bool isReminderSent;
};
