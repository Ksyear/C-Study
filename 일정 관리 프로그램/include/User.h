#pragma once

#include "h.h"

using namespace std::chrono;
using namespace std;

class User
{
public:
  User(int id, string name, string email, system_clock::time_point now);

  virtual ~User() = default;

  virtual void displayAllUsers();

  virtual void setInfo(string name, string email, system_clock::time_point now);

  int getId();

  string getName();

  string getEmail();

  string getCreatedAt();

  string getUpdatedAt();

  void setCreatedAt(const string &createdAt);

  void setUpdatedAt(const string &updatedAt);

  string serializeCommon() const;

private:
  int id = 0;
  string name;
  string email;
  string createdAt, updatedAt;
};
