/**
 *
 * This file contains the declaration of the ExecutionController class and all associated member functions and attributes.
 * The ExecutionController class moves the mower as per the instructions generated
 * Depending on whether the current instruction is to move forward/backward or turn left/right it will send instructions to the wheel controller class
 *
 */

#ifndef EXECUTIONCONTROLLER_H
#define EXECUTIONCONTROLLER_H

#include <string>
#include <deque>
#include "State.h"
#include "Instruction.h"
#include "Path.h"
#include "WheelController.h"
#include "BladeController.h"

class ExecutionController {
	public:
		ExecutionController(State& currentState, Path& path, WheelController& wheelControl, BladeController& bladeControl);
		~ExecutionController();
		int startExecutionListener();
		int assignInstructions();
		int clearInstructions();
		State getCurrentState();
		void sendShutDownSignal();
        
	protected:
		
	private:
		State* m_currentState;
		Path* m_path;
		WheelController* m_wheelControl;
		BladeController* m_bladeControl;
		std::deque<Instruction> m_remainingInstructions;
		bool m_shutDownFlag;
		bool m_isBladeSpinning;

		int executeInstruction(Instruction instruction);
};

#endif // EXECUTIONCONTROLLER_H
