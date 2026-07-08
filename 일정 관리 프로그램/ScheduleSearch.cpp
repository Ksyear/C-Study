#include "ScheduleSearch.h"

ScheduleSearch::ScheduleSearch(const std::vector<ItemPtr> &items): scheduleItems(items)
{
}

void ScheduleSearch::searchByScheduleByUserId(int userId)
{
  bool found = false;
  // 비교하지 말고 할것 if 쓰지말아라
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << endl;
  }
}

void ScheduleSearch::searchByUserAndTitle(int userId, const string &title)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId && item->getTitle() == title)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << ", title " << title << endl;
  }
}

void ScheduleSearch::searchByUserAndDate(int userId, const string &startDate)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId && item->getStartDate() == startDate)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << ", startDate " << startDate << endl;
  }
}

void ScheduleSearch::searchByUserAndPriority(int userId, const string &priority)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId && item->getPriority() == priority)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: userId " << userId << ", priority " << priority << endl;
  }
}


void ScheduleSearch::searchByTitle(string title)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getTitle() == title)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: title " << title << endl;
  }
}

void ScheduleSearch::searchByDate(string startDate)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getStartDate() == startDate)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: startDate " << startDate << endl;
  }
}

void ScheduleSearch::searchByPriority(string priority)
{
  bool found = false;
  for (const auto &item : scheduleItems)
  {
    if (item->getPriority() == priority)
    {
      item->displayAllSchedules();
      cout << endl;
      found = true;
    }
  }
  if (!found)
  {
    cout << "일정을 찾을 수 없습니다: priority " << priority << endl;
  }
}
