#include "ScheduleManager.h"

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

  // "어떤값"을 기준으로 txt안의 값들을 구분하기 위해 사용
  std::vector<std::string> split(const std::string &line, char delimiter)
  {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimiter))
    {
      result.push_back(token);
    }
    return result;
  }
}

ScheduleManager::ScheduleManager()
{
  loadFromFile();
}

// 조회 결과를 반환값(iterator)으로만 돌려주므로 멤버를 건드리지 않음
std::vector<ScheduleManager::ItemPtr>::iterator ScheduleManager::findById(int id)
{
  // std::find_if(탐색 시작 위치, 탐색 끝 위치, 람다(조건 함수){이 조건을 만족하면 찾은 것});
  return std::find_if(scheduleItems.begin(), scheduleItems.end(),
                      [id](const ItemPtr &item)
                      {
                        return item->getId() == id;
                      });
}

void ScheduleManager::validateScheduleInput(int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority)
{
  if (scheduleItems.size() >= 100)
  {
    throw std::runtime_error("일정이 가득 찼습니다. 더 이상 추가할 수 없습니다.");
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
      startDate.substr(4, 1) != "-" || startDate.substr(7, 1) != "-" ||
      endDate.substr(4, 1) != "-" || endDate.substr(7, 1) != "-" ||
      startTime.substr(2, 1) != ":" || endTime.substr(2, 1) != ":" ||
      !testStartDate || !testEndDate || !testStartTime || !testEndTime ||
      (sdStart > sdEnd) || (sdStart == sdEnd && st >= et) ||
      (priority != "LOW" && priority != "MEDIUM" && priority != "HIGH"))
  {
    // invalid_argument => 형식은 맞는데 값이 논리적으로 잘못된 경우
    throw std::invalid_argument("잘못된 날짜/시간/우선순위 형식입니다.");
  }

  if (checkConflict(id, startDate, endDate, startTime, endTime))
  {
    throw std::runtime_error("일정 충돌이 발생했습니다.");
  }
}

int ScheduleManager::inputInt()
{
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
    cout << "값을 입력하시오" << endl;
    cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> category >> place >> memo;
    validateScheduleInput(idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    // 메모리 누수 없이 안전하게 동적 객체를 생성하고, std::vector 같은 컨테이너 끝에 보관
    scheduleItems.push_back(std::make_unique<GeneralSchedule>(++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, category, place, memo));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 2:
  {
    cout << "값을 입력하시오" << endl;
    cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> location >> participantns >> agenda >> host;
    validateScheduleInput(idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    scheduleItems.push_back(std::make_unique<MeetingSchedule>(++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, location, participantns, agenda, host));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 3:
  {
    cout << "값을 입력하시오" << endl;
    cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> deadline >> progress >> taskStatus >> assignedTo;
    validateScheduleInput(idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    scheduleItems.push_back(std::make_unique<TaskSchedule>(++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, deadline, progress, taskStatus, assignedTo));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 4:
  {
    cout << "값을 입력하시오" << endl;
    cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> reminderTime >> reminderMessage >> notificationType;
    validateScheduleInput(idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    scheduleItems.push_back(std::make_unique<ReminderSchedule>(++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, reminderTime, reminderMessage, notificationType));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  default:
  {
    throw std::invalid_argument("잘못된 입력입니다.");
  }
  }
}

void ScheduleManager::displayAllSchedules()
{
  if (scheduleItems.empty())
  {
    cerr << "일정이 없습니다." << endl;
    return;
  }
  for (const auto &item : scheduleItems)
  {
    item->displayAllSchedules();
    cout << endl;
  }
}

void ScheduleManager::displaySchedule(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  (*it)->displayAllSchedules();
  cout << endl;
}

void ScheduleManager::updateSchedule(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  cout << "수정할 값을 입력하시오: title, description, startDate, endDate, startTime, endTime, priority" << endl;
  cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority;
  validateScheduleInput((*it)->getId(), title, description, startDate, endDate, startTime, endTime, priority);
  // dynamic_cast => 다운캐스팅
  if (auto *GS = dynamic_cast<GeneralSchedule *>(it->get()))
  {
    cout << "값을 입력하시오: category, place, memo" << endl;
    cin >> category >> place >> memo;
    GS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), category, place, memo);
  }
  else if (auto *MS = dynamic_cast<MeetingSchedule *>(it->get()))
  {
    cout << "값을 입력하시오: location, participants, agenda, host" << endl;
    cin >> location >> participantns >> agenda >> host;
    MS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), location, participantns, agenda, host);
  }
  else if (auto *TS = dynamic_cast<TaskSchedule *>(it->get()))
  {
    cout << "값을 입력하시오: deadline, progress, taskStatus, assignedTo" << endl;
    cin >> deadline >> progress >> taskStatus >> assignedTo;
    TS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), deadline, progress, taskStatus, assignedTo);
  }
  else if (auto *RS = dynamic_cast<ReminderSchedule *>(it->get()))
  {
    cout << "값을 입력하시오: reminderTime, reminderMessage, notificationType" << endl;
    cin >> reminderTime >> reminderMessage >> notificationType;
    RS->setInfo(title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), reminderTime, reminderMessage, notificationType);
  }
  saveToFile();
  cout << "수정되었습니다.\n";
}

void ScheduleManager::deleteSchedule(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cout << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  // unique_ptr가 객체를 자동 해제하므로 delete 불필요
  scheduleItems.erase(it);
  saveToFile();
  cout << "일정이 삭제되었습니다: ID " << id << endl;
}

void ScheduleManager::completeSchedule(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  (*it)->markAsCompleted();
  saveToFile();
  cout << "일정이 완료되었습니다." << endl;
}

void ScheduleManager::searchByTitle(string title)
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

void ScheduleManager::searchByDate(string startDate)
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

void ScheduleManager::searchByPriority(string priority)
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

void ScheduleManager::sortByDate()
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
  displayAllSchedules();
}

void ScheduleManager::sortByPriority()
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
  displayAllSchedules();
}

void ScheduleManager::sortByCompleted()
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
  displayAllSchedules();
}

bool ScheduleManager::checkConflict(int id, string startDate, string endDate, string startTime, string endTime)
{
  for (const auto &item : scheduleItems)
  {
    if (item->getId() == id)
    {
      continue;
    }
    if ((item->getStartDate() + " " + item->getStartTime()) < (endDate + " " + endTime) &&
        (startDate + " " + startTime) < (item->getEndDate() + " " + item->getEndTime()))
    {
      item->displayAllSchedules();
      cout << endl;
      return true;
    }
  }
  return false;
}

void ScheduleManager::notifyUser(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  if (auto *RS = dynamic_cast<ReminderSchedule *>(it->get()))
  {
    if (!RS->getIsReminderSent())
    {
      cout << RS->getReminderMessage() << endl;
      RS->setIsReminderSent(true);
      saveToFile();
      return;
    }
  }
  cout << "이 일정은 알림 대상이 아닙니다.\n";
}

void ScheduleManager::runNotification(int id)
{
  notifyUser(id);
}

void ScheduleManager::saveToFile() const
{
  // std::ofstream => 파일에 데이터를 쓰기(출력) 위해 사용하는 스트림 클래스
  std::ofstream out("schedules.txt");
  if (!out)
  {
    throw std::runtime_error("일정 파일을 저장할 수 없습니다.");
  }
  for (const auto &item : scheduleItems)
  {
    out << item->serialize() << '\n';
  }
}

void ScheduleManager::loadFromFile()
{
  std::ifstream in("schedules.txt");
  if (!in)
  {
    return;
  }
  std::string line;
  while (std::getline(in, line))
  {
    if (line.empty())
    {
      continue;
    }
    auto fields = split(line, '|');
    const std::string &type = fields.at(0);
    int id = std::stoi(fields.at(1));
    std::string title = fields.at(2);
    std::string description = fields.at(3);
    std::string startDate = fields.at(4);
    std::string endDate = fields.at(5);
    std::string startTime = fields.at(6);
    std::string endTime = fields.at(7);
    std::string priority = fields.at(8);
    bool isCompleted = std::stoi(fields.at(9)) != 0;
    std::string createdAt = fields.at(10);
    std::string updatedAt = fields.at(11);
    system_clock::time_point now = system_clock::now();

    ItemPtr item;
    if (type == "GENERAL")
    {
      item = std::make_unique<GeneralSchedule>(id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(12), fields.at(13), fields.at(14));
    }
    else if (type == "MEETING")
    {
      item = std::make_unique<MeetingSchedule>(id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(12), fields.at(13), fields.at(14), fields.at(15));
    }
    else if (type == "TASK")
    {
      item = std::make_unique<TaskSchedule>(id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(12), std::stoi(fields.at(13)), fields.at(14), fields.at(15));
    }
    else if (type == "REMINDER")
    {
      item = std::make_unique<ReminderSchedule>(id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(12), fields.at(13), fields.at(14));
    }
    else
    {
      throw std::runtime_error("알 수 없는 일정 타입입니다: " + type);
    }

    item->setIsCompleted(isCompleted);
    item->setCreatedAt(createdAt);
    item->setUpdatedAt(updatedAt);
    if (auto *RS = dynamic_cast<ReminderSchedule *>(item.get()))
    {
      RS->setIsReminderSent(std::stoi(fields.at(15)) != 0);
    }
    idCounter = std::max(idCounter, id);
    scheduleItems.push_back(std::move(item));
  }
}

void ScheduleManager::exit()
{
  saveToFile();
}
