#include <iostream>

enum class LedState { Off, On, Blink, Error };

int main(){
	int num;


	std::cout << "input number: ";
	std::cin >> num;

	LedState state = static_cast<LedState>(num);


	switch (state) {
        	case LedState::Off:
        		std::cout << "LED OFF\n";
            		break;
        	case LedState::On:
            		std::cout << "LED ON\n";
            		break;
        	case LedState::Blink:
            		std::cout << "LED BLINKING at 1 Hz\n";
            		break;
        	case LedState::Error:
            		std::cerr << "[ERROR] LED driver fault\n";
            		return 1;
    	}	
    return 0;
}
