#pragma once

#include "ScheduleItem.cpp"

class MeetingSchedule : public ScheduleItem
{
public:
  MeetingSchedule(int idCounter, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host)
      : ScheduleItem(idCounter, title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->location = location;
    this->participantns = participantns;
    this->agenda = agenda;
    this->host = host;
  }

  void displayAllSchedules() override
  {
    ScheduleItem::displayAllSchedules();
    cout << ", Location: " << location
         << ", Participants: " << participantns
         << ", Agenda: " << agenda
         << ", Host: " << host;
  }

  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host)
  {
    ScheduleItem::setInfo(title, description, startDate, endDate, startTime, endTime, priority, now);
    this->location = location;
    this->participantns = participantns;
    this->agenda = agenda;
    this->host = host;
  }

  void markAsCompleted() override
  {
    ScheduleItem::markAsCompleted();
  }

  string getLocation()
  {
    return location;
  }

  string getParticipantns()
  {
    return participantns;
  }

  string getAgenda()
  {
    return agenda;
  }

  string getHost()
  {
    return host;
  }

private:
  string location, participantns, agenda, host;
  system_clock::time_point now = system_clock::now();
};
