#include "TaskSchedule.h"

#include <iostream>

TaskSchedule::TaskSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo)
    : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
{
  this->deadline = deadline;
  this->progress = progress;
  this->taskStatus = taskStatus;
  this->assignedTo = assignedTo;
}

void TaskSchedule::displayAllSchedules()
{
  ScheduleItem::displayAllSchedules();
  cout << ", Deadline: " << deadline
       << ", Progress: " << progress
       << ", Task Status: " << taskStatus
       << ", Assigned To: " << assignedTo;
}

void TaskSchedule::setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo)
{
  ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
  this->deadline = deadline;
  this->progress = progress;
  this->taskStatus = taskStatus;
  this->assignedTo = assignedTo;
}

void TaskSchedule::markAsCompleted()
{
  ScheduleItem::markAsCompleted();
  this->progress = 100;
  this->taskStatus = "DONE";
}

string TaskSchedule::getDeadline()
{
  return deadline;
}

int TaskSchedule::getProgress()
{
  return progress;
}

string TaskSchedule::getTaskStatus()
{
  return taskStatus;
}

string TaskSchedule::getAssignedTo()
{
  return assignedTo;
}
