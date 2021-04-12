#include <iostream>
#include <thread>
#include <wiringPi.h>
#include "State.h"
#include "Path.h"
#include "Motor.h"
#include "WheelController.h"
#include "BladeController.h"
#include "ButtonController.h"
#include "ExecutionController.h"

int main (void) {
    const int START_PIN = 29;
    const int INPUT_PIN = 1;
    const int UP_PIN = 4;
    const int DOWN_PIN = 28;
    const double LENGTH = 3.0;
    const double WIDTH = 3.0;
    const double CAR_DIAMETER = 0.87;
    const double BLADE_DIAMETER = 0.435;

    State currentState = IDLE;
    Path path(LENGTH, WIDTH, CAR_DIAMETER, BLADE_DIAMETER);

    // wheel motors, based on circuit diagram
    Motor motor1(24, 23);
	Motor motor2(21, 22);

    // blade motor, based on circuit diagram
    Motor motor3(2, 3);
	
	WheelController wheelControl(motor1, motor2);
    BladeController bladeControl(motor3);

    ExecutionController* exec = new ExecutionController(currentState, path, wheelControl, bladeControl);

    ButtonController btn(START_PIN, INPUT_PIN, UP_PIN, DOWN_PIN, currentState, path, *exec);

    std::thread button_thread(&ButtonController::startInputListener, btn);

    std::cout << "button thread now running" << std::endl;

    std::thread exec_thread(&ExecutionController::startExecutionListener, exec);

    std::cout << "exec thread now running" << std::endl;

    exec_thread.join();
    button_thread.join();

    std::cout << "All threads completed" << std::endl;

	return 0;
}
