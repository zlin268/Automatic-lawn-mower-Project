/**
 *
 * This file contains the declaration of the MotorController class and all associated member functions and attributes.
 * The MotorController class is used as a parent class to any classes which need to send instructions to a DC motor.
 * The methods and attributes contained in this class are common to all child classes.
 *
 */

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController {
	public:
		MotorController();
		virtual ~MotorController();
		virtual int stopMotor();
		int getErrorNum();
		
	protected:
		int m_errorNum;
		
	private:
		
};

#endif // MOTORCONTROLLER_H
