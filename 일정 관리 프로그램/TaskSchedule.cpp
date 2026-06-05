#pragma once

#include "ScheduleItem.cpp"

class TaskSchedule : public ScheduleItem
{
public:
  TaskSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo)
      : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->deadline = deadline;
    this->progress = progress;
    this->taskStatus = taskStatus;
    this->assignedTo = assignedTo;
  }

  void displayAllSchedules() override
  {
    ScheduleItem::displayAllSchedules();
    cout << ", Deadline: " << deadline
         << ", Progress: " << progress
         << ", Task Status: " << taskStatus
         << ", Assigned To: " << assignedTo;
  }

  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo)
  {
    ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
    this->deadline = deadline;
    this->progress = progress;
    this->taskStatus = taskStatus;
    this->assignedTo = assignedTo;
  }

  void markAsCompleted() override
  {
    ScheduleItem::markAsCompleted();
    this->progress = 100;
    this->taskStatus = "DONE";
  }

  string getDeadline()
  {
    return deadline;
  }

  int getProgress()
  {
    return progress;
  }

  string getTaskStatus()
  {
    return taskStatus;
  }

  string getAssignedTo()
  {
    return assignedTo;
  }

private:
  string deadline, taskStatus, assignedTo;
  int progress;
  system_clock::time_point now = system_clock::now();
};
