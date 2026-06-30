#pragma once

#include "ScheduleItem.h"

class MeetingSchedule : public ScheduleItem
{
public:
  MeetingSchedule(int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host);

  void displayAllSchedules() override;

  void setInfo(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host);

  void markAsCompleted() override;

  string getType() const override;

  string serialize() const override;

  string getLocation();

  string getParticipantns();

  string getAgenda();

  string getHost();

private:
  string location, participantns, agenda, host;
  system_clock::time_point now = system_clock::now();
};
