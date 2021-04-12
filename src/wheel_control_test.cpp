/**
 * This file contains the implementation of the wheel_control_test class and all associated member functions that are included in the WheelController.h and Motor.h header files.
 * This class effectively just tests our motors and movement, was used in debugging
 * 
 */

#include "Motor.h"
#include "WheelController.h"
#include "BladeController.h"
#include <wiringPi.h>
#include <iostream>

// NOTE: Must be compiled with argument "-lwiringPi" in order to link the wiringPi library


/**
 * main function, just testing the motors and movement.
 *
 * @return 0: working properly
 * @return -1: an error occured, not working properly!
 *
 */
int main (void) {
	try {
		Motor motor1(24, 23); // creates the motor vars, assigns GPIO pins
		Motor motor2(21, 22);
		Motor motor3(2, 3);
	
		WheelController controller(motor1, motor2); // creates the controller, passes it the l/r motors
		BladeController blade(motor3);

		controller.moveForward(); // movement/turning, delay keeps us moving
		delay(1000);
		controller.stopMotor();
		delay(1000);
		controller.moveBackward();
		delay(1000);
		controller.stopMotor();
		delay(1000);
		controller.turnLeft(TurnDuration::positionOne);
		delay(1000);
		controller.turnRight(TurnDuration::positionFour);
		delay(1000);
		Motor* ptr1 = controller.getLeftWheelMotor();

		ptr1->start(Direction::CW);
		delay(1000);
		ptr1->stop();
		delay(1000);

		blade.startMotor();
		delay(2000);
		blade.stopMotor();
	} catch (...) {
		std::cout << "Something went wrong..." << std::endl;
		return -1;
	}

	return 0;
}
