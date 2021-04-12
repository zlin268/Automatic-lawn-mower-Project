/**
 *
 * This file contains the declaration of the WheelController class and all associated member functions and attributes.
 * The WheelController class is used to send instructions to the motors for the left and right wheels.
 * The methods in this class will be called by the main loop of the program, when executing instructions.
 *
 */

#ifndef WHEELCONTROLLER_H
#define WHEELCONTROLLER_H

#include "MotorController.h"
#include "Motor.h"

enum TurnDuration { 
	positionOne = 900, // standard TR or TL
	positionTwo = 1075, // TR after MB
	positionThree = 1100, // TL before finish
	positionFour = 1450, // TL after MB
	positionFive = 900 // placeholder value for additional turn duration (may need to be added, depending on terrain)
};

class WheelController : public MotorController {
	public:
		WheelController(Motor& leftWheelMotor, Motor& rightWheelMotor);
		~WheelController();
		int stopMotor();
		int moveForward();
		int moveBackward();
		int turnLeft(TurnDuration turnDuration);
		int turnRight(TurnDuration turnDuration);
		Motor* getLeftWheelMotor();
		Motor* getRightWheelMotor();
		
	protected:
		
	private:
		Motor* m_leftWheelMotor;
		Motor* m_rightWheelMotor;
};

#endif // WHEELCONTROLLER_H
