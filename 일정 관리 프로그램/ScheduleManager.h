#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

class ScheduleManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleManager();

  bool addif(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority);

  void addSchedule();

  void displayAllSchedules();

  void displaySchedule(int id);

  void updateSchedule(int id);

  void deleteSchedule(int id);

  void completeSchedule(int id);

  void searchByTitle(string title);

  void searchByDate(string startDate);

  void searchByPriority(string priority);

  void sortByDate();

  void sortByPriority();

  void sortByCompleted();

  bool checkConglict(int id, string startDate, string endDate, string startTime, string endTime);

  void notifyUser(int id);

  void runNotification(int id);

private:
  ScheduleItem **scheduleItems = new ScheduleItem *[10];
  int idCounter = 0;
};
