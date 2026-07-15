#pragma once

#include "User.h"

class UserManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  UserManager();

  void addUser();

  void displayAllUser();

  void displayUserById(int id);

  void updateUser(int id);

  void deleteUser(int id);

  bool checkUserIdAndSchedule(int id);

  // user.txt에 해당 id가 존재하는지 확인
  bool userExists(int id);

  void exit();

private:
  // unique_ptr => 스마트 포인터, 원소나 ScheduleManager 삭제시 객체 자동 해제
  using ItemPtr = std::unique_ptr<User>;
  std::vector<ItemPtr> user;
  string name, email;
  int idCounter = 0;

  std::vector<ItemPtr>::iterator findById(int id);

  bool validateUserInput(int id, const string &name, const string &email);

  bool checkConflict(int id, string email);

  void saveToFile() const;

  void loadFromFile();
};
