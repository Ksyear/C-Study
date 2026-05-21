#include <iostream>

int main(){
	double sum=0, num=0;
	int count=0, ExceededCount=0;

	std::cout << "Enter 5 temperature samples: \n";
	for(int i=0;i<5;i++){
		std::cin >> num;
		count++;
		if(num>85.0) {
			std::cout << "[WARN] sample #" << count << " exceeded threshold: " << num << " C\n";
			ExceededCount++;
		}
		sum+=num;
	}
	std::cout << "Average: " << sum/count << " C\nExceeded count: " << ExceededCount << "\n";
	return 0;
}
