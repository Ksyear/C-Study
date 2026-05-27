#include <iostream>

int main(){
	int N=0, num=0;
	
	std::cout << "Enter N: ";
	std::cin >> N;	

	for(int i=2; i<=N; i++){
		for(int j=2; j<i; j++){
			if(i%j==0) num=1;
		}
		if(num==0) std::cout << i << " ";
		num=0;
	}

	return 0;
}
