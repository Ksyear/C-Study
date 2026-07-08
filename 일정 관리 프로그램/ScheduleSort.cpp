#include "ScheduleSort.h"

ScheduleSort::ScheduleSort(std::vector<ItemPtr> &items) : scheduleItems(items) {}

namespace
{
  // sort를 하기 위해 int 값으로 설정
  int priorityRank(const std::string &priority)
  {
    if (priority == "HIGH")
      return 0;
    if (priority == "MEDIUM")
      return 1;
    return 2;
  }
}

void ScheduleSort::sortByUserAndDate(int userId)
{
  std::vector<ScheduleItem *> userSchedules;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId)
    {
      userSchedules.push_back(item.get());
    }
  }

  if (userSchedules.empty())
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << endl;
    return;
  }

  std::sort(userSchedules.begin(), userSchedules.end(),
            [](ScheduleItem *a, ScheduleItem *b)
            {
              if (a->getStartDate() != b->getStartDate())
              {
                return a->getStartDate() < b->getStartDate();
              }
              return a->getStartTime() < b->getStartTime();
            });

  for (const auto &item : userSchedules)
  {
    item->displayAllSchedules();
    cout << endl;
  }
}

void ScheduleSort::sortByUserAndPriority(int userId)
{
  std::vector<ScheduleItem *> userSchedules;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId)
    {
      userSchedules.push_back(item.get());
    }
  }

  if (userSchedules.empty())
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << endl;
    return;
  }

  std::sort(userSchedules.begin(), userSchedules.end(),
            [](ScheduleItem *a, ScheduleItem *b)
            {
              return priorityRank(a->getPriority()) < priorityRank(b->getPriority());
            });

  for (const auto &item : userSchedules)
  {
    item->displayAllSchedules();
    cout << endl;
  }
}

void ScheduleSort::sortByUserAndCompleted(int userId)
{
  std::vector<ScheduleItem *> userSchedules;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId)
    {
      userSchedules.push_back(item.get());
    }
  }

  if (userSchedules.empty())
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << endl;
    return;
  }

  std::sort(userSchedules.begin(), userSchedules.end(),
            [](ScheduleItem *a, ScheduleItem *b)
            {
              return a->getIsCompleted() < b->getIsCompleted();
            });

  for (const auto &item : userSchedules)
  {
    item->displayAllSchedules();
    cout << endl;
  }
}

void ScheduleSort::sortByDate()
{
  if (scheduleItems.empty())
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }
  // std::sort(시작_반복자, 끝_반복자, [](const 타입 &a, const 타입 &b) { if(a가 b보다 앞에 와야 하는 기준(조건)을 true로 반환) return 조건; });
  std::sort(scheduleItems.begin(), scheduleItems.end(),
            [](const ItemPtr &a, const ItemPtr &b)
            {
              if (a->getStartDate() != b->getStartDate())
              {
                return a->getStartDate() < b->getStartDate();
              }
              return a->getStartTime() < b->getStartTime();
            });
}

void ScheduleSort::sortByPriority()
{
  if (scheduleItems.empty())
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }
  std::sort(scheduleItems.begin(), scheduleItems.end(),
            [](const ItemPtr &a, const ItemPtr &b)
            {
              return priorityRank(a->getPriority()) < priorityRank(b->getPriority());
            });
}

void ScheduleSort::sortByCompleted()
{
  if (scheduleItems.empty())
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }
  std::sort(scheduleItems.begin(), scheduleItems.end(),
            [](const ItemPtr &a, const ItemPtr &b)
            {
              return a->getIsCompleted() < b->getIsCompleted();
            });
}
