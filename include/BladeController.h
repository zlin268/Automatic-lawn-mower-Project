/**
 *
 * This file contains the declaration of the BladeController class and all associated member functions and attributes.
 * The BladeController class controls the blade and is responsible for starting/stopping the blade
 * The blade will be always on when the mower is in the Mowing state
 *
 */

#ifndef BLADECONTROLLER_H
#define BLADECONTROLLER_H

#include "Motor.h"
#include "MotorController.h"

class BladeController : public MotorController {
	public:
		BladeController(Motor& bladeMotor);
		~BladeController();
		int stopMotor();
		int startMotor();
		Motor* getBladeMotor();

	protected:

	private:
		Motor* m_bladeMotor;

};

#endif // BLADECONTROLLER_H
