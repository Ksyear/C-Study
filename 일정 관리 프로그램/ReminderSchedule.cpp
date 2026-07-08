#include "ReminderSchedule.h"

ReminderSchedule::ReminderSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
    : ScheduleItem(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now)
{
  this->reminderTime = reminderTime;
  this->reminderMessage = reminderMessage;
  this->notificationType = notificationType;
  this->isReminderSent = false;
}

void ReminderSchedule::displayAllSchedules()
{
  ScheduleItem::displayAllSchedules();
  cout << ", Reminder Time: " << reminderTime
       << ", Reminder Message: " << reminderMessage
       << ", Notification Type: " << notificationType
       << ", Is Reminder Sent: " << isReminderSent;
}

void ReminderSchedule::setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
{
  ScheduleItem::setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, now);
  this->reminderTime = reminderTime;
  this->reminderMessage = reminderMessage;
  this->notificationType = notificationType;
}

void ReminderSchedule::markAsCompleted()
{
  ScheduleItem::markAsCompleted();
}

string ReminderSchedule::getReminderMessage()
{
  return reminderMessage;
}

string ReminderSchedule::getType() const
{
  return "REMINDER";
}

string ReminderSchedule::serialize() const
{
  std::ostringstream oss;
  oss << serializeCommon() << '|'
      << reminderTime << '|'
      << reminderMessage << '|'
      << notificationType << '|'
      << isReminderSent;
  return oss.str();
}

bool ReminderSchedule::getIsReminderSent()
{
  return isReminderSent;
}

void ReminderSchedule::setIsReminderSent(bool isReminderSent)
{
  this->isReminderSent = isReminderSent;
}
