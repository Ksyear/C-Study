#include "ScheduleManager.h"

// 해야될거: 시간날짜 타입 형태 변경, 생성이랑 수정 시간 표현 정확히 할것

ScheduleManager::ScheduleManager()
{
  for (int i = 0; i < 100; i++)
  {
    scheduleItems[i] = nullptr;
  }
}

void ScheduleManager::checkId(int id)
{
  for (int i = 0; i < 100; i++)
  {
    if (scheduleItems[i] == nullptr)
    {
      cout << "일정을 찾을 수 없습니다." << endl;
      isDelete = true;
      return;
    }
    else if (scheduleItems[i]->getId() == id)
    {
      updateId = i;
      isDelete = false;
      break;
    }
  }
}

bool ScheduleManager::addif(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority)
{
  if (scheduleItems[99] != nullptr)
  {
    cerr << "일정이 가득 찼습니다. 더 이상 추가할 수 없습니다." << scheduleItems[99] << endl;
    return false;
  }

  std::istringstream testStartDate{startDate};
  testStartDate >> parse("%F", sdStart);
  std::istringstream testEndDate{endDate};
  testEndDate >> parse("%F", sdEnd);
  std::istringstream testStartTime{startTime};
  testStartTime >> parse("%R", st);
  std::istringstream testEndTime{endTime};
  testEndTime >> parse("%R", et);

  if (title.empty() || startDate.length() != 10 || endDate.length() != 10 || startTime.length() != 5 || endTime.length() != 5 ||
      // // stoi(): 문자열을 숫자로 변환해주는 함수 라고 하네요
      // stoi(startDate.substr(0, 3)) > stoi(endDate.substr(0, 3)) || stoi(startDate.substr(5, 6)) > stoi(endDate.substr(5, 6)) || stoi(startDate.substr(8, 9)) > stoi(endDate.substr(8, 9)) ||
      // stoi(startTime.substr(0, 1)) > stoi(endTime.substr(0, 1)) || stoi(startTime.substr(3, 4)) > stoi(endTime.substr(3, 4)) ||
      // stoi(startDate.substr(5, 6)) < 0 || stoi(startDate.substr(5, 6)) > 12 || stoi(startDate.substr(8, 9)) < 0 || stoi(startDate.substr(8, 9)) > 31 ||
      // stoi(endDate.substr(0, 1)) < 0 || stoi(endDate.substr(0, 1)) > 12 || stoi(endDate.substr(3, 4)) < 0 || stoi(endDate.substr(3, 4)) > 31 ||
      // stoi(startTime.substr(0, 1)) < 0 || stoi(startTime.substr(0, 1)) > 23 || stoi(startTime.substr(3, 4)) < 0 || stoi(startTime.substr(3, 4)) > 59 ||
      // stoi(endTime.substr(0, 1)) < 0 || stoi(endTime.substr(0, 1)) > 23 || stoi(endTime.substr(3, 4)) < 0 || stoi(endTime.substr(3, 4)) > 59 ||
      // startDate.substr(4, 1) != "-" || startDate.substr(7, 1) != "-" ||
      // endDate.substr(4, 1) != "-" || endDate.substr(7, 1) != "-" ||
      // startTime.substr(2, 1) != ":" || endTime.substr(2, 1) != ":" ||
      !testStartDate || !testEndDate || !testStartTime || !testEndTime ||
      (sdStart > sdEnd) || (sdStart == sdEnd && st >= et) ||
      priority != "LOW" && priority != "MEDIUM" && priority != "HIGH")
  {
    cerr << "잘못된 날짜/시간/우선순위 형식입니다." << endl;
    return false;
  }
  else if ((checkConglict(id, startDate, endDate, startTime, endTime)))
  {
    return false;
  }
  return true;
}

int ScheduleManager::inputInt(){
  while (true)
  {
    cin >> num;
    if (cin.fail())
    {
      cout << "잘못된 입력입니다." << endl;
      cin.clear();
      cin.ignore(1000, '\n');
    }
    else
    {
      break;
    }
  }
  return num;
}

void ScheduleManager::addSchedule()
{
  choice = inputInt();
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
  if (id - 1 > 100 + deleteCount || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  checkId(id);
  if (!isDelete)
  {
    scheduleItems[updateId]->displayAllSchedules();
  }
  return;
}

void ScheduleManager::updateSchedule(int id)
{
  if (id - 1 > 100 + deleteCount || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  checkId(id);
  if (isDelete)
    return;
  cout << "수정할 값을 입력하시오: title, description, startDate, endDate, startTime, endTime, priority" << endl;
  cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority;
  if ((checkConglict(updateId + 1, startDate, endDate, startTime, endTime)))
  {
    return;
  }
  else if (addif(updateId + 1, title, description, startDate, endDate, startTime, endTime, priority))
  {
    if (typeid(*scheduleItems[updateId]) == typeid(GeneralSchedule))
    {
      cout << "값을 입력하시오: category, place, memo" << endl;
      cin >> category >> place >> memo;
      // dynamic_cast: 부모 클래스 포인터를 자식 클래스 포인터로 변환하는 연산자라고 하네여
      GeneralSchedule *GS = dynamic_cast<GeneralSchedule *>(scheduleItems[updateId]);
      GS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), category, place, memo);
    }
    else if (typeid(*scheduleItems[updateId]) == typeid(MeetingSchedule))
    {
      cout << "값을 입력하시오: location, participants, agenda, host" << endl;
      cin >> location >> participantns >> agenda >> host;
      MeetingSchedule *MS = dynamic_cast<MeetingSchedule *>(scheduleItems[updateId]);
      MS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), location, participantns, agenda, host);
    }
    else if (typeid(*scheduleItems[updateId]) == typeid(TaskSchedule))
    {
      cout << "값을 입력하시오: deadline, progress, taskStatus, assignedTo" << endl;
      cin >> deadline >> progress >> taskStatus >> assignedTo;
      TaskSchedule *TS = dynamic_cast<TaskSchedule *>(scheduleItems[updateId]);
      TS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), deadline, progress, taskStatus, assignedTo);
    }
    else if (typeid(*scheduleItems[updateId]) == typeid(ReminderSchedule))
    {
      cout << "값을 입력하시오: reminderTime, reminderMessage, notificationType" << endl;
      cin >> reminderTime >> reminderMessage >> notificationType;
      ReminderSchedule *RS = dynamic_cast<ReminderSchedule *>(scheduleItems[updateId]);
      RS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), reminderTime, reminderMessage, notificationType);
    }
    cout << "수정되었습니다.\n";
  }
}

void ScheduleManager::deleteSchedule(int id)
{
  if (id - 1 > 100 + deleteCount || id - 1 < 0 || scheduleItems[id - 1] == nullptr)
  {
    cout << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  checkId(id);
  delete scheduleItems[updateId];
  scheduleItems[updateId] = nullptr;
  for (int i = updateId; i < 100; i++)
  {
    scheduleItems[i] = scheduleItems[i + 1];
    if (scheduleItems[i] == nullptr)
    {
      break;
    }
  }
  deleteCount++;
  cout << "일정이 삭제되었습니다: ID " << id << endl;
}

void ScheduleManager::completeSchedule(int id)
{
  checkId(id);
  if (scheduleItems[updateId] != nullptr && scheduleItems[updateId]->getId() == id)
  {
    scheduleItems[updateId]->markAsCompleted();
    cout << "일정이 완료되었습니다." << endl;
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
  while (count < 100 && scheduleItems[count] != nullptr)
  {
    count++;
  }
  for (int i = 0; i < count; i++)
  {
    sortId[i] = i;
  }
  for (int i = 0; i < count - 1; i++)
  {
    int minIndex = i;
    for (int j = i + 1; j < count; j++)
    {
      ScheduleItem *curItem = scheduleItems[sortId[j]];
      ScheduleItem *minItem = scheduleItems[sortId[minIndex]];

      if (curItem->getStartDate() < minItem->getStartDate())
      {
        minIndex = j;
      }
      else if (curItem->getStartDate() == minItem->getStartDate())
      {
        if (curItem->getStartTime() < minItem->getStartTime())
        {
          minIndex = j;
        }
      }
    }
    if (minIndex != i)
    {
      int tmp = sortId[i];
      sortId[i] = sortId[minIndex];
      sortId[minIndex] = tmp;
    }
  }
  for (int k = 0; k < count; k++)
  {
    int idx = sortId[k];
    scheduleItems[idx]->displayAllSchedules();
    cout << endl;
  }
  count = 0;
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
      continue;
    }
    if (scheduleItems[i]->getId() == id)
    {
      continue;
    }
    else if ((scheduleItems[i]->getStartDate() + " " + scheduleItems[i]->getStartTime()) < (endDate + " " + endTime) &&
             (startDate + " " + startTime) < (scheduleItems[i]->getEndDate() + " " + scheduleItems[i]->getEndTime()))
    {
      cout << "충돌이 났습니다. " << endl;
      scheduleItems[i]->displayAllSchedules();
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
  checkId(id);
  if (scheduleItems[updateId] != nullptr && scheduleItems[updateId]->getId() == id)
  {
    ReminderSchedule *RS = dynamic_cast<ReminderSchedule *>(scheduleItems[updateId]);
    if (RS != nullptr && !RS->getIsReminderSent())
    {
      cout << RS->getReminderMessage() << endl;

      RS->setIsReminderSent(true);
      return;
    }
    cout << "이 일정은 알림 대상이 아닙니다.\n";
  }
}

void ScheduleManager::runNotification(int id)
{
  checkId(id);
  if (scheduleItems[updateId] != nullptr && scheduleItems[updateId]->getId() == id)
  {
    notifyUser(id);
  }
}

void ScheduleManager::exit()
{
  delete[] scheduleItems;
}