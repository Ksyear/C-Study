#include<iostream>

int main(){
	int num;
	
	std::cout << "input number: ";
	std::cin >> num;

	if(num>=90) std::cout << "A";
	else if(num>=80) std::cout << "B";
	else if(num>=70) std::cout << "C";
	else if(num>=60) std::cout << "D";
	else std::cout << "F";

	return 0;
}
