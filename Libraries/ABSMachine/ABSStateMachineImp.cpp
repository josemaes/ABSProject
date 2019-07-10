/*
 * ABSStateMachineImp.cpp
 *
 *  Created on: 2/8/2015
 *      Author: patrisemua
 */

#include "ABSStateMachineImp.h"

ABSStateMachineImp::ABSStateMachineImp() {
	mySpeedBuffer = FixLenthSpeedBuffer();
	this->breakLevel = 90;
	this->lastSafeBreakLevel = 120;
	this->state = STATE_ATLOWSPEED;
}

ABSStateMachineImp::~ABSStateMachineImp() {
}

void ABSStateMachineImp::calulateState(float speedCurrent, float thrBrkLevel) {
	this->mySpeedBuffer.add(speedCurrent);
	switch (this->state){
	case STATE_ATLOWSPEED :
		this->breakLevel = thrBrkLevel;
		if (speedCurrent > 4.0f)
			this->state = STATE_WAITING_BRAKES;
		break;
	case STATE_WAITING_BRAKES :
		this->breakLevel = thrBrkLevel;
		if (thrBrkLevel>90)
			this->state = STATE_BREAKINGRADIOLEVEL;
		break;
	case STATE_BREAKINGRADIOLEVEL :
		if (thrBrkLevel>90)
		{
			if(speedCurrent > 4.0f)
			{
				if(this->SpeedIncrement() < -5.0)
				{
					this->breakLevel = thrBrkLevel - ((thrBrkLevel - 90) * 50/100);
					this->state = STATE_INCREASING_BREAKLEVEL;
				}else
				{
					this->breakLevel = thrBrkLevel;
					this->lastSafeBreakLevel = thrBrkLevel;
				}
			}else
			{
				this->state = STATE_ATLOWSPEED;
			}
		}else{
			this->state = STATE_WAITING_BRAKES;
		}
		break;
	case STATE_INCREASING_BREAKLEVEL :
			if (thrBrkLevel>90)
			{ // user is braking
				if(speedCurrent > 4.0f)
				{ // enough speed to improve brake performance
					if(this->SpeedIncrement() < -5.0)//(mySpeedBuffer.lastNCoef(1) < -0.25)
					{ // decreasing speed
						if (this->breakLevel >= thrBrkLevel)
						{ // autonomously braking more than radio... let's correct that
							this->breakLevel = thrBrkLevel;
							this->state = STATE_BREAKINGRADIOLEVEL;
						}else
						{ // less power than radio brake level but still decreasing too much
							this->breakLevel = this->breakLevel - ((this->breakLevel - 90) * 25/100);
						}
					}else
					{// can increase brake power
						this->breakLevel = thrBrkLevel;//this->breakLevel + ((this->breakLevel - 90) * 25/100);
					}
				}else
				{
					this->state = STATE_ATLOWSPEED;
				}
			}else{
				this->state = STATE_WAITING_BRAKES;
			}
		break;
	default :
		break;
	}

}

float ABSStateMachineImp::SpeedIncrement()
{
	return ( mySpeedBuffer.lastNCoef(4) * 50 );
}

float ABSStateMachineImp::getBreakLevel() const {
	return breakLevel;
}
