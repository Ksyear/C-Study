#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

class ScheduleAddUpdate
{
public:
  // unique_ptr => 스마트 포인터, 원소나 소유 객체 삭제시 객체 자동 해제
  using ItemPtr = std::unique_ptr<ScheduleItem>;

  // ScheduleManager가 소유한 scheduleItems를 참조로 받아 직접 수정한다.
  explicit ScheduleAddUpdate(std::vector<ItemPtr> &items);

  void addSchedule();

  void updateSchedule(int id);

  void setIdCounter(int idCounter);

private:
  // ScheduleManager의 벡터를 그대로 참조 => 데이터 공유 (add/update는 벡터를 수정하므로 non-const)
  std::vector<ItemPtr> &scheduleItems;

  string title, description, startDate, endDate, startTime, endTime, priority, category, place, memo, location, agenda, deadline, taskStatus, assignedTo, reminderTime, reminderMessage, notificationType, stringParticipants;
  int host;
  std::vector<int> participants;
  sys_days sdStart{}, sdEnd{};
  minutes st{}, et{};
  int progress;
  int num;
  int choice;
  int idCounter = 0;
  int userId = 0;

  // ScheduleManager의 add/update가 쓰던 보조 함수들. 값 멤버 ScheduleManager를 없앴으므로
  // 이 클래스가 scheduleItems 참조만으로 자립 동작하도록 여기서 직접 보유한다.
  int inputInt();

  std::vector<ItemPtr>::iterator findById(int id);

  bool checkUserIdConflict(int userId);

  void validateScheduleInput(int userId, int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority);

  bool checkConflict(int userId, int id, const string &startDate, const string &endDate, const string &startTime, const string &endTime);

  void saveToFile() const;
};