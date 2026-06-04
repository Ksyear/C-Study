#include <iostream>
#include <string>
#include <typeinfo>
// 현재 시간 가져오기
#include <chrono>
#include <ctime>

using namespace std::chrono;
using namespace std;

class ScheduleItem
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleItem(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now)
  {
    id++;
    this->title = title;
    this->description = description;
    this->startDate = startDate;
    this->endDate = endDate;
    this->startTime = startTime;
    this->endTime = endTime;
    this->priority = priority;
    this->isCompleted = false;
    this->createdAt = system_clock::to_time_t(now); // 자동
  }

  virtual void displayAllSchedules()
  {
    cout << "ID: " << id
         << ", Title: " << title
         << ", Description: " << description
         << ", Start Date: " << startDate
         << ", End Date: " << endDate
         << ", Start Time: " << startTime
         << ", End Time: " << endTime
         << ", Priority: " << priority
         << ", Created At: " << createdAt
         << ", Updated At: " << updatedAt;
  }

  virtual bool markAsCompleted()
  {
    return /*isCompleted = */ true;
  }

  int getId()
  {
    return id;
  }

  string getTitle()
  {
    return title;
  }

  string getDescription()
  {
    return description;
  }

  string getStartDate()
  {
    return startDate;
  }

  string getEndDate()
  {
    return endDate;
  }

  string getStartTime()
  {
    return startTime;
  }

  string getEndTime()
  {
    return endTime;
  }

  string getPriority()
  {
    return priority;
  }

  std::time_t getCreatedAt()
  {
    return createdAt;
  }

  std::time_t getUpdatedAt()
  {
    return updatedAt;
  }

  bool getIsCompleted(){
    return isCompleted;
  }

private:
  int id = 0;
  string priority, title, description;
  // YYYY-MM-DD, HH:MM
  string startDate, endDate, startTime, endTime;
  std::time_t createdAt, updatedAt;
  bool isCompleted, conflict;
};

class GeneralSchedule : public ScheduleItem
{
public:
  GeneralSchedule(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string category, string place, string memo)
      : ScheduleItem(title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->category = category;
    this->place = place;
    this->memo = memo;
  }

  void displayAllSchedules()
  {
    ScheduleItem::displayAllSchedules();
    cout << "Category: " << category
         << ", Place: " << place
         << ", Memo: " << memo;
  }

  bool markAsCompleted(string title)
  {
    ScheduleItem::markAsCompleted();
  }

  string getCategory()
  {
    return category;
  }

  string getPlace()
  {
    return place;
  }

  string getMemo()
  {
    return memo;
  }

private:
  string category, place, memo;
  system_clock::time_point now = system_clock::now();
};

class MeetingSchedule : public ScheduleItem
{
public:
  MeetingSchedule(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string location, string participantns, string agenda, string host) : ScheduleItem(title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    ScheduleItem::displayAllSchedules();
    this->location = location;
    this->participantns = participantns;
    this->agenda = agenda;
    this->host = host;
  }

  void displayAllSchedules()
  {
    ScheduleItem::displayAllSchedules();
    cout << "Location: " << location
         << ", Participants: " << participantns
         << ", Agenda: " << agenda
         << ", Host: " << host;
  }

  void markAsCompleted(string title)
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

class TaskSchedule : public ScheduleItem
{
public:
  TaskSchedule(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string deadline, string progress, string taskStatus, string assignedTo) : ScheduleItem(title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->deadline = deadline;
    this->progress = progress;
    this->taskStatus = taskStatus;
    this->assignedTo = assignedTo;
  }

  void displayAllSchedules()
  {
    ScheduleItem::displayAllSchedules();
    cout << "Location: " << deadline
         << ", Participants: " << progress
         << ", Agenda: " << taskStatus
         << ", Host: " << assignedTo;
  }

  void markAsCompleted(string title)
  {
    ScheduleItem::markAsCompleted();
  }

  string getDeadline()
  {
    return deadline;
  }

  string getProgress()
  {
    return progress;
  }

  string getTaskStatus()
  {
    return taskStatus;
  }

  string getAssignedTo()
  {
    return assignedTo;
  }

private:
  string deadline, progress, taskStatus, assignedTo;
  system_clock::time_point now = system_clock::now();
};

class ReminderSchedule : public ScheduleItem
{
public:
  ReminderSchedule(string title, string description, string startDate, string endDate, string startTime, string endTime, string priority, system_clock::time_point now, string reminderTime, string reminderMessage, string notificationType) : ScheduleItem(title, description, startDate, endDate, startTime, endTime, priority, now)
  {
    this->reminderTime = reminderTime;
    this->reminderMessage = reminderMessage;
    this->notificationType = notificationType;
    this->isReminderSent = false;
  }

  int markAsCompleted(string title)
  {
    ScheduleItem::markAsCompleted();
  }

  string getReminderTime()
  {
    return reminderTime;
  }

  string getReminderMessage()
  {
    return reminderMessage;
  }

  string getNotificationType()
  {
    return notificationType;
  }

private:
  string reminderTime, reminderMessage, notificationType;
  system_clock::time_point now = system_clock::now();
  bool isReminderSent;
}

class ScheduleManager
{
public: // 구현 단계에서 객체를 만들때 상속하면 여러개가 계속 만들어지니까 분해할 것
  ScheduleManager()
  {
  }

  void addSchedule()
  {
    string title, description, startDate, endDate, startTime, endTime, priority, category, place, memo, location, participantns, agenda, host, deadline, progress, taskStatus, assignedTo, reminderTime, reminderMessage, notificationType;

    int choice;
    cin >> choice;
    if (scheduleItems[99] != nullptr)
    {
      cerr << "일정이 가득 찼습니다. 더 이상 추가할 수 없습니다." << endl;
      return;
    }
    if (title.empty() || startDate.length() != 10 || endDate.length() != 10 || startTime.length() != 5 || endTime.length() != 5 ||
        // stoi(): 문자열을 숫자로 변환해주는 함수 라고 하네요
        stoi(startDate.substr(0, 3)) > stoi(endDate.substr(0, 3)) || stoi(startDate.substr(5, 6)) > stoi(endDate.substr(5, 6)) || stoi(startDate.substr(8, 9)) > stoi(endDate.substr(8, 9)) ||
        stoi(startTime.substr(0, 1)) > stoi(endTime.substr(0, 1)) || stoi(startTime.substr(3, 4)) > stoi(endTime.substr(3, 4)) ||
        stoi(startDate.substr(5, 6)) < 0 || stoi(startDate.substr(5, 6)) > 12 || stoi(startDate.substr(8, 9)) < 0 || stoi(startDate.substr(8, 9)) > 31 ||
        stoi(endDate.substr(0, 1)) < 0 || stoi(endDate.substr(0, 1)) > 12 || stoi(endDate.substr(3, 4)) < 0 || stoi(endDate.substr(3, 4)) > 31 ||
        stoi(startTime.substr(0, 1)) < 0 || stoi(startTime.substr(0, 1)) > 23 || stoi(startTime.substr(3, 4)) < 0 || stoi(startTime.substr(3, 4)) > 59 ||
        stoi(endTime.substr(0, 1)) < 0 || stoi(endTime.substr(0, 1)) > 23 || stoi(endTime.substr(3, 4)) < 0 || stoi(endTime.substr(3, 4)) > 59 ||
        startDate.substr(4) != "-" || startDate.substr(7) != "-" ||
        endDate.substr(4) != "-" || endDate.substr(7) != "-" ||
        startTime.substr(2) != ":" || endTime.substr(2) != ":" ||
        priority != "LOW" || priority != "MEDIUM" || priority != "HIGH")
    {
      cerr << "잘못된 날짜 형식입니다." << endl;
      return;
    }
    else if ((checkConglict(startDate, endDate, startTime, endTime)))
    {
      cerr << "Title: " << title
           << ", Description: " << description
           << ", Start Date: " << startDate
           << ", End Date: " << endDate
           << ", Start Time: " << startTime
           << ", End Time: " << endTime
           << ", Priority: " << priority
           << endl;
      return;
    }

    switch (choice)
    {
    case 1:
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          cout << "값을 입력하시오" << endl;
          cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> category >> place >> memo;
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new GeneralSchedule(title, description, startDate, endDate, startTime, endTime, priority, now, category, place, memo);
          cout << "일정이 추가되었습니다." << endl;
          break;
        }
      }
      break;
    case 2:
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          cout << "값을 입력하시오" << endl;
          cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> location >> participantns >> agenda >> host;
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new MeetingSchedule(title, description, startDate, endDate, startTime, endTime, priority, now, location, participantns, agenda, host);
          cout << "일정이 추가되었습니다." << endl;
          break;
        }
      }
    case 3:
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          cout << "값을 입력하시오" << endl;
          cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> deadline >> progress >> taskStatus >> assignedTo;
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new TaskSchedule(title, description, startDate, endDate, startTime, endTime, priority, now, deadline, progress, taskStatus, assignedTo);
          cout << "일정이 추가되었습니다." << endl;
          break;
        }
      }
    case 4:
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          cout << "값을 입력하시오" << endl;
          cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority >> reminderTime >> reminderMessage >> notificationType;
          ;
          system_clock::time_point now = system_clock::now();
          scheduleItems[i] = new ReminderSchedule(title, description, startDate, endDate, startTime, endTime, priority, now, reminderTime, reminderMessage, notificationType);
          cout << "일정이 추가되었습니다." << endl;
          break;
        }
      }
    default:
      cerr << "잘못된 입력입니다." << endl;
      break;
    }
  }

  void displayAllSchedules()
  {
    if (scheduleItems[0] == nullptr)
    {
      cerr << "일정이 없습니다." << endl;
      return;
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
        break;
      scheduleItems[i]->displayAllSchedules();
      cout << endl;
    }
  }

  void displaySchedule(int id)
  {
    if (id > 100 || id < 0 || scheduleItems[id] == nullptr)
    {
      cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
      return;
    }
    scheduleItems[id]->displayAllSchedules();
    cout << endl;
    return;
  }

  //   void updateSchedule(int id)
  //   {
  //     if (id > 100 || id < 0 || scheduleItems[id] == nullptr)
  //     {
  //       cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //       return;
  //     }
  //     cout << "수정할 값을 입력하시오: ";
  //     cin >> title >> description >> startDate >> endDate >> startTime >> endTime >> priority;
  //     if (title.empty() || startDate.length() != 10 || endDate.length() != 10 || startTime.length() != 5 || endTime.length() != 5 ||
  //         // stoi(): 문자열을 숫자로 변환해주는 함수 라고 하네요
  //         stoi(startDate.substr(0, 3)) > stoi(endDate.substr(0, 3)) || stoi(startDate.substr(5, 6)) > stoi(endDate.substr(5, 6)) || stoi(startDate.substr(8, 9)) > stoi(endDate.substr(8, 9)) ||
  //         stoi(startTime.substr(0, 1)) > stoi(endTime.substr(0, 1)) || stoi(startTime.substr(3, 4)) > stoi(endTime.substr(3, 4)) ||
  //         stoi(startDate.substr(5, 6)) < 0 || stoi(startDate.substr(5, 6)) > 12 || stoi(startDate.substr(8, 9)) < 0 || stoi(startDate.substr(8, 9)) > 31 ||
  //         stoi(endDate.substr(0, 1)) < 0 || stoi(endDate.substr(0, 1)) > 12 || stoi(endDate.substr(3, 4)) < 0 || stoi(endDate.substr(3, 4)) > 31 ||
  //         stoi(startTime.substr(0, 1)) < 0 || stoi(startTime.substr(0, 1)) > 23 || stoi(startTime.substr(3, 4)) < 0 || stoi(startTime.substr(3, 4)) > 59 ||
  //         stoi(endTime.substr(0, 1)) < 0 || stoi(endTime.substr(0, 1)) > 23 || stoi(endTime.substr(3, 4)) < 0 || stoi(endTime.substr(3, 4)) > 59 ||
  //         startDate.substr(4) != "-" || startDate.substr(7) != "-" ||
  //         endDate.substr(4) != "-" || endDate.substr(7) != "-" ||
  //         startTime.substr(2) != ":" || endTime.substr(2) != ":" ||
  //         priority != "LOW" || priority != "MEDIUM" || priority != "HIGH")
  //     {
  //       cerr << "잘못된 날짜 형식입니다." << endl;
  //       return;
  //     }
  //     else if ((checkConglict(startDate, endDate, startTime, endTime)))
  //     {
  //       // 현재 시간
  //       cerr << "Title: " << title
  //            << ", Description: " << description
  //            << ", Start Date: " << startDate
  //            << ", End Date: " << endDate
  //            << ", Start Time: " << startTime
  //            << ", End Time: " << endTime
  //            << ", Priority: " << priority
  //            << endl;
  //       return;
  //     }
  //     else
  //     {
  //       system_clock::time_point now = system_clock::now();
  //       scheduleItems[id]->title = title;
  //       scheduleItems[id]->description = description;
  //       scheduleItems[id]->startDate = startDate;
  //       scheduleItems[id]->endDate = endDate;
  //       scheduleItems[id]->startTime = startTime;
  //       scheduleItems[id]->endTime = endTime;
  //       scheduleItems[id]->priority = priority;
  //       scheduleItems[id]->updatedAt = system_clock::to_time_t(now);
  //       ;

  //       cout << "수정된 값 => ID: " << scheduleItems[id]->id
  //            << ", Title: " << scheduleItems[id]->title
  //            << ", Description: " << scheduleItems[id]->description
  //            << ", Start Date: " << scheduleItems[id]->startDate
  //            << ", End Date: " << scheduleItems[id]->endDate
  //            << ", Start Time: " << scheduleItems[id]->startTime
  //            << ", End Time: " << scheduleItems[id]->endTime
  //            << ", Priority: " << scheduleItems[id]->priority
  //            << ", Created At: " << scheduleItems[id]->createdAt
  //            << ", Updated At: " << scheduleItems[id]->updatedAt
  //            << endl;
  //     }
  //   }

  void deleteSchedule(int id)
  {
    if (scheduleItems[id] != nullptr && scheduleItems[id]->getId() == id)
    {
      delete scheduleItems[id];
      scheduleItems[id] = nullptr;
      cout << "일정이 삭제되었습니다: ID " << id << endl;
    }
    else
    {
      cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
    }
  }

  // void completeSchedule(int id)
  // {
  //   int num;
  //   if (scheduleItems[id] != nullptr && scheduleItems[id]->getId() == id)
  //   {
  //     scheduleItems[id]->markAsCompleted();
  //     // typeid을 사용하여 클래스를 확인가능하다고 하네요
  //     if (typeid(scheduleItems[id]) == typeid(ReminderSchedule))
  //   }
  //   else
  //   {
  //     cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //   }
  // }

  void searchByTitle(string title)
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cerr << "일정을 찾을 수 없습니다: title " << title << endl;
        return;
      }
      if (scheduleItems[i]->getTitle() == title)
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
        return;
      }
    }
  }

  void searchByDate(string startDate)
  {
    if (scheduleItems[0] == nullptr)
    {
      cerr << "일정을 찾을 수 없습니다: startDate " << startDate << endl;
      return;
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cerr << "일정을 찾을 수 없습니다: startDate " << startDate << endl;
        return;
      }
      if (scheduleItems[i]->getStartDate() == startDate)
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
      }
    }
  }

  void searchByPriority(string priority)
  {
    if (scheduleItems[0] == nullptr)
    {
      cerr << "일정을 찾을 수 없습니다: priority " << priority << endl;
      return;
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        cerr << "일정을 찾을 수 없습니다: priority " << priority << endl;
        return;
      }
      if (scheduleItems[i]->getPriority() == priority)
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
      }
    }
  }

  //   void sortByDate(string startDate, string startTime)
  //   {
  //     if (scheduleItems[0] == nullptr)
  //     {
  //       cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //       return;
  //     }
  //     for (int i = 0; i < 100; i++)
  //     {
  //       if (scheduleItems[i] == nullptr)
  //       {
  //         cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //         break;
  //       }
  //     }
  //   }

  void sortByPriority(string priority)
  {
    if (scheduleItems[0] == nullptr)
    {
      cerr << "일정을 찾을 수 없습니다: priority " << priority << endl;
      return;
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        break;
      }
      if (scheduleItems[i]->getPriority() == "HIGH")
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
      }
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        break;
      }
      if (scheduleItems[i]->getPriority() == "MEDIUM")
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
      }
    }
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        break;
      }
      if (scheduleItems[i]->getPriority() == "LOW")
      {
        scheduleItems[i]->displayAllSchedules();
        cout << endl;
      }
    }
    return;
  }

    void sortByCompleted(string isCompleted)
    {
      if (scheduleItems[0] == nullptr)
      {
        cerr << "일정을 찾을 수 없습니다: isCompleted " << isCompleted << endl;
        return;
      }
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          break;
        }
        if (scheduleItems[i]->getIsCompleted() == false)
        {
          scheduleItems[i]->displayAllSchedules();
        cout << endl;
        }
      }
      for (int i = 0; i < 100; i++)
      {
        if (scheduleItems[i] == nullptr)
        {
          break;
        }
        if (scheduleItems[i]->getIsCompleted() == true)
        {
          scheduleItems[i]->displayAllSchedules();
        cout << endl;
        }
      }
      return;
    }

  bool checkConglict(string startDate, string endDate, string startTime, string endTime)
  {
    for (int i = 0; i < 100; i++)
    {
      if (scheduleItems[i] == nullptr)
      {
        return false;
      }
      if (stoi(scheduleItems[i]->getStartDate().substr(0, 3)) <= stoi(startDate.substr(0, 3)) && stoi(scheduleItems[i]->getStartDate().substr(5, 6)) <= stoi(startDate.substr(5, 6)) && stoi(scheduleItems[i]->getStartDate().substr(8, 9)) <= stoi(startDate.substr(8, 9)) &&
          stoi(scheduleItems[i]->getEndDate().substr(0, 3)) >= stoi(endDate.substr(0, 3)) && stoi(scheduleItems[i]->getEndDate().substr(5, 6)) >= stoi(endDate.substr(5, 6)) && stoi(scheduleItems[i]->getEndDate().substr(8, 9)) >= stoi(endDate.substr(8, 9)) &&
          stoi(scheduleItems[i]->getStartTime().substr(0, 1)) <= stoi(startTime.substr(0, 1)) && stoi(scheduleItems[i]->getStartTime().substr(3, 4)) <= stoi(startTime.substr(3, 4)) &&
          stoi(scheduleItems[i]->getEndTime().substr(0, 1)) >= stoi(endTime.substr(0, 1)) && stoi(scheduleItems[i]->getEndTime().substr(3, 4)) >= stoi(endTime.substr(3, 4)))
      {
        cerr << "충돌이 났습니다. " << endl;
        return true;
      }
    }
  }

  //   void getScheduleType()
  //   {
  //     for (int i = 0; i < 100; i++)
  //     {
  //       if (scheduleItems[i] == nullptr)
  //       {
  //         cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //         return;
  //       }
  //       // typeid을 사용하여 클래스를 확인가능하다고 하네요
  //       if (typeid(scheduleItems[i]) == typeid(GeneralSchedule))
  //       {
  //         cout << "이 일정은 GeneralSchedule입니다.";
  //       }
  //       else if (typeid(scheduleItems[i]) == typeid(MeetingSchedule))
  //       {
  //         cout << "이 일정은 MeetingSchedule입니다.";
  //       }
  //       else if (typeid(scheduleItems[i]) == typeid(ScheduleItem))
  //       {
  //         cout << "이 일정은 ScheduleItem입니다.";
  //       }
  //       else if (typeid(scheduleItems[i]) == typeid(ReminderSchedule))
  //       {
  //         cout << "이 일정은 ReminderSchedule입니다.";
  //       }
  //       return;
  //     }
  //   }

  //   void notifyUser(int id)
  //   {
  //     if (scheduleItems[id] != nullptr && scheduleItems[id]->id == id)
  //     {
  //       // typeid을 사용하여 클래스를 확인가능하다고 하네요
  //       if (typeid(scheduleItems[id]) == typeid(ReminderSchedule))
  //       {
  //         cout << scheduleItems[id]->reminderMessage;
  //         scheduleItems[id]->isReminderSent = true;
  //         return;
  //       }
  //       cerr << "이 일정은 알림 대상이 아닙니다.";
  //     }
  //     else
  //     {
  //       cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //     }
  //   }

  //   void runNotification(int id)
  //   {
  //     if (scheduleItems[id] != nullptr && scheduleItems[id]->id == id)
  //     {
  //       notifyUser(id);
  //     }
  //     else
  //     {
  //       cerr << "일정을 찾을 수 없습니다: ID " << id << endl;
  //     }
  //   }

private:
  ScheduleItem **scheduleItems = new ScheduleItem *[10];
};

int main()
{
  ScheduleManager test;
  for (;;)
  {
    cout << "일정 관리 프로그램입니다. 원하는 작업을 선택하세요: 1. 일정 등록, 2. 전체 일정 조회, 3. 일정 상세 조회, 4. 일정 수정, 5. 일정 삭제, 6. 일정 완료 처리, 7. 제목 검색, 8. 날짜 검색, 9. 중요도 검색, 10. 날짜순 정렬, 11. 중요도순 정렬, 12. 완료 여부순 정렬, 13. 알림 실행, 14. 프로그램 종료" << endl;
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
      cout << "일정 관리 프로그램입니다. 원하는 작업을 선택하세요: 1. GeneralSchedule, 2. MeetingSchedule, 3. TaskSchedule, 4. ReminderSchedule" << endl;
      test.addSchedule();
      continue;
    case 2:
      test.displayAllSchedules();
      break;
    case 3:
      int id;
      cout << "조회할 일정의 ID를 입력하시오: ";
      cin >> id;
      test.displaySchedule(id);
      continue;
    case 4:
      cout << "수정할 일정의 ID를 입력하시오: ";
      cin >> id;
      // test.updateSchedule(id);
      continue;
    case 5:
      cout << "삭제할 일정의 ID를 입력하시오: ";
      cin >> id;
      test.deleteSchedule(id);
      continue;
    case 6:
      cout << "완료 처리할 일정의 ID를 입력하시오: ";
      cin >> id;
      // test.completeSchedule(id);
      continue;
    case 7:
      string title;
      cout << "검색할 일정의 제목을 입력하시오: ";
      cin >> title;
      test.searchByTitle(title);
      continue;
    case 8:
      string startDate;
      cout << "검색할 일정의 시작 날짜를 입력하시오 (YYYY-MM-DD): ";
      cin >> startDate;
      test.searchByDate(startDate);
      continue;
    case 9:
      string priority;
      cout << "검색할 일정의 중요도를 입력하시오 (HIGH, MEDIUM, LOW): ";
      cin >> priority;
      test.searchByPriority(priority);
      continue;
    case 10:
      string startTime;
      cout << "정렬할 일정의 시작 날짜를 입력하시오 (YYYY-MM-DD): ";
      cin >> startDate;
      cout << "정렬할 일정의 시작 시간을 입력하시오 (HH:MM): ";
      cin >> startTime;
      // test.sortByDate(startDate, startTime);
      continue;
    case 11:
      cout << "정렬할 일정의 중요도를 입력하시오 (HIGH, MEDIUM, LOW): ";
      cin >> priority;
      test.sortByPriority(priority);
      continue;
    case 12:
      string isCompleted;
      cout << "정렬할 일정의 완료 여부를 입력하시오 (true, false): ";
      cin >> isCompleted;
      test.sortByCompleted(isCompleted);
      continue;
    case 13:
      cout << "알림을 실행할 일정의 ID를 입력하시오: ";
      cin >> id;
      test.runNotification(id);
      continue;
    case 14:
      cout << "프로그램을 종료합니다." << endl;
      break;
    default:
      cerr << "잘못된 입력입니다." << endl;
      break;
    }
  }

  return 0;
}
