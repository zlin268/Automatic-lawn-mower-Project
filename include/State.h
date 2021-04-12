/**
 *
 * This file contains an enumeration which holds the various states that the device can be in from start to finish
 * The idea is to mirror the concept of a finite state machine where there are start, executing and finished/accepting states
 *
 */

#ifndef STATE_H
#define	STATE_H

/**
 *
 * Idle: Start state: machine is waiting to either start or begin input
 * Input_Length & Input_Width: machine is accepting input from the user to input dimensions of the path
 * Mowing: machine is currently mowing (non accepting state) and can either: stop (go to idle), pause or finish mowing (accepting)
 * Paused: Machine is currently paused and will resume mowing when the blue button is pressed
 */
enum State {
	IDLE,
	MOWING,
	INPUT_LENGTH,
	INPUT_WIDTH,
	PAUSED
};

#endif // STATE_H

