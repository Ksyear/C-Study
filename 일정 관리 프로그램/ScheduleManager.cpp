#include "ScheduleManager.h"

#include <iostream>
#include <typeinfo>

ScheduleManager::ScheduleManager()
{
  for (int i = 0; i < 100; i++)
  {
    scheduleItems[i] = nullptr;
  }
}

bool ScheduleManager::addif(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority)
{
  if (scheduleItems[99] != nullptr)
  {
    cerr << "일정이 가득 찼습니다. 더 이상 추가할 수 없습니다." << scheduleItems[99] << endl;
    return false;
  }
  if (title.empty() || startDate.length() != 10 || endDate.length() != 10 || startTime.length() != 5 || endTime.length() != 5 ||
      // stoi(): 문자열을 숫자로 변환해주는 함수 라고 하네요
      stoi(startDate.substr(0, 3)) > stoi(endDate.substr(0, 3)) || stoi(startDate.substr(5, 6)) > stoi(endDate.substr(5, 6)) || stoi(startDate.substr(8, 9)) > stoi(endDate.substr(8, 9)) ||
      stoi(startTime.substr(0, 1)) > stoi(endTime.substr(0, 1)) || stoi(startTime.substr(3, 4)) > stoi(endTime.substr(3, 4)) ||
      stoi(startDate.substr(5, 6)) < 0 || stoi(startDate.substr(5, 6)) > 12 || stoi(startDate.substr(8, 9)) < 0 || stoi(startDate.substr(8, 9)) > 31 ||
      stoi(endDate.substr(0, 1)) < 0 || stoi(endDate.substr(0, 1)) > 12 || stoi(endDate.substr(3, 4)) < 0 || stoi(endDate.substr(3, 4)) > 31 ||
      stoi(startTime.substr(0, 1)) < 0 || stoi(startTime.substr(0, 1)) > 23 || stoi(startTime.substr(3, 4)) < 0 || stoi(startTime.substr(3, 4)) > 59 ||
      stoi(endTime.substr(0, 1)) < 0 || stoi(endTime.substr(0, 1)) > 23 || stoi(endTime.substr(3, 4)) < 0 || stoi(endTime.substr(3, 4)) > 59 ||
      startDate.substr(4, 1) != "-" || startDate.substr(7, 1) != "-" ||
      endDate.substr(4, 1) != "-" || endDate.substr(7, 1) != "-" ||
      startTime.substr(2, 1) != ":" || endTime.substr(2, 1) != ":" ||
      priority != "LOW" && priority != "MEDIUM" && priority != "HIGH")
  {
    cerr << "잘못된 날짜 형식입니다." << endl;
    return false;
  }
  else if ((checkConglict(id, startDate, endDate, startTime, endTime)))
  {
    cout << "Title: " << title
         << ", Description: " << description
         << ", Start Date: " << startDate
         << ", End Date: " << endDate
         << ", Start Time: " << startTime
         << ", End Time: " << endTime
         << ", Priority: " << priority
         << endl;
    return false;
  }
  return true;
}

void ScheduleManager::addSchedule()
{
  string title, description, startDate, endDate, startTime, endTime, priority, category, place, memo, location, participantns, agenda, host, deadline, taskStatus, assignedTo, reminderTime, reminderMessage, notificationType;
  int progress;
  int choice;
  cin >> choice;
  switch (choice)
  {
  case 1:
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cout << "값을 입력하시오" << endl;
        cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> category >> place >> memo;
        if (addif(idCounter - 1, title, description, startDate, endDate, startTime, endTime, priority))
        {
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new GeneralSchedule(idCounter++, title, description, startDate, endDate, startTime, endTime, priority, now, category, place, memo);
          cout << "일정이 추가되었습니다." << endl;
        }
        break;
      }
    }
    break;
  }
  case 2:
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cout << "값을 입력하시오" << endl;
        cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> location >> participantns >> agenda >> host;
        if (addif(idCounter - 1, title, description, startDate, endDate, startTime, endTime, priority))
        {
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new MeetingSchedule(idCounter++, title, description, startDate, endDate, startTime, endTime, priority, now, location, participantns, agenda, host);
          cout << "일정이 추가되었습니다." << endl;
        }
        break;
      }
    }
    break;
  }
  case 3:
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cout << "값을 입력하시오" << endl;
        cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> deadline >> progress >> taskStatus >> assignedTo;
        if (addif(idCounter - 1, title, description, startDate, endDate, startTime, endTime, priority))
        {
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new TaskSchedule(idCounter++, title, description, startDate, endDate, startTime, endTime, priority, now, deadline, progress, taskStatus, assignedTo);
          cout << "일정이 추가되었습니다." << endl;
        }
        break;
      }
    }
    break;
  }
  case 4:
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cout << "값을 입력하시오" << endl;
        cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> reminderTime >> reminderMessage >> notificationType;
        if (addif(idCounter - 1, title, description, startDate, endDate, startTime, endTime, priority))
        {
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new ReminderSchedule(idCounter++, title, description, startDate, endDate, startTime, endTime, priority, now, reminderTime, reminderMessage, notificationType);
          cout << "일정이 추가되었습니다." << endl;
        }
        break;
      }
    }
    break;
  }
  default:
  {
    cerr << "잘못된 입력입니다." << endl;
    break;
  }
  }
}

void ScheduleManager::displayAllSchedules()
{
  if (scheduleItems[0] == nullptr)
  {
    cerr << "일정이 없습니다." << endl;
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
      break;
    scheduleItems[i]->displayAllSchedules();
    cout << endl;
  }
}

void ScheduleManager::displaySchedule(int id)
{
  if (id - 1 > 100 || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  scheduleItems[id - 1]->displayAllSchedules();
  cout << endl;
  return;
}

void ScheduleManager::updateSchedule(int id)
{
  if (id - 1 > 100 || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  cout << "수정할 값을 입력하시오: title, description, startDate, endDate, startTime, endTime, priority" << endl;
  string title, description, startDate, endDate, startTime, endTime, priority;
  cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority;
  if ((checkConglict(id - 1, startDate, endDate, startTime, endTime)))
  {
    cout << "Title: " << title
         << ", Description: " << description
         << ", Start Date: " << startDate
         << ", End Date: " << endDate
         << ", Start Time: " << startTime
         << ", End Time: " << endTime
         << ", Priority: " << priority
         << endl;
    return;
  }
  else if (addif(id - 1, title, description, startDate, endDate, startTime, endTime, priority))
  {
    if (typeid(*scheduleItems[id - 1]) == typeid(GeneralSchedule))
    {
      string category, place, memo;
      cout << "값을 입력하시오: category, place, memo" << endl;
      cin >> category >> place >> memo;
      // dynamic_cast: 부모 클래스 포인터를 자식 클래스 포인터로 변환하는 연산자라고 하네여
      GeneralSchedule *GS = dynamic_cast<GeneralSchedule *>(scheduleItems[id - 1]);
      GS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), category, place, memo);
    }
    else if (typeid(*scheduleItems[id - 1]) == typeid(MeetingSchedule))
    {
      string location, participantns, agenda, host;
      cout << "값을 입력하시오: location, participants, agenda, host" << endl;
      cin >> location >> participantns >> agenda >> host;
      MeetingSchedule *MS = dynamic_cast<MeetingSchedule *>(scheduleItems[id - 1]);
      MS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), location, participantns, agenda, host);
    }
    else if (typeid(*scheduleItems[id - 1]) == typeid(TaskSchedule))
    {
      string deadline, taskStatus, assignedTo;
      int progress;
      cout << "값을 입력하시오: deadline, progress, taskStatus, assignedTo" << endl;
      cin >> deadline >> progress >> taskStatus >> assignedTo;
      TaskSchedule *TS = dynamic_cast<TaskSchedule *>(scheduleItems[id - 1]);
      TS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), deadline, progress, taskStatus, assignedTo);
    }
    else if (typeid(*scheduleItems[id - 1]) == typeid(ReminderSchedule))
    {
      string reminderTime, reminderMessage, notificationType;
      cout << "값을 입력하시오: reminderTime, reminderMessage, notificationType" << endl;
      cin >> reminderTime >> reminderMessage >> notificationType;
      ReminderSchedule *RS = dynamic_cast<ReminderSchedule *>(scheduleItems[id - 1]);
      RS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), reminderTime, reminderMessage, notificationType);
    }
  }
}

void ScheduleManager::deleteSchedule(int id)
{
  if (id - 1 > 100 || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  delete scheduleItems[id - 1];
  scheduleItems[id - 1] = nullptr;
  for (int i = id - 1; i < 100; i++)
  {
    scheduleItems[i] = scheduleItems[i + 1];
    if (scheduleItems[i] != nullptr)
    {
      scheduleItems[i]->setId(i + 1);
    }
    else
    {
      break;
    }
  }
  cout << "일정이 삭제되었습니다: ID " << id << endl;
}

void ScheduleManager::completeSchedule(int id)
{
  int num;
  if (scheduleItems[id - 1] != nullptr && scheduleItems[id - 1]->getId() == id)
  {
    scheduleItems[id - 1]->markAsCompleted();
  }
  else
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  }
}

void ScheduleManager::searchByTitle(string title)
{
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      cout << "일정을 찾을 수 없습니다: title " << title << endl;
      return;
    }
    if (scheduleItems[i]->getTitle() == title)
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
      return;
    }
  }
}

void ScheduleManager::searchByDate(string startDate)
{
  if (scheduleItems[0] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다: startDate " << startDate << endl;
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      return;
    }
    if (scheduleItems[i]->getStartDate() == startDate)
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
}

void ScheduleManager::searchByPriority(string priority)
{
  if (scheduleItems[0] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다: priority " << priority << endl;
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      return;
    }
    if (scheduleItems[i]->getPriority() == priority)
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
}

void ScheduleManager::sortByDate()
{
  int sortId[100];
  if (scheduleItems[0] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }

  int count = 0;
  while (count < 100 && scheduleItems[count] != nullptr)
  {
    count++;
  }

  for (int i = 0; i < count - 1; i++)
  {
    int minIndex = i;
    for (int j = i + 1; j < count; j++)
    {
      if (scheduleItems[j]->getStartDate() < scheduleItems[minIndex]->getStartDate())
      {
        minIndex = j;
      }
      else if (scheduleItems[j]->getStartDate() == scheduleItems[minIndex]->getStartDate())
      {
        if (scheduleItems[j]->getStartTime() < scheduleItems[minIndex]->getStartTime())
        {
          minIndex = j;
        }
      }
    }
    if (minIndex != i)
    {
      ScheduleItem *temp = scheduleItems[i];
      scheduleItems[i] = scheduleItems[minIndex];
      scheduleItems[minIndex] = temp;
    }
  }

  for (int i = 0; i < count; i++)
  {
    scheduleItems[i]->setId(i + 1);
  }

  displayAllSchedules();

  return;
}

void ScheduleManager::sortByPriority()
{
  if (scheduleItems[0] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
    if (scheduleItems[i]->getPriority() == "HIGH")
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
    if (scheduleItems[i]->getPriority() == "MEDIUM")
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
    if (scheduleItems[i]->getPriority() == "LOW")
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
  return;
}

void ScheduleManager::sortByCompleted()
{
  if (scheduleItems[0] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다" << endl;
    return;
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
    if (scheduleItems[i]->getIsCompleted() == false)
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
    if (scheduleItems[i]->getIsCompleted() == true)
    {
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }
  return;
}

bool ScheduleManager::checkConglict(int id, string startDate, string endDate, string startTime, string endTime)
{
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      return false;
    }
    if (scheduleItems[i]->getId() == id)
    {
      continue;
    }
    else if ((scheduleItems[i]->getStartDate() + " " + scheduleItems[i]->getStartTime()) <= (endDate + " " + endTime) &&
             (startDate + " " + startTime) < (scheduleItems[i]->getEndDate() + " " + scheduleItems[i]->getEndTime()))
    {
      cout << "충돌이 났습니다. " << endl;
      return true;
    }
    else
    {
      continue;
    }
  }
  return false;
}

void ScheduleManager::notifyUser(int id)
{
  if (scheduleItems[id - 1] != nullptr && scheduleItems[id - 1]->getId() == id)
  {
    ReminderSchedule *RS = dynamic_cast<ReminderSchedule *>(scheduleItems[id - 1]);
    if (RS != nullptr && !RS->getIsReminderSent())
    {
      cout << RS->getReminderMessage() << endl;

      RS->setIsReminderSent(true);
      return;
    }
    cout << "이 일정은 알림 대상이 아닙니다.\n";
  }
  else
  {
    cout << "일정을 찾을 수 없습니다: ID " << id << endl;
  }
}

void ScheduleManager::runNotification(int id)
{
  if (scheduleItems[id - 1] != nullptr && scheduleItems[id - 1]->getId() == id)
  {
    notifyUser(id);
  }
  else
  {
    cout << "일정을 찾을 수 없습니다: ID " << id << endl;
  }
}
