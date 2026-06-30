#pragma once

#include "ScheduleItem.h"
#include "GeneralSchedule.h"
#include "MeetingSchedule.h"
#include "TaskSchedule.h"
#include "ReminderSchedule.h"

#include <iostream>
#include <typeinfo>
// STL => vector, memory, algorithm, fstream, sstream, stdexcept
#include <vector> // std::vector: 실제 일정 객체만 순서대로 보관하는 동적 배열 컨테이너
#include <memory> // std::unique_ptr: 일정 객체를 자동으로 delete해 주는 단일 소유 스마트 포인터
#include <algorithm> // <algorithm>: 일정 검색·정렬에 사용하는 표준 알고리즘(find_if, sort 등)을 제공
#include <fstream> // <fstream>: 일정 정보를 파일로 저장·불러오기 위한 파일 입출력 스트림 제공
#include <sstream> // <sstream>: 문자열 한 줄을 공백 단위로 파싱할 때 쓰는 문자열 스트림 제공
#include <stdexcept> // <stdexcept>: 잘못된 상태에서 예외를 던질 때 쓰는 표준 예외 클래스(runtime_error 등)

class ScheduleManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleManager();

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

  void notifyUser(int id);

  void runNotification(int id);

  void exit();

private:
  // unique_ptr => 스마트 포인터, 원소나 ScheduleManager 삭제시 객체 자동 해제
  using ItemPtr = std::unique_ptr<ScheduleItem>;
  std::vector<ItemPtr> scheduleItems;
  string title, description, startDate, endDate, startTime, endTime, priority, category, place, memo, location, participantns, agenda, host, deadline, taskStatus, assignedTo, reminderTime, reminderMessage, notificationType;
  sys_days sdStart{}, sdEnd{};
  minutes st{}, et{};
  int progress;
  int num;
  int choice;
  int idCounter = 0;

  std::vector<ItemPtr>::iterator findById(int id);

  void validateScheduleInput(int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority);

  bool checkConflict(int id, string startDate, string endDate, string startTime, string endTime);

  void saveToFile() const;

  void loadFromFile();
};
