#include "Motor.h"
#include "MotorController.h"
#include <wiringPi.h>
#include <iostream>

// NOTE: Must be compiled with argument "-lwiringPi" in order to link the wiringPi library

int main (void) {
	try {
		Motor motor1(24, 23); // creates the motor vars, assigns GPIO pins
		Motor motor2(21, 22);
		
		motor1.start(Direction::CW);
		delay(1000); 
		motor2.start(Direction::CW);
		delay(1000);
		motor1.stop();
		motor2.stop();
	} catch (...) {
		std::cout << "Something went wrong..." << std::endl;
		return -1;
	}
	return 0;
}
