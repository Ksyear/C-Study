#include <iostream>
#include <array>

int main(){
	std::array<int, 10> data = {45, -3, 27, 18, 0, 99, -27, 56, 12, 8};
	int max=data[0], min=data[0];
	for(int i : data){
		if(i>max) max=i;
		else if(i<min) min=i;
	}
	std::cout << "Max = " << max << ", Min = " << min;
	return 0;
}
