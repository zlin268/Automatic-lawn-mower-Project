/**
 *
 * This file contains the implementation of the WheelController class and all associated member functions that are included in the WheelController.h file.
 * The WheelController class is used to send instructions to the motors for the left and right wheels.
 * The methods in this class will be called by the main loop of the program, when executing instructions.
 * 
 */

#include "WheelController.h"
#include <iostream>
#include <wiringPi.h>


/**
 * Constructor that takes two parameters of object type Motor and initializes them
 *
 * @param leftWheelMotor variable for the motor which will provide power to the left wheel
 * @param leftWheelMotor variable for the motor which will provide power to the right wheel
 *
 */
WheelController::WheelController(Motor& leftWheelMotor, Motor& rightWheelMotor) {
	m_leftWheelMotor = &leftWheelMotor;
	m_rightWheelMotor = &rightWheelMotor;

	m_errorNum = 0;
}

/**
 *
 * Member function destructor which deletes an object: no return
 *
 */
WheelController::~WheelController() {
	
}

/**
 * Function which halts the right/left wheel motors when called
 *
 * @return int (0) containing result of successful halt
 * calls the stop() function which is provided via Motor.h
 */
int WheelController::stopMotor() {
	m_leftWheelMotor->stop();
	m_rightWheelMotor->stop();
	
	return 0;
}

/**
 * Function which calls the start function from Motor.cpp to power the motors in order to rotate the wheels forward
 *
 * @return int from start function return
 *  0: if successful stop
 * -1: if error 
 * start() takes parameter based on Direction enum (Motor.h)
 */
int WheelController::moveForward() {
	m_leftWheelMotor->start(Direction::CCW);
	m_rightWheelMotor->start(Direction::CW);
	
	return 0;
}

/**
 * Function which calls the start function from Motor.cpp to power the motors in order to rotate the wheels backwards
 *
 * @return int from start function return
 *  0: if successful stop
 * -1: if error 
 * start() takes parameter based on Direction enum (Motor.h)
 */
int WheelController::moveBackward() {
	m_leftWheelMotor->start(Direction::CW);
	m_rightWheelMotor->start(Direction::CCW);
	
	return 0;
}

/**
 * Function which calls the start function from Motor.cpp to power ONLY the right wheel motor to perform a right turn
 *
 * @return int from start function return
 *  0: if successful stop
 * -1: if error 
 * start() takes parameter based on Direction enum (Motor.h)
 */
int WheelController::turnLeft(TurnDuration turnDuration) {
	return m_rightWheelMotor->start(Direction::CW, turnDuration);
}

/**
 * Function which calls the start function from Motor.cpp to power ONLY the left wheel motor to perform a right turn
 *
 * @return int from start function return
 *  0: if successful stop
 * -1: if error 
 * start() takes parameter based on Direction enum (Motor.h)
 */
int WheelController::turnRight(TurnDuration turnDuration) {
	return m_leftWheelMotor->start(Direction::CCW, turnDuration);
}

/**
 * Getter function which returns the initialized variable for left wheel motor
 *
 * @return type Motor
 */
Motor* WheelController::getLeftWheelMotor() {
	return m_leftWheelMotor;
}

/**
 * Getter function which returns the initialized variable for right wheel motor
 *
 * @return type Motor
 */
Motor* WheelController::getRightWheelMotor() {
	return m_rightWheelMotor;
}

