/*
 * ABSStateMachineImp.h
 *
 *  Created on: 2/8/2015
 *      Author: patrisemua
 */

#include "FixLenthSpeedBuffer.h"

#ifndef ABSSTATEMACHINEIMP_H_
#define ABSSTATEMACHINEIMP_H_

#define STATE_ATLOWSPEED 0
#define STATE_WAITING_BRAKES 1
#define STATE_BREAKINGRADIOLEVEL 2
#define STATE_INCREASING_BREAKLEVEL 3

class ABSStateMachineImp {
public:
	ABSStateMachineImp();
	virtual ~ABSStateMachineImp();

	void calulateState(float speedCurrent, float thrBrkLevel);
	float SpeedIncrement();
	float getBreakLevel() const;
	float getLastSafeBrakeLevel(){return this->lastSafeBreakLevel;}
	
	unsigned char state;
private:
	
	float lastSafeBreakLevel;
	FixLenthSpeedBuffer mySpeedBuffer;

	float breakLevel;
};

#endif /* ABSSTATEMACHINEIMP_H_ */
