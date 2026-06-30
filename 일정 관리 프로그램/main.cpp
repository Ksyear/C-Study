#include "ScheduleManager.h"

#include <iostream>

int main()
{
  ScheduleManager test = ScheduleManager();
  for (;;)
  {
    cout << "\n일정 관리 프로그램입니다. 원하는 작업을 선택하세요: \n 1. 일정 등록\n 2. 전체 일정 조회\n 3. 일정 상세 조회\n 4. 일정 수정\n 5. 일정 삭제\n 6. 일정 완료 처리\n 7. 제목 검색\n 8. 날짜 검색\n 9. 중요도 검색\n 10. 날짜순 정렬\n 11. 중요도순 정렬\n 12. 완료 여부순 정렬\n 13. 알림 실행\n 14. 프로그램 종료" << endl;
    string startTime, title, startDate, priority;
    try
    {
      switch (test.inputInt())
      {
      case 1:
      {
        cout << "원하는 작업을 선택하세요: 1. GeneralSchedule, 2. MeetingSchedule, 3. TaskSchedule, 4. ReminderSchedule" << endl;
        test.addSchedule();
        break;
      }
      case 2:
      {
        test.displayAllSchedules();
        break;
      }
      case 3:
      {
        cout << "조회할 일정의 ID를 입력하시오: ";
        test.displaySchedule(test.inputInt());
        break;
      }
      case 4:
      {
        cout << "수정할 일정의 ID를 입력하시오: ";
        test.updateSchedule(test.inputInt());
        break;
      }
      case 5:
      {
        cout << "삭제할 일정의 ID를 입력하시오: ";
        test.deleteSchedule(test.inputInt());
        break;
      }
      case 6:
      {
        cout << "완료 처리할 일정의 ID를 입력하시오: ";
        test.completeSchedule(test.inputInt());
        break;
      }
      case 7:
      {
        cout << "검색할 일정의 제목을 입력하시오: ";
        cin >> title;
        test.searchByTitle(title);
        break;
      }
      case 8:
      {
        cout << "검색할 일정의 시작 날짜를 입력하시오 (YYYY-MM-DD): ";
        cin >> startDate;
        test.searchByDate(startDate);
        break;
      }
      case 9:
      {
        cout << "검색할 일정의 중요도를 입력하시오 (HIGH, MEDIUM, LOW): ";
        cin >> priority;
        test.searchByPriority(priority);
        break;
      }
      case 10:
      {
        test.sortByDate();
        break;
      }
      case 11:
      {
        test.sortByPriority();
        break;
      }
      case 12:
      {
        test.sortByCompleted();
        break;
      }
      case 13:
      {
        cout << "알림을 실행할 일정의 ID를 입력하시오: ";
        test.runNotification(test.inputInt());
        break;
      }
      case 14:
      {
        cout << "프로그램을 종료합니다." << endl;
        test.exit();
        return 0;
        break;
      }
      default:
      {
        cerr << "잘못된 입력입니다." << endl;
        break;
      }
      }
    }
    catch (const std::exception &e)
    {
      cerr << e.what() << endl;
    }
  }

  return 0;
}
