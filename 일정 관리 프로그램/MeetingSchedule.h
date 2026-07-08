#pragma once

#include "ScheduleItem.h"

class MeetingSchedule : public ScheduleItem
{
public:
  MeetingSchedule(int userId, int id, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, std::vector<int> participants, string agenda, int host);

  void displayAllSchedules() override;

  void setInfo(int userId, string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, std::vector<int> participants, string agenda, int host);

  void markAsCompleted() override;

  string getType() const override;

  string serialize() const override;

  string getLocation();

  std::vector<int> &getParticipants();

  string getAgenda();

  int getHost();

private:
  string location, agenda;
  int host;
  std::vector<int> participants;
  system_clock::time_point now = system_clock::now();
};
