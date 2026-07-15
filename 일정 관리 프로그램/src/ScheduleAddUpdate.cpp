#include "../include/ScheduleAddUpdate.h"
#include "../include/UserManager.h"

ScheduleAddUpdate::ScheduleAddUpdate(std::vector<ItemPtr> &items) : scheduleItems(items)
{
  // 기존 항목이 있으면 그 최대 id로 idCounter를 시드해 새 id가 겹치지 않게 한다.
  for (const auto &item : scheduleItems)
  {
    idCounter = std::max(idCounter, item->getId());
  }
}

void ScheduleAddUpdate::addSchedule()
{
  cout << "일정 유형을 선택하시오: \n1. 일반 일정\n2. 회의 일정\n3. 작업 일정\n4. 리마인더 일정" << endl;
  choice = inputInt();
  switch (choice)
  {
  case 1:
  {
    cout << "값을 입력하시오" << endl;
    cout << "사용자 ID: ";
    cin >> userId;
    cout << "제목: ";
    cin >> title;
    cout << "설명: ";
    cin >> description;
    cout << "시작 날짜 (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "종료 날짜 (YYYY-MM-DD): ";
    cin >> endDate;
    cout << "시작 시간 (HH:MM): ";
    cin >> startTime;
    cout << "종료 시간 (HH:MM): ";
    cin >> endTime;
    cout << "우선순위 (LOW/MEDIUM/HIGH): ";
    cin >> priority;
    cout << "카테고리: ";
    cin >> category;
    cout << "장소: ";
    cin >> place;
    cout << "메모: ";
    cin >> memo;
    validateScheduleInput(userId, idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    // 메모리 누수 없이 안전하게 동적 객체를 생성하고, std::vector 같은 컨테이너 끝에 보관
    scheduleItems.push_back(std::make_unique<GeneralSchedule>(userId, ++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, category, place, memo));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 2:
  {
    cout << "값을 입력하시오" << endl;
    cout << "사용자 ID: ";
    cin >> userId;
    cout << "제목: ";
    cin >> title;
    cout << "설명: ";
    cin >> description;
    cout << "시작 날짜 (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "종료 날짜 (YYYY-MM-DD): ";
    cin >> endDate;
    cout << "시작 시간 (HH:MM): ";
    cin >> startTime;
    cout << "종료 시간 (HH:MM): ";
    cin >> endTime;
    cout << "우선순위 (LOW/MEDIUM/HIGH): ";
    cin >> priority;
    cout << "장소: ";
    cin >> location;
    cout << "참석자 IDs (쉼표로 구분): ";
    cin >> stringParticipants;
    cout << "의제: ";
    cin >> agenda;
    cout << "주최자 ID: ";
    cin >> host;
    validateScheduleInput(userId, idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    // stringParticipants를 int 벡터로 변환
    std::vector<int> participantIds;
    std::stringstream ss(stringParticipants);
    std::string id;
    while (std::getline(ss, id, ','))
    {
      participantIds.push_back(std::stoi(id));
    }
    participants = participantIds;
    scheduleItems.push_back(std::make_unique<MeetingSchedule>(userId, ++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, location, participants, agenda, host));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 3:
  {
    cout << "값을 입력하시오" << endl;
    cout << "사용자 ID: ";
    cin >> userId;
    cout << "제목: ";
    cin >> title;
    cout << "설명: ";
    cin >> description;
    cout << "시작 날짜 (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "종료 날짜 (YYYY-MM-DD): ";
    cin >> endDate;
    cout << "시작 시간 (HH:MM): ";
    cin >> startTime;
    cout << "종료 시간 (HH:MM): ";
    cin >> endTime;
    cout << "우선순위 (LOW/MEDIUM/HIGH): ";
    cin >> priority;
    cout << "마감일 (YYYY-MM-DD): ";
    cin >> deadline;
    cout << "진행 상황: ";
    cin >> progress;
    cout << "작업 상태: ";
    cin >> taskStatus;
    cout << "담당자 ID: ";
    cin >> assignedTo;
    validateScheduleInput(userId, idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    scheduleItems.push_back(std::make_unique<TaskSchedule>(userId, ++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, deadline, progress, taskStatus, assignedTo));
    saveToFile();
    cout << "일정이 추가되었습니다." << endl;
    break;
  }
  case 4:
  {
    cout << "값을 입력하시오" << endl;
    cout << "사용자 ID: ";
    cin >> userId;
    cout << "제목: ";
    cin >> title;
    cout << "설명: ";
    cin >> description;
    cout << "시작 날짜 (YYYY-MM-DD): ";
    cin >> startDate;
    cout << "종료 날짜 (YYYY-MM-DD): ";
    cin >> endDate;
    cout << "시작 시간 (HH:MM): ";
    cin >> startTime;
    cout << "종료 시간 (HH:MM): ";
    cin >> endTime;
    cout << "우선순위 (LOW/MEDIUM/HIGH): ";
    cin >> priority;
    cout << "리마인더 시간: ";
    cin >> reminderTime;
    cout << "리마인더 메시지: ";
    cin >> reminderMessage;
    cout << "알림 유형: ";
    cin >> notificationType;
    validateScheduleInput(userId, idCounter + 1, title, description, startDate, endDate, startTime, endTime, priority);
    system_clock::time_point now = system_clock::now();
    scheduleItems.push_back(std::make_unique<ReminderSchedule>(userId, ++idCounter, title, description, startDate, endDate, startTime, endTime, priority, now, reminderTime, reminderMessage, notificationType));
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

void ScheduleAddUpdate::updateSchedule(int id)
{
  auto it = findById(id);
  if (it == scheduleItems.end())
  {
    cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  cout << "수정할 값을 입력하시오" << endl;
  cout << "사용자 ID: ";
  cin >> userId;
  cout << "제목: ";
  cin >> title;
  cout << "설명: ";
  cin >> description;
  cout << "시작 날짜 (YYYY-MM-DD): ";
  cin >> startDate;
  cout << "종료 날짜 (YYYY-MM-DD): ";
  cin >> endDate;
  cout << "시작 시간 (HH:MM): ";
  cin >> startTime;
  cout << "종료 시간 (HH:MM): ";
  cin >> endTime;
  cout << "우선순위 (LOW/MEDIUM/HIGH): ";
  cin >> priority;
  validateScheduleInput(userId, (*it)->getId(), title, description, startDate, endDate, startTime, endTime, priority);
  // dynamic_cast => 다운캐스팅
  if (auto *GS = dynamic_cast<GeneralSchedule *>(it->get()))
  {
    cout << "카테고리: ";
    cin >> category;
    cout << "장소: ";
    cin >> place;
    cout << "메모: ";
    cin >> memo;
    GS->setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), category, place, memo);
  }
  else if (auto *MS = dynamic_cast<MeetingSchedule *>(it->get()))
  {
    cout << "장소: ";
    cin >> location;
    cout << "참석자 IDs (쉼표로 구분): ";
    cin >> stringParticipants;
    cout << "의제: ";
    cin >> agenda;
    cout << "주최자 ID: ";
    cin >> host;
    // stringParticipants를 int 벡터로 변환
    std::vector<int> participantIds;
    std::stringstream ss(stringParticipants);
    std::string id;
    while (std::getline(ss, id, ','))
    {
      participantIds.push_back(std::stoi(id));
    }
    participants = participantIds;

    MS->setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), location, participants, agenda, host);
  }
  else if (auto *TS = dynamic_cast<TaskSchedule *>(it->get()))
  {
    cout << "마감일 (YYYY-MM-DD): ";
    cin >> deadline;
    cout << "진행 상황: ";
    cin >> progress;
    cout << "작업 상태: ";
    cin >> taskStatus;
    cout << "담당자 ID: ";
    cin >> assignedTo;
    TS->setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), deadline, progress, taskStatus, assignedTo);
  }
  else if (auto *RS = dynamic_cast<ReminderSchedule *>(it->get()))
  {
    cout << "리마인더 시간: ";
    cin >> reminderTime;
    cout << "리마인더 메시지: ";
    cin >> reminderMessage;
    cout << "알림 유형: ";
    cin >> notificationType;
    RS->setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, system_clock::now(), reminderTime, reminderMessage, notificationType);
  }
  saveToFile();
  cout << "수정되었습니다.\n";
}

int ScheduleAddUpdate::inputInt()
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

// 조회 결과를 반환값(iterator)으로만 돌려주므로 멤버를 건드리지 않음
std::vector<ScheduleAddUpdate::ItemPtr>::iterator ScheduleAddUpdate::findById(int id)
{
  return std::find_if(scheduleItems.begin(), scheduleItems.end(),
                      [id](const ItemPtr &item)
                      {
                        return item->getId() == id;
                      });
}

bool ScheduleAddUpdate::checkUserIdConflict(int userId)
{
  UserManager userManager;
  return userManager.userExists(userId);
}

void ScheduleAddUpdate::validateScheduleInput(int userId, int id, const string &title, const string &description, const string &startDate, const string &endDate, const string &startTime, const string &endTime, const string &priority)
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

bool ScheduleAddUpdate::checkConflict(int userId, int id, const string &startDate, const string &endDate, const string &startTime, const string &endTime)
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

void ScheduleAddUpdate::saveToFile() const
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

void ScheduleAddUpdate::setIdCounter(int idCounter){
  this->idCounter = idCounter;
}
