#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

class ScheduleSearch
{
public:
  // unique_ptr => 스마트 포인터, 원소나 소유 객체 삭제시 객체 자동 해제
  using ItemPtr = std::unique_ptr<ScheduleItem>;

  // ScheduleManager가 소유한 scheduleItems를 참조로 받아 그대로 들여다본다.
  explicit ScheduleSearch(const std::vector<ItemPtr> &items);

  void searchByScheduleByUserId(int userId);

  void searchByUserAndTitle(int userId, const string &title);

  void searchByUserAndDate(int userId, const string &startDate);

  void searchByUserAndPriority(int userId, const string &priority);

  void searchByTitle(string title);

  void searchByDate(string startDate);

  void searchByPriority(string priority);

private:
  // ScheduleManager의 벡터를 그대로 참조 => 데이터 공유
  const std::vector<ItemPtr> &scheduleItems;
};
