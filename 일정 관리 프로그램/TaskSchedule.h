#pragma once

#include "ScheduleItem.h"

class TaskSchedule : public ScheduleItem
{
public:
  TaskSchedule(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo);

  void displayAllSchedules() override;

  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, int progress, string taskStatus, string assignedTo);

  void markAsCompleted() override;

  string getType() const override;

  string serialize() const override;

  string getDeadline();

  int getProgress();

  string getTaskStatus();

  string getAssignedTo();

private:
  string deadline, taskStatus, assignedTo;
  int progress;
  system_clock::time_point now = system_clock::now();
};
