#include "../include/MeetingSchedule.h"

#include <iostream>

MeetingSchedule::MeetingSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, std::vector<int> participants, string agenda, int host)
    : ScheduleItem(userId, id, title, description, startDate, endDate, startTime, endTime, priority, now)
{
  this->location = location;
  this->participants = std::move(participants);
  this->agenda = agenda;
  this->host = host;
}

void MeetingSchedule::displayAllSchedules()
{
  ScheduleItem::displayAllSchedules();
  cout << ", Location: " << location << ", Participants: ";
  for (size_t i = 0; i < participants.size(); ++i)
  {
    if (i)
      cout << ",";
    cout << participants[i];
  }
  cout << ", Agenda: " << agenda
       << ", Host: " << host;
}

void MeetingSchedule::setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, std::vector<int> participants, string agenda, int host)
{
  ScheduleItem::setInfo(userId, title, description, startDate, endDate, startTime, endTime, priority, now);
  this->location = location;
  this->participants = std::move(participants);
  this->agenda = agenda;
  this->host = host;
}

void MeetingSchedule::markAsCompleted()
{
  ScheduleItem::markAsCompleted();
}

string MeetingSchedule::getType() const
{
  return "MEETING";
}

string MeetingSchedule::serialize() const
{
  std::ostringstream oss;
  oss << serializeCommon() << '|'
      << location << '|';
  for (size_t i = 0; i < participants.size(); ++i)
  {
    if (i)
      oss << ',';
    oss << participants[i];
  }
  oss << '|'
      << agenda << '|'
      << host;
  return oss.str();
}

string MeetingSchedule::getLocation()
{
  return location;
}

std::vector<int> &MeetingSchedule::getParticipants()
{
  return participants;
}

string MeetingSchedule::getAgenda()
{
  return agenda;
}

int MeetingSchedule::getHost()
{
  return host;
}
