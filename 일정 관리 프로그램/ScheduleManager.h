#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

#include <iostream>
#include <typeinfo>

class ScheduleManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleManager();

  void checkId(int id);

  bool addif(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority);

  int inputInt();

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

  void exit();

private:
  ScheduleItem **scheduleItems = new ScheduleItem *[100];
  string title, description, startDate, endDate, startTime, endTime, priority, category, place, memo, location, participantns, agenda, host, deadline, taskStatus, assignedTo, reminderTime, reminderMessage, notificationType;
  sys_days sdStart{}, sdEnd{};
  minutes st{}, et{};
  bool isDelete;
  int progress;
  int num;
  int choice;
  int idCounter = 0;
  int deleteCount = 0;
  int updateId = 0;
  int count = 0;
};
