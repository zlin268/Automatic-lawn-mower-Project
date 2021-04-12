/**
 * This file contains the implementation of the Motor class and all associated member functions that are included in the Motor.h file.
 * The Motor class is used to send power to the motor control board, which is connected through the GPIO pins on the RPi.
 * The motor control board is used to spin the motors in a clockwise or counterclockwise direction.
 * The methods in this class will be used by any classes that are children of the MotorController class.
 * 
 */

#include "Motor.h"
#include <wiringPi.h>
#include <iostream>

/**
 * Constructor that takes two parameters of type int that correlate to general purpose input/output pins on the RPi
 *
 * @param pinCW: GPIO pin for the right wheel which tells the motor to spin it clockwise at HIGH/LOW value
 * @param pinCCW: GPIO pin for the left wheel which tells the motor to spin it counter clockwise 
 *
 */
Motor::Motor(int pinCW, int pinCCW) {
	wiringPiSetup();
	pinMode(pinCW, OUTPUT);
	pinMode(pinCCW, OUTPUT);
	
	m_pinCW = pinCW;
	m_pinCCW = pinCCW;
	m_errorNum = 0;
}

/**
 * Member function destructor which deletes an object: no return
 */
Motor::~Motor() {
	
}

/**
 * Function which stops power to the wheel at location on the RPi specified by each pin
 * Each pin is provided a value of LOW which halts power to the motor
 * Return value is 0 for successful stoppage
 */
int Motor::stop() {
	digitalWrite(m_pinCW, LOW);
	digitalWrite(m_pinCCW, LOW);
	
	return 0;
}

/**
 * Function which gives power to the wheel at location on the RPi specified by each pin
 * Return value calls the function spinClockwise() if the direction provided is CW
 * Or, return value calls the function spinCounterClockwise() if the direction provided is CCW
 * Else, the function returns -2 for error
 */
int Motor::start(Direction direction) {
	switch (direction) {
		case CW:
			return spinClockwise();
		case CCW:
			return spinCounterClockwise();
		default:
			m_errorNum = -2;
			return -2;
	}
}

/**
 * Function similar to start above which gives power to the wheel at location on the RPi specified by each pin and specifies a duration recognized by an int
 * Return value on clockwise/counter clockwise  calls stop() after the delay is complete (specified by duration)
 * Else, the function returns -2 for error
 */
int Motor::start(Direction direction, int duration) {
	switch (direction) {
		case CW:
			spinClockwise();
			delay(duration);
			return stop();
		case CCW:
			spinCounterClockwise();
			delay(duration);
			return stop();
		default:
			m_errorNum = -2;
			return -2;
	}
}

/**
 * Getter function which returns the value of the right wheel GPIO pin 
 */
int Motor::getPinCW() {
	return m_pinCW;	
}

/**
 * Getter function which returns the value of the left wheel GPIO pin
 */
int Motor::getPinCCW() {
	return m_pinCCW;	
}

/**
 * Getter function which returns the errorNum variable which holds the value of the current error call 
 */
int Motor::getErrorNum() {
	return m_errorNum;	
}

/**
 * Function which spins the wheel at the specified GPIO pin location at a fixed speed (HIGH)
 * Returns 0 if successful or an error number -1 
 */
int Motor::spinClockwise() {
	try {
		digitalWrite(m_pinCW, HIGH);
	} catch (...) {
		m_errorNum = -1;
		return -1;
	}
	
	return 0;
}

/**
 * Function which spins the counter clockwise at the specified GPIO pin location at a fixed speed (HIGH)
 * Returns 0 if successful or an error number -1 
 */
int Motor::spinCounterClockwise() {
	try {
		digitalWrite(m_pinCCW, HIGH);
	} catch (...) {
		m_errorNum = -1;
		return -1;
	}
	
	return 0;
}

