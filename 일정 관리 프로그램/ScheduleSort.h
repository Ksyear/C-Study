#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

class ScheduleSort
{
public:
  using ItemPtr = std::unique_ptr<ScheduleItem>;

  // ScheduleManager가 소유한 scheduleItems를 참조로 받아 원본을 정렬한다.
  explicit ScheduleSort(std::vector<ItemPtr> &items);

  void sortByUserAndDate(int userId);

  void sortByUserAndPriority(int userId);

  void sortByUserAndCompleted(int userId);

  void sortByDate();

  void sortByPriority();

  void sortByCompleted();

private:
  // ScheduleManager의 벡터를 그대로 참조 => 데이터 공유 (원본 정렬을 위해 non-const)
  std::vector<ItemPtr> &scheduleItems;
};
