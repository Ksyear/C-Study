#include "MeetingSchedule.h"

#include <iostream>

MeetingSchedule::MeetingSchedule(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host)
    : ScheduleItem(id, title, description, startDate, endDate, startTime, endTime, priority, now)
{
  this->location = location;
  this->participantns = participantns;
  this->agenda = agenda;
  this->host = host;
}

void MeetingSchedule::displayAllSchedules()
{
  ScheduleItem::displayAllSchedules();
  cout << ", Location: " << location
       << ", Participants: " << participantns
       << ", Agenda: " << agenda
       << ", Host: " << host;
}

void MeetingSchedule::setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host)
{
  ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
  this->location = location;
  this->participantns = participantns;
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
      << location << '|'
      << participantns << '|'
      << agenda << '|'
      << host;
  return oss.str();
}

string MeetingSchedule::getLocation()
{
  return location;
}

string MeetingSchedule::getParticipantns()
{
  return participantns;
}

string MeetingSchedule::getAgenda()
{
  return agenda;
}

string MeetingSchedule::getHost()
{
  return host;
}
