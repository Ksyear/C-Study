#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ScheduleAddUpdate.h"
#include "ScheduleSearch.h"
#include "ScheduleSort.h"
#include "ReminderSchedule.h"

class ScheduleManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleManager();

  int inputInt();

  string inputString();

  void addSchedule();

  void displayAllSchedules();

  void displaySchedule(int id);

  void updateSchedule(int id);

  void deleteSchedule(int id);

  void completeSchedule(int id);

  void searchByScheduleByUserId(int userId);

  void searchByUserAndTitle(int userId, const string &title);

  void searchByUserAndDate(int userId, const string &startDate);

  void searchByUserAndPriority(int userId, const string &priority);

  void searchByTitle(string title);

  void searchByDate(string startDate);

  void searchByPriority(string priority);

  void sortByUserAndDate(int userId);

  void sortByUserAndPriority(int userId);

  void sortByUserAndCompleted(int userId);

  void sortByDate();

  void sortByPriority();

  void sortByCompleted();

  void notifyUser(int id);

  void runNotification(int id);

  void exit();

  bool checkUserIdConflict(int userId);

private:
  // unique_ptr => 스마트 포인터, 원소나 ScheduleManager 삭제시 객체 자동 해제
  using ItemPtr = std::unique_ptr<ScheduleItem>;
  std::vector<ItemPtr> scheduleItems;

  // 검색 기능 담당 객체. scheduleItems를 참조로 공유 => 반드시 scheduleItems 뒤에 선언 => 초기화 순서를 맞추기(나머지도 동일)
  ScheduleSearch search;
  ScheduleSort sort;
  ScheduleAddUpdate addUpdate;

  string String;
  int num;

  sys_days sdStart{}, sdEnd{};
  minutes st{}, et{};

  std::vector<ItemPtr>::iterator findById(int id);

  void validateScheduleInput(int userId, int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority);

  bool checkConflict(int userId, int id, const string &startDate, const string &endDate, const string &startTime, const string &endTime);

  void saveToFile() const;

  void loadFromFile();
};
