/**
 * This file contains the implementation of the MotorController class and all associated member functions that are included in the MotorController.h file.
 * The MotorController class is used as a parent class to any classes which need to send instructions to a DC motor.
 * The methods and attributes contained in this class are common to all child classes.
 * 
 * TODO: Add comments describing each method in terms of usage, parameters, and return value
 */

#include "MotorController.h"

/**
 * Constructor, takes no parameters
 * Just initializes the errnum
 */
MotorController::MotorController() {
	m_errorNum = 0;	
}

/**
 * Member function destructor which deletes an object: no return
 */
MotorController::~MotorController() {
	
}

/**
 * Stop moter function
 */
int MotorController::stopMotor() {
	return 0;
}

/**
 * Returns the errnum generated
 */
int MotorController::getErrorNum() {
	return m_errorNum;
}
