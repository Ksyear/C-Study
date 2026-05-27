#include <iostream>

int main(){
	int cycles, duty;
	int H_count=0, L_count=0;

	std::cout << "Enter cycles, duty: ";
	std::cin >> cycles >> duty;
	
	for(int j=1;j<=cycles;j++){
		std::cout << "Cycle " << j << ":\n";
		for(int i=0;i<duty;i++){
			std::cout << "H";
			H_count++;
		}

		for(int i=duty;i<100;i++){
			std::cout << "L";
			L_count++;	
		}
		std::cout << "\n";
	}
	
	std::cout << "Toral H = " << H_count << ", L = " << L_count << ", H ration = " << (double)H_count/(H_count+L_count)*100 << "%";
	return 0;
}	
