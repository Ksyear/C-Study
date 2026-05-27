#include <iostream>

int main(){
	int N, sum=0;
	
	std::cout << " Enter N: ";
	std::cin >> N;

	for(int i=1;i<=N;i++) sum+=i;
	
	std::cout << "Sum = " << sum;

	return 0;
}
