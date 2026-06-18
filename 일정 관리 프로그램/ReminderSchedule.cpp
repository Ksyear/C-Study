#include "ReminderSchedule.h"

#include <iostream>

ReminderSchedule::ReminderSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
    : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
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

void ReminderSchedule::setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType)
{
  ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
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

bool ReminderSchedule::getIsReminderSent()
{
  return isReminderSent;
}

void ReminderSchedule::setIsReminderSent(bool isReminderSent)
{
  this->isReminderSent = isReminderSent;
}
