#include "../include/ScheduleManager.h"
#include "../include/UserManager.h"

#include <iostream>

int main()
{
  ScheduleManager schedule = ScheduleManager();
  UserManager user = UserManager();
  for (;;)
  {
    cout << "\n일정 관리 프로그램입니다. 원하는 작업을 선택하세요: \n1. 사용자 등록\n2. 전체 사용자 조회\n3. 사용자 상세 조회\n4. 사용자 수정\n5. 사용자 삭제\n6. 일정 등록\n7. 전체 일정 조회\n8. 사용자별 일정 조회\n9. 일정 상세 조회\n10. 일정 수정\n11. 일정 삭제\n12. 일정 완료 처리\n13. 제목 검색\n14. 날짜 검색\n15. 중요도 검색\n16. 사용자별 제목 검색\n17. 사용자별 날짜 검색\n18. 사용자별 중요도 검색\n19. 날짜순 정렬\n20. 중요도순 정렬\n21. 완료 여부순 정렬\n22. 사용자별 날짜순 정렬\n23. 사용자별 중요도순 정렬\n24. 사용자별 완료 여부순 정렬\n25. 알림 실행\n26. 프로그램 종료" << endl;
    string startTime, title, startDate, priority;
    try
    {
      switch (schedule.inputInt())
      {
      case 1:
      {
        user.addUser();
        break;
      }
      case 2:
      {
        user.displayAllUser();
        break;
      }
      case 3:
      {
        cout << "조회할 사용자의 ID를 입력하시오: ";
        user.displayUserById(schedule.inputInt());
        break;
      }
      case 4:
      {
        cout << "수정할 사용자의 ID를 입력하시오: ";
        user.updateUser(schedule.inputInt());
        break;
      }
      case 5:
      {
        cout << "삭제할 사용자의 ID를 입력하시오: ";
        user.deleteUser(schedule.inputInt());
        break;
      }
      case 6:
      {
        schedule.addSchedule();
        break;
      }
      case 7:
      {
        schedule.displayAllSchedules();
        break;
      }
      case 8:
      {
        cout << "조회할 일정의 사용자 ID를 입력하시오: ";
        schedule.searchByScheduleByUserId(schedule.inputInt());
        break;
      }
      case 9:
      {
        cout << "조회할 일정의 ID를 입력하시오: ";
        schedule.displaySchedule(schedule.inputInt());
        break;
      }
      case 10:
      {
        cout << "수정할 일정의 ID를 입력하시오: ";
        schedule.updateSchedule(schedule.inputInt());
        break;
      }
      case 11:
      {
        cout << "삭제할 일정의 ID를 입력하시오: ";
        schedule.deleteSchedule(schedule.inputInt());
        break;
      }
      case 12:
      {
        cout << "완료 처리할 일정의 ID를 입력하시오: ";
        schedule.completeSchedule(schedule.inputInt());
        break;
      }
      case 13:
      {
        cout << "검색할 일정의 제목을 입력하시오: ";
        schedule.searchByTitle(schedule.inputString());
        break;
      }
      case 14:
      {
        cout << "검색할 일정의 시작 날짜를 입력하시오 (YYYY-MM-DD): ";
        schedule.searchByDate(schedule.inputString());
        break;
      }
      case 15:
      {
        cout << "검색할 일정의 중요도를 입력하시오 (LOW, MEDIUM, HIGH): ";
        schedule.searchByPriority(schedule.inputString());
        break;
      }
      case 16:
      {
        cout << "검색할 일정의 사용자 ID과 제목을 입력하시오: ";
        schedule.searchByUserAndTitle(schedule.inputInt(), schedule.inputString());
        break;
      }
      case 17:
      {
        cout << "검색할 일정의 사용자 ID과 시작 날짜를 입력하시오 (YYYY-MM-DD): ";
        schedule.searchByUserAndDate(schedule.inputInt(), schedule.inputString());
        break;
      }
      case 18:
      {
        cout << "검색할 일정의 사용자 ID과 중요도를 입력하시오 (LOW, MEDIUM, HIGH): ";
        schedule.searchByUserAndPriority(schedule.inputInt(), schedule.inputString());
        break;
      }
      case 19:
      {
        cout << "날짜순으로 정렬합니다.\n"
             << endl;
        schedule.sortByDate();
        break;
      }
      case 20:
      {
        cout << "중요도순으로 정렬합니다.\n"
             << endl;
        schedule.sortByPriority();
        break;
      }
      case 21:
      {
        cout << "완료 여부순으로 정렬합니다.\n"
             << endl;
        schedule.sortByCompleted();
        break;
      }
      case 22:
      {
        cout << "날짜순으로 정렬할 사용자를 입력하시오: ";
        schedule.sortByUserAndDate(schedule.inputInt());
        break;
      }
      case 23:
      {
        cout << "중요도순으로 정렬할 사용자를 입력하시오: ";
        schedule.sortByUserAndPriority(schedule.inputInt());
        break;
      }
      case 24:
      {
        cout << "완료 여부순으로 정렬할 사용자를 입력하시오: ";
        schedule.sortByUserAndCompleted(schedule.inputInt());
        break;
      }
      case 25:
      {
        cout << "알림을 실행할 일정의 ID를 입력하시오: ";
        schedule.runNotification(schedule.inputInt());
        break;
      }
      case 26:
      {
        cout << "프로그램을 종료합니다." << endl;
        schedule.exit();
        user.exit();
        return 0;
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
