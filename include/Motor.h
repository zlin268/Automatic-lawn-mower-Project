/**
 *
 * This file contains the declaration of the Motor class and all associated member functions and attributes.
 * The Motor class is used to send power to the motor control board, which is connected through the GPIO pins on the RPi.
 * The motor control board is used to spin the motors in a clockwise or counterclockwise direction.
 * The methods in this class will be used by any classes that are children of the MotorController class.
 *
 */

#ifndef MOTOR_H
#define MOTOR_H

enum Direction { 
	CW, // Clockwise
	CCW // Counter-Clockwise
};

class Motor {
	public:
		Motor(int pinCW, int pinCCW);
		~Motor();
		int stop();
		int start(Direction direction);
		int start(Direction direction, int duration);
		int getPinCW();
		int getPinCCW();
		int getErrorNum();
		
	protected:
		
	private:
		int m_pinCW;
		int m_pinCCW;
		int m_errorNum;
		
		int spinClockwise();
		int spinCounterClockwise();
		
};

#endif // MOTOR_H
