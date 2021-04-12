/**
 *
 * This file contains the declaration of the ButtonController class and all associated member functions and attributes.
 * The ButtonController class is used to receive button input and then depending on the button pressed + current state will do a specific action
 * This is where we used the idea of an FSM to better understand how we should react to button presses depending on the current state
 *
 */

#ifndef BUTTONCONTROLLER_H
#define	BUTTONCONTROLLER_H

#include "State.h"
#include "Path.h"
#include "ExecutionController.h"

class ButtonController {
	public:
		ButtonController(int pinStart, int pinSetDimensions, int pinUpArrow, int pinDownArrow, State& currentState, Path& path, ExecutionController& exeControl);
		~ButtonController();
		int sendButtonPress(int pinButtonPressed);
		int startInputListener();
		State getCurrentState();
		int getErrorNum();

	protected:

	private:
		State* m_currentState;
		Path* m_path;
		ExecutionController* m_exeControl;
		int m_pinStart;
		int m_pinSetDimensions;
		int m_pinUpArrow;
		int m_pinDownArrow;
		int m_inputLength;
		int m_inputWidth;
		int m_errorNum;
		bool m_shutDownFlag;

		// button press callbacks
		int	onStart();
		int onSetDimensions();
		int onUpArrow();
		int onDownArrow();

		// display functions for OLED display through i2c (based on driver implemented in C, included in "ssd1306_i2c.h")
		void goodbyeScreen();
		void welcomeScreen();
		void idleScreen();
		void mowingScreen();
		void pausedScreen();
		void lwInputMode(char* val, int decider, State state);
		void displayTemp();
		void staticDisplays(char* state);
		int calcX(char* str);
		void drawText(int x, int y, char* s, int size, char* state);
};

#endif // BUTTONCONTROLLER_H
