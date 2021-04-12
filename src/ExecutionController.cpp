/**
 * This file contains the implementation of the ExecutionController class and all associated member functions that are included in the ExecutionController.h file.
 * Used to send instructions to the motors and move the mower
 * Depending on move/turn it will send different instructions to the wheel controller (motors)
 */

#include "ExecutionController.h"
#include <iostream>
#include <wiringPi.h>

/**
 * Constructor that takes 3 parameters and initializes own variables
 *
 * @param currentState: GPIO pin for the right wheel which tells the motor to spin it clockwise at HIGH/LOW value
 * @param path: Path the mower will take
 * @param wheelControl: reference to the wheelcontroller object that will be used (motors already assigned)
 * @param bladeControl: reference to the bladecontroller object that will be used (motor already assigned)
 *
 */
ExecutionController::ExecutionController(State& currentState, Path& path, WheelController& wheelControl, BladeController& bladeControl) {
    m_currentState = &currentState;
    m_path = &path;
    m_wheelControl = &wheelControl;
    m_bladeControl = &bladeControl;
    m_shutDownFlag = false;
    m_isBladeSpinning = false;
}

/**
 * Member function destructor which deletes an object: no return
 */
ExecutionController::~ExecutionController() {
	
}

/**
 * Function that iterates through the instructions and determines how many are left
 * Return value is 0 for success
 */
int ExecutionController::startExecutionListener() {
    while (!m_shutDownFlag) {
        if (m_remainingInstructions.size() > 0) {
            if (*m_currentState == MOWING) {
                if (!m_isBladeSpinning) {
                    m_bladeControl->startMotor();
                    m_isBladeSpinning = true;
                }

                Instruction currentInstruction = m_remainingInstructions.front();
                    
                std::cout << "# of instructions left: " << m_remainingInstructions.size() << std::endl;
                std::cout << "instruction: " << currentInstruction.action << currentInstruction.value << std::endl;

                m_remainingInstructions.pop_front();
                executeInstruction(currentInstruction);
            } else { // this will be reach only if we are in paused state (i.e. we need to stop blade from spinning while paused)
                m_bladeControl->stopMotor();
                m_isBladeSpinning = false;
            }
        } else { // if we finish all instructions
            if (*m_currentState == MOWING) { // only if we were previously mowing and finished all instructions, return to idle state, stop blade motor
                m_bladeControl->stopMotor();
                m_isBladeSpinning = false;
                *m_currentState = IDLE;
            }
        }
    }

    delay(1000); // small delay before destroying current thread to avoid any errors

    return 0;
}

/**
 * Function that sets the remaining instructions
 * Return value is 0 for success
 */
int ExecutionController::assignInstructions() {
    std::cout << "assigning instructions" << std::endl;

    if (m_path->getInstructions().size() > 0) {
        std::cout << "assigned" << std::endl;
        m_remainingInstructions = m_path->getInstructions();
    } else {
        std::cout << "not assigned" << std::endl;
    }

    return 0;
}

/**
 * Function that clears the current instruction set, assuming > 0
 * Return value is 0 for success
 */
int ExecutionController::clearInstructions() {
    std::cout << "clearing instructions" << std::endl;

    if (m_remainingInstructions.size() > 0) {
        m_remainingInstructions.clear();
    }

    return 0;
}

/**
 * Getter function that returns the current state of the mower
 */
State ExecutionController::getCurrentState() {
	return *m_currentState;
}

void ExecutionController::sendShutDownSignal() {
    m_shutDownFlag = true;

    return;
}

/**
 * Function used in the executionlistener function to move the mower depending on the instruction
 * Return value is 0 for success
 */
int ExecutionController::executeInstruction(Instruction instruction) {
    // parse instruction, call appropriate motor classes
    double duration;

    if (instruction.action == "MF") { // if move forward
        duration = instruction.value * 925;

        m_wheelControl->moveForward();
        delay(duration);
        m_wheelControl->stopMotor();
    } else if (instruction.action == "MB") { // if move backwards
        duration = instruction.value * 925;

        m_wheelControl->moveBackward();
        delay(duration);
        m_wheelControl->stopMotor();
    } else if (instruction.action == "TL") { // if turn left
        if (instruction.value == 90.0) {
            m_wheelControl->turnLeft(TurnDuration::positionOne);
        } else if (instruction.value == 100.0) {
            m_wheelControl->turnLeft(TurnDuration::positionThree);
        } else if (instruction.value == 110.0) {
            m_wheelControl->turnLeft(TurnDuration::positionFour);
        } else { // 180.0
            m_wheelControl->turnLeft(TurnDuration::positionOne);
            m_wheelControl->turnLeft(TurnDuration::positionOne);
        }
    } else if (instruction.action == "TR") { // if turn right
        if (instruction.value == 90.0) {
            m_wheelControl->turnRight(TurnDuration::positionOne);
        } else if (instruction.value == 100.0) {
            m_wheelControl->turnRight(TurnDuration::positionTwo);
        } else { // 180.0
            m_wheelControl->turnRight(TurnDuration::positionOne);
            m_wheelControl->turnRight(TurnDuration::positionOne);
        }
    }

    return 0;
}
