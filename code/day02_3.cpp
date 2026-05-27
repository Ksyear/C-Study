#include <iostream>

int main(){
	int num, a;
	
	std::cout << "Enter battery percent and charger(0/1): ";
	std::cin >> num >> a;

	if(num<5&&a==0) std::cerr << "[CRITICAL] CRITICAL_SHUTDOWN";
	else if(num<20&&a==0) std::cout << "LOW_POWER_MODE";
	else if(num<100&&a==1) std::cout << "CHARGING";
	else if(num==100&&a==1) std::cout << "FULLY_CHARGED";
	else std::cout << "NORMAL";

	return 0;
}
