/**
 * This file contains the implementation of the ButtonController class and all associated member functions that are included in the ButtonController.h file.
 * 
 * 
 */

#include "ButtonController.h"
#include <wiringPi.h>
#include <iostream>
#include "ssd1306_i2c.h"
#include <stdio.h>
#include <string.h>

/**
 * Constructor
 *
 * @param pinStart: red button (start)
 * @param pinSetDimensions: blue button (set dimensions/pause/resume)
 * @param pinUpArrow: left yellow button (up)
 * @param pinDownArrow: right yellow button (down)
 * @param currentState: current state of the machine
 * @param path: current path determined
 * @param exeControl: reference to execution controller object
 *
 */
ButtonController::ButtonController(int pinStart, int pinSetDimensions, int pinUpArrow, int pinDownArrow, State& currentState, Path& path, ExecutionController& exeControl) {
	m_pinStart = pinStart; // init variables
	m_pinSetDimensions = pinSetDimensions;
	m_pinUpArrow = pinUpArrow;
	m_pinDownArrow = pinDownArrow;
	m_currentState = &currentState;
	m_path = &path;
	m_exeControl = &exeControl;
	m_inputLength = 0;
	m_inputWidth = 0;
	m_errorNum = 0;
	m_shutDownFlag = false;

	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS); // initialize screen for displaying states, info, etc.
	welcomeScreen();
	delay(3000);
}

/**
 * Member function destructor which deletes an object: no return
 */
ButtonController::~ButtonController() {

}

/**
 * Function for buttons, will call specific functions
 * @return 0: success
 * @return -1: failure, set errnum too
 */
int ButtonController::sendButtonPress(int pinButtonPressed) {
	if (pinButtonPressed == m_pinStart) {
		onStart();
	} else if (pinButtonPressed == m_pinSetDimensions) {
		onSetDimensions();
	} else if (pinButtonPressed == m_pinUpArrow) {
		onUpArrow();
	} else if (pinButtonPressed == m_pinDownArrow) {
		onDownArrow();
	} else {
		m_errorNum = -1;
		return -1;
	}

	return 0;
}

int ButtonController::startInputListener() {
    wiringPiSetup();

	pinMode(m_pinStart, INPUT); // red
    pullUpDnControl(m_pinStart, PUD_UP);

    pinMode(m_pinSetDimensions, INPUT); // blue
    pullUpDnControl(m_pinSetDimensions, PUD_UP);

    pinMode(m_pinUpArrow, INPUT); // up yellow
    pullUpDnControl(m_pinUpArrow, PUD_UP);

    pinMode(m_pinDownArrow, INPUT); // down yellow
    pullUpDnControl(m_pinDownArrow, PUD_UP);

	// these flags are used to prevent multiple presses when the user holds a button down
    bool buttonPressed = false;
    bool buttonPressed2 = false;
    bool buttonPressed3 = false;
    bool buttonPressed4 = false;

	idleScreen();

    while (!m_shutDownFlag) {
        if (digitalRead(m_pinStart) == LOW) {
            if (buttonPressed) {
                std::cout << "Start button pressed" << std::endl;
                buttonPressed = false;
				onStart();
                delay(500);
            }
        } else {
            buttonPressed = true;
        }
        
        if (digitalRead(m_pinSetDimensions) == LOW) {
            if (buttonPressed2) {
                std::cout << "Input button pressed" << std::endl;
                buttonPressed2 = false;
				onSetDimensions();
                delay(500);
            }
        } else {
            buttonPressed2 = true;
        }

        if (digitalRead(m_pinUpArrow) == LOW) {
            if (buttonPressed3) {
                std::cout << "Up button pressed" << std::endl;
                buttonPressed3 = false;
				onUpArrow();
                delay(500);
            }
        } else {
            buttonPressed3 = true;
        }

        if (digitalRead(m_pinDownArrow) == LOW) {
            if (buttonPressed4) {
                std::cout << "Down button pressed" << std::endl;
                buttonPressed4 = false;
				onDownArrow();
                delay(500);
            }
        } else {
            buttonPressed4 = true;
        }  
    }

	goodbyeScreen();

	return 0;
}

/**
 * Getter function, returns the current state
 */
State ButtonController::getCurrentState() {
	return *m_currentState;
}

/**
 * Getter function, returns the current errnum
 */
int ButtonController::getErrorNum() {
	return m_errorNum;
}

/**
 * Function to determine what state we should move to when red button is pressed
 * @return 0: success
 * @return -2: failure, -2 is more specific so we set that to see where code is developing errors
 */
int ButtonController::onStart() {
	switch (*m_currentState) {
		case IDLE: // if curr state is idle and red button pressed... etc.
			// tell executionController to start executing instructions
			m_exeControl->assignInstructions();
			*m_currentState = MOWING;
			mowingScreen();
			break;
		case MOWING:
			// tell executionController to stop executing instructions, end current mowing job
			m_exeControl->clearInstructions();
			*m_currentState = IDLE;
			idleScreen();
			break;
		case INPUT_LENGTH:
			// don't set new inputs
			*m_currentState = IDLE;
			idleScreen();
			break;
		case INPUT_WIDTH:
			// don't set new inputs
			*m_currentState = IDLE;
			idleScreen();
			break;
		case PAUSED:
			// tell executionController to stop executing instructions, end current mowing job
			m_exeControl->clearInstructions();
			*m_currentState = IDLE;
			idleScreen();
			break;
		default:
			m_errorNum = -2;
			return -2;
	}

	return 0;
}

/**
 * Function to determine what state we should move to when blue button is pressed
 * @return 0: success
 * @return -2: failure, -2 is more specific so we set that to see where code is developing errors
 */
int ButtonController::onSetDimensions() {
	switch (*m_currentState) {
		case IDLE: // if blue button pressed and currently on idle state
			m_inputLength = 0;
			m_inputWidth = 0;
			*m_currentState = INPUT_LENGTH;
			lwInputMode((char*) m_inputLength, 1, INPUT_LENGTH);
			break;
		case MOWING:
			// tell executionController to pause executing instructions
			*m_currentState = PAUSED;
			pausedScreen();
			break;
		case INPUT_LENGTH:
			// accept length input, listen for width input
			*m_currentState = INPUT_WIDTH;
			lwInputMode((char*) m_inputWidth, 1, INPUT_WIDTH);
			break;
		case INPUT_WIDTH:
			// send new dimensions to path object
			m_path->setDimensions(m_inputLength, m_inputWidth);
			*m_currentState = IDLE;
			idleScreen();
			break;
		case PAUSED:
			// tell executionController to resume executing instructions
			*m_currentState = MOWING;
			mowingScreen();
			break;
		default:
			m_errorNum = -2;
			return -2;
	}

	return 0;
}

/**
 * Function to determine what state we should move to when left yellow button is pressed (up)
 * @return 0: success
 * @return -2: failure, -2 is more specific so we set that to see where code is developing errors
 */
int ButtonController::onUpArrow() {
	switch (*m_currentState) {
		case IDLE: // up arrow has no functionality on idle/paused/mowing state
			break;
		case MOWING:
			break;
		case INPUT_LENGTH:
			m_inputLength += 1;
			lwInputMode((char*) m_inputLength, 1, INPUT_LENGTH);
			break;
		case INPUT_WIDTH:
			m_inputWidth += 1;
			lwInputMode((char*) m_inputWidth, 1, INPUT_WIDTH);
			break;
		case PAUSED:
			break;
		default:
			m_errorNum = -2;
			return -2;
	}

	return 0;
}

/**
 * Function to determine what state we should move to when right yellow button is pressed (down)
 * @return 0: success
 * @return -2: failure, -2 is more specific so we set that to see where code is developing errors
 */
int ButtonController::onDownArrow() {
	switch (*m_currentState) {
		case IDLE: // down arrow is used to send immediate shutdown signal (end all threads) when not in input mode
			m_shutDownFlag = true;
			m_exeControl->sendShutDownSignal();
			break;
		case MOWING:
			m_shutDownFlag = true;
			m_exeControl->sendShutDownSignal();
			break;
		case INPUT_LENGTH:
			m_inputLength -= 1;
			break;
		case INPUT_WIDTH:
			m_inputWidth -= 1;
			break;
		case PAUSED:
			m_shutDownFlag = true;
			m_exeControl->sendShutDownSignal();
			break;
		default:
			m_errorNum = -2;
			return -2;
	}

	return 0;
}

void ButtonController::goodbyeScreen() {
	drawText(calcX("NoMo Lawn: GOODBYE :)"), 32, "NoMo Lawn: GOODBYE :)", 1, "IDLE");
	displayTemp();	
}

void ButtonController::welcomeScreen() {
	drawText(calcX("NoMo Lawn: WELCOME :)"), 32, "NoMo Lawn: WELCOME :)", 1, "IDLE");
	displayTemp();
}

void ButtonController::idleScreen() {
	drawText(calcX("CURRENTLY IDLE"), 32, "CURRENTLY IDLE", 1, "IDLE");
	drawText(calcX("PRESS START!"), 32 + 8, "PRESS START!", 1, "IDLE");
	displayTemp();
}

void ButtonController::mowingScreen() {
	drawText(calcX("CURRENTLY MOWING!"), 32, "CURRENTLY MOWING!", 1, "MOWING");
	displayTemp();
}

void ButtonController::pausedScreen() {
	drawText(calcX("CURRENTLY PAUSED!"), 32, "CURRENTLY PAUSED!", 1, "PAUSED");
	displayTemp();
}

void ButtonController::lwInputMode(char* val, int decider, State state) {
	int y = 32; 										// halfway between top and bottom (64 pixels high)
	int vertOffset = 8; 								// 8 pixels will give nice looking spacing for multi-line text
	int msDelay = 2000; 								// delay in between changing display text

	char* s1 = "";
	char s1b[20];

	if (decider == 1){
		s1 = "Length Input Mode!";
		strcpy(s1b, "Length: ");
	} else if (decider == 2){
		s1 = "Width Input Mode!";
		strcpy(s1b, "Width: ");
	}

	strcat(s1b, val);												// s1b += val

	if (state == INPUT_LENGTH) {
		drawText(calcX(s1), y, s1, 1, "INPUT_LENGTH"); 							//print l/w input mode
		drawText(calcX(s1b), y+vertOffset+vertOffset, s1b, 1, "INPUT_LENGTH");	//print len/wid curr @ 
	} else if (state == INPUT_WIDTH) {
		drawText(calcX(s1), y, s1, 1, "INPUT_WIDTH"); 							//print l/w input mode
		drawText(calcX(s1b), y+vertOffset+vertOffset, s1b, 1, "INPUT_WIDTH");	//print len/wid curr @ 
	}

	displayTemp();
}

void ButtonController::displayTemp() {
	ssd1306_display();												// actually display
	ssd1306_clearDisplay();											// clear display
}

void ButtonController::staticDisplays(char* state) {
	char final[50];
	strcpy(final, "STATE:");
	strcat(final, state);

	int x = 0;
 	char* c = final;

	for (c = final; *c != '\0'; c++) {
		ssd1306_drawChar(x, 0, *c, WHITE, 1);
		x += 6;
	}
}

int ButtonController::calcX(char* str) {
	char* c = str;
	int count = 0;

	for(c = str; *c != '\0'; c++){
		count++;
	}

	return (64 - ((count / 2) * 6));
}

void ButtonController::drawText(int x, int y, char* s, int size, char* state) {
	staticDisplays(state);

	char* c = s;

	for (c = s; *c != '\0'; c++) {
		ssd1306_drawChar(x, y, *c, WHITE, size);
		x += 6;
	}
}
