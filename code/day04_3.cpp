#include <iostream>
#include <array>

int main(){
	std::array<int, 8> buf{};
	std::size_t head = 0;  // 다음 push 위치
	std::size_t tail = 0;  // 다음 pop 위치
	std::size_t count = 0; // 현재 저장된 요소 수
	
	for(;;){
		std::cout << "Cmd: ";
		char a;
		std::cin >> a;
		if(a=='P'){
			if(count>=8){
				std::cerr << "[FULL] errer";
				break;
			}
			std::cin >> buf[count];
			count++;
			head++;
		} else if(a=='G'){
			std::cout << "Pop: " << buf[tail] << "\n";
			count--;
			tail++;
		} else if(a=='S'){
			std::cout << "[ ";
			for(int i=0;i<8;i++){
				if(i<=count) std::cout << buf[i] << " ";
				else if(count<8) std::cout << "0 ";
			}
			std::cout << "] head=" << head << " tail=" << tail << " count=" << count << "\n";
		} else if(a=='Q') break;
	} 
	return 0;
}
