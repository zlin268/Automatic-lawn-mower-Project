/**
 * This file contains the implementation of the Bladecontroller class (similar to WheelController) and all associated member functions that are included in the BladeController.h file.
 * The BladeController class is used to send instructions to the motor for the blade on the mowing device.
 * The methods in this class will be called by the main loop of the program, when executing instructions.
 * 
 */

#include "BladeController.h"

/**
 * Constructor that takes one parameters of object type Motor and initializes it
 *
 * @param bladeMotor variable for the motor which will provide power to the blade
 *
 */
BladeController::BladeController(Motor& bladeMotor) {
	m_bladeMotor = &bladeMotor;
}

/**
 * Member function destructor which deletes an object: no return
 */
BladeController::~BladeController() {

}

/**
 * Function which halts power to the blade motor when called via stop function from the Motor class
 *
 * @return int (0) containing result of successful halt
 */
int BladeController::stopMotor() {
	m_bladeMotor->stop();

	return 0;
}

/**
 * Function which calls the start function from the Motor class to power the blade motor to move it in a certain direction
 *
 * @return int from start function return
 *  0: if successful stop
 * -1: if error 
 * start() takes parameter (clockwise/counterclockwise) based on Direction enum (Motor.h)
 */
int BladeController::startMotor() {
	m_bladeMotor->start(Direction::CCW);

	return 0;
}

/**
 * Getter function which returns the initialized variable for blade motor
 *
 * @return type Motor
 */
Motor* BladeController::getBladeMotor() {
	return m_bladeMotor;
}

