#include "../include/UserManager.h"
#include "../include/ScheduleManager.h"

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

UserManager::UserManager()
{
  loadFromFile();
}

// 조회 결과를 반환값(iterator)으로만 돌려주므로 멤버를 건드리지 않음
std::vector<UserManager::ItemPtr>::iterator UserManager::findById(int id)
{
  // std::find_if(탐색 시작 위치, 탐색 끝 위치, 람다(조건 함수){이 조건을 만족하면 찾은 것});
  return std::find_if(user.begin(), user.end(),
                      [id](const ItemPtr &item)
                      {
                        return item->getId() == id;
                      });
}

bool UserManager::validateUserInput(int id, const string &name, const string &email)
{
  if (user.size() >= 100)
  {
    throw std::runtime_error("일정이 가득 찼습니다. 더 이상 추가할 수 없습니다.");
  }

  if (name.empty() || email.empty() ||
      // email 형식 검증: "문자열@문자열.문자열" 형식인지 확인, 대소문자 구분 없이 검사
      !std::regex_match(email, std::regex(R"((\w+)(\.{1}\w+)*@(\w+)(\.(\w+))+)", std::regex_constants::icase)))
  {
    // invalid_argument => 형식은 맞는데 값이 논리적으로 잘못된 경우
    throw std::invalid_argument("값을 입력하시오");
  }
  if (checkConflict(id, email))
  {
    throw std::runtime_error("이메일이 중복되었습니다.");
  }
  return true;
}

bool UserManager::userExists(int id)
{
  // findById가 user.end()가 아니면 명부에 존재한다는 의미
  return findById(id) != user.end();
}

bool UserManager::checkConflict(int id, string email)
{
  for (const auto &item : user)
  {
    if (item->getId() != id && item->getEmail() == email)
    {
      return true;
    }
  }
  return false;
}

bool UserManager::checkUserIdAndSchedule(int id)
{
  ScheduleManager scheduleManager;
  if (scheduleManager.checkUserIdConflict(id))
  {
    return true;
  }
  auto it = findById(id);
  if (it != user.end())
  {
    if (auto *MS = dynamic_cast<MeetingSchedule *>(it->get()))
    {
      // any_of => 조건을 만족하는 원소가 하나라도 있으면 true 반환
      if (MS->getHost() == id || std::any_of(MS->getParticipants().begin(), MS->getParticipants().end(),
                                             [id](int participant)
                                             {
                                               return participant == id;
                                             }))
      {
        return true;
      }
    }
  }
  return false;
}

void UserManager::saveToFile() const
{
  // std::ofstream => 파일에 데이터를 쓰기(출력) 위해 사용하는 스트림 클래스
  std::ofstream out("user.txt");
  if (!out)
  {
    throw std::runtime_error("일정 파일을 저장할 수 없습니다.");
  }
  for (const auto &item : user)
  {
    out << item->serializeCommon() << '\n';
  }
}

void UserManager::loadFromFile()
{
  std::ifstream in("user.txt");
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

    ItemPtr item;
    item = std::make_unique<User>(std::stoi(fields.at(0)), fields.at(1), fields.at(2), system_clock::now());

    item->setCreatedAt(fields.at(3));
    item->setUpdatedAt(fields.at(4));

    idCounter = std::max(idCounter, std::stoi(fields.at(0)));
    user.push_back(std::move(item));
  }
}

void UserManager::exit()
{
  saveToFile();
}

void UserManager::addUser()
{
  try
  {
    cout << "사용자의 정보를 입력하시오" << endl;
    cout << "이름: ";
    cin >> name;
    cout << "이메일: ";
    cin >> email;
    validateUserInput(idCounter + 1, name, email);
    system_clock::time_point now = system_clock::now();
    // 메모리 누수 없이 안전하게 동적 객체를 생성하고, std::vector 같은 컨테이너 끝에 보관
    user.push_back(std::make_unique<User>(++idCounter, name, email, now));
    saveToFile();
    cout << "사용자가 추가되었습니다." << endl;
  }
  catch (const std::exception &e)
  {
    cerr << "사용자 추가 중 오류 발생 " << endl;
  }
}

void UserManager::displayAllUser()
{
  if (user.empty())
  {
    cerr << "사용자가 없습니다." << endl;
    return;
  }
  for (const auto &item : user)
  {
    item->displayAllUsers();
    cout << endl;
  }
}

void UserManager::displayUserById(int id)
{
  auto it = findById(id);
  if (it == user.end())
  {
    cout << "사용자를 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  (*it)->displayAllUsers();
}

void UserManager::updateUser(int id)
{
  auto it = findById(id);
  if (it == user.end())
  {
    cout << "사용자를 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  cout << "사용자의 정보를 입력하시오" << endl;
  cout << "이름: ";
  cin >> name;
  cout << "이메일: ";
  cin >> email;
  validateUserInput(id, name, email);
  system_clock::time_point now = system_clock::now();
  (*it)->setInfo(name, email, now);
  saveToFile();
  cout << "사용자 정보가 수정되었습니다." << endl;
}

void UserManager::deleteUser(int id)
{
  auto it = findById(id);
  if (it == user.end())
  {
    cout << "사용자를 찾을 수 없습니다: ID " << id << endl;
    return;
  }
  // unique_ptr가 객체를 자동 해제하므로 delete 불필요
  if (checkUserIdAndSchedule(id))
  {
    cout << "해당 사용자와 관련된 일정이 존재합니다. 사용자를 삭제할 수 없습니다." << endl;
    return;
  }
  user.erase(it);
  saveToFile();
  cout << "사용자가 삭제되었습니다: ID " << id << endl;
}