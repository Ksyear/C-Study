#include "ScheduleManager.h"
#include "UserManager.h"

namespace
{
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

string ScheduleManager::inputString()
{
  while (true)
  {
    cin >> String;
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
  return String;
}

ScheduleManager::ScheduleManager() : search(scheduleItems), sort(scheduleItems), addUpdate(scheduleItems)
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

bool ScheduleManager::checkUserIdConflict(int userId)
{
  UserManager userManager;
  return userManager.userExists(userId);
}

void ScheduleManager::validateScheduleInput(int userId, int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority)
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

  if (userId <= 0 || id <= 0)
  {
    throw std::invalid_argument("잘못된 사용자 ID 또는 일정 ID입니다.");
  }

  if (!checkUserIdConflict(userId))
  {
    throw std::runtime_error("존재하지 않는 사용자 ID입니다.");
  }

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

  if (checkConflict(userId, id, startDate, endDate, startTime, endTime))
  {
    throw std::runtime_error("일정 충돌이 발생했습니다.");
  }
}

void ScheduleManager::addSchedule()
{
  addUpdate.addSchedule();
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
  UserManager userManager;
  int userId = (*it)->getUserId();
  userManager.displayUserById(userId);
  cout << endl;
}

void ScheduleManager::updateSchedule(int id)
{
  addUpdate.updateSchedule(id);
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

void ScheduleManager::searchByScheduleByUserId(int userId)
{
  search.searchByScheduleByUserId(userId);
}

void ScheduleManager::searchByUserAndTitle(int userId, const string &title)
{
  search.searchByUserAndTitle(userId, title);
}

void ScheduleManager::searchByUserAndDate(int userId, const string &startDate)
{
  search.searchByUserAndDate(userId, startDate);
}

void ScheduleManager::searchByUserAndPriority(int userId, const string &priority)
{
  search.searchByUserAndPriority(userId, priority);
}

void ScheduleManager::searchByTitle(string title)
{
  search.searchByTitle(title);
}

void ScheduleManager::searchByDate(string startDate)
{
  search.searchByDate(startDate);
}

void ScheduleManager::searchByPriority(string priority)
{
  search.searchByPriority(priority);
}

void ScheduleManager::sortByUserAndDate(int userId)
{
  sort.sortByUserAndDate(userId);
}

void ScheduleManager::sortByUserAndPriority(int userId)
{
  sort.sortByUserAndPriority(userId);
}

void ScheduleManager::sortByUserAndCompleted(int userId)
{
  sort.sortByUserAndCompleted(userId);
}

void ScheduleManager::sortByDate()
{
  sort.sortByDate();
  displayAllSchedules();
}

void ScheduleManager::sortByPriority()
{
  sort.sortByPriority();
  displayAllSchedules();
}

void ScheduleManager::sortByCompleted()
{
  sort.sortByCompleted();
  displayAllSchedules();
}

bool ScheduleManager::checkConflict(int userId, int id, const string &startDate, const string &endDate, const string &startTime, const string &endTime)
{
  for (const auto &item : scheduleItems)
  {
    if (item->getUserId() == userId && item->getId() != id && (item->getStartDate() + " " + item->getStartTime()) < (endDate + " " + endTime) &&
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
    int userId = std::stoi(fields.at(1));
    int id = std::stoi(fields.at(2));
    std::string title = fields.at(3);
    std::string description = fields.at(4);
    std::string startDate = fields.at(5);
    std::string endDate = fields.at(6);
    std::string startTime = fields.at(7);
    std::string endTime = fields.at(8);
    std::string priority = fields.at(9);
    bool isCompleted = std::stoi(fields.at(10)) != 0;
    std::string createdAt = fields.at(11);
    std::string updatedAt = fields.at(12);
    system_clock::time_point now = system_clock::now();

    ItemPtr item;
    if (type == "GENERAL")
    {
      item = std::make_unique<GeneralSchedule>(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(13), fields.at(14), fields.at(15));
    }
    else if (type == "MEETING")
    {
      std::vector<int> participants;
      for (const auto &p : split(fields.at(14), ','))
      {
        participants.push_back(std::stoi(p));
      }
      item = std::make_unique<MeetingSchedule>(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(13), participants, fields.at(15), std::stoi(fields.at(16)));
    }
    else if (type == "TASK")
    {
      item = std::make_unique<TaskSchedule>(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(13), std::stoi(fields.at(14)), fields.at(15), fields.at(16));
    }
    else if (type == "REMINDER")
    {
      item = std::make_unique<ReminderSchedule>(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now, fields.at(13), fields.at(14), fields.at(15));
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
      RS->setIsReminderSent(std::stoi(fields.at(16)) != 0);
    }
    addUpdate.setIdCounter(std::max(0, id));
    scheduleItems.push_back(std::move(item));
  }
}

void ScheduleManager::exit()
{
  saveToFile();
}
