#include "../include/User.h"

User::User(int id, string name, string email, system_clock::time_point now)
{
  this->id = id;
  this->name = name;
  this->email = email;
  zoned_time zt{locate_zone("Asia/Seoul"), now};
  this->createdAt = std::format("{:%Y-%m-%d %H:%M}", zt);
  this->updatedAt = std::format("{:%Y-%m-%d %H:%M}", zt);
}

void User::displayAllUsers()
{
  cout << "ID: " << id
       << ", Name: " << name
       << ", Email: " << email
       << ", Created At: " << createdAt  // std::ctime(&createdAt)
       << ", Updated At: " << updatedAt; // std::ctime(&updatedAt);
}

void User::setInfo(string name, string email, system_clock::time_point now)
{
  this->name = name;
  this->email = email;
  zoned_time zt{locate_zone("Asia/Seoul"), now};
  this->updatedAt = std::format("{:%Y-%m-%d %H:%M}", zt);
}

int User::getId()
{
  return id;
}

string User::getName()
{
  return name;
}

string User::getEmail()
{
  return email;
}

string User::getCreatedAt()
{
  return createdAt;
}

string User::getUpdatedAt()
{
  return updatedAt;
}

void User::setCreatedAt(const string &createdAt)
{
  this->createdAt = createdAt;
}

void User::setUpdatedAt(const string &updatedAt)
{
  this->updatedAt = updatedAt;
}

string User::serializeCommon() const
{
  std::ostringstream oss;
  oss << id << '|'
      << name << '|'
      << email << '|'
      << createdAt << '|'
      << updatedAt;
  return oss.str();
}
