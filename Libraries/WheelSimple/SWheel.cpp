/*
 * SWhell.cpp
 *
 *  Created on: May 3, 2013
 *      Author: jose.maestra
 */

#include "SWheel.h"

SWhell::SWhell(WheelPosition_t position, uint32 timeBase) {
	this->pulseCount = 0;
	this->idNumber = FrontLeft;
	this->instantSpeed = 0.0;
	this->pulsesForTimeUnit = 0;
	this->timeBase = timeBase;
	this->pulsesCalculated = 0;
}

void SWhell::wheelStart(void)
{
}

SWhell::~SWhell() {
	// TODO Auto-generated destructor stub
}

void SWhell::onPulse(unsigned long long time)
{
microsInterval=time;
    pulseCount++;
    pulseQuarter++;
    pulsesHalf++;
    if(pulseQuarter==QUARTERWHEEL)
    {
    	pulseQuarter=0;
    	/*speed quarter based*/
    	if(this->quarterInterval<microsInterval)
    	{
    		this->microsIncrement = microsInterval - this->quarterInterval;
    	}else{
    		this->microsIncrement = 0xFFFFFFFF - this->quarterInterval + microsInterval;
    	}
    	this->quarterInterval=microsInterval;
    	this->quarterSpeed = 73500.0/(float)microsIncrement;
    }
    if(pulsesHalf==HALFWHEEL)
	{
    	pulsesHalf=0;
    	/*speed quarter based*/
    	if(this->halfInterval<microsInterval)
		{
			this->microsIncrement = microsInterval - this->halfInterval;
		}else{
			this->microsIncrement = 0xFFFFFFFF - this->halfInterval + microsInterval;
		}
    	this->halfInterval=microsInterval;
    	this->halfSpeed = 147000.0/(float)microsIncrement;
	}
	this->meanSpeed = (this->quarterSpeed + this->halfSpeed)/2;
}


void SWhell::handlerTime()
{
/* Timer based Speed calculation - counts nomber of pulses between timer triggers */
  this->pulsesCalculated = pulseCount;
  	  	  	  	  	  	  /* pulses * pulseLength/ time in seconds */
  if(pulseCount!=0)
  {
	  // When there are pulses between timers timeouts: speed = (pulseNumbers * segmentLength * miliseconds/second) / timerTimoutTimeMilliseconds
	  this->timeNoPulse = 0;
	  this->instantSpeed = (this->pulseCount * SEGMENT_LENGTH * 1000.0 /*millisecond/second*/)/(float)this->timeBase /* milliseconds */ ;
	  this->lastValidSpeed = this->instantSpeed;
  }else
  {
	  // When pulses are not received between timer timeouts, the speed must decrease each timeout it happens
	  // based on the maximum speed possible since the time last pulse received.
	  this->timeNoPulse++;
	  this->instantSpeed = ((SEGMENT_LENGTH)/( (4*this->timeNoPulse)+((this->lastValidSpeed/SEGMENTWHEEL)*16*4)/1000 ) )*1000;
	  this->quarterSpeed = this->instantSpeed/this->timeNoPulse;
          this->halfSpeed = this->instantSpeed/this->timeNoPulse;
	  /* instantSpeed =  */
  }
  /* Reset */
  this->pulseCount = 0;
}

void SWhell::timerCalcule(void)
{
}

WheelPosition_t SWhell::getIdNumber() const {
	return idNumber;
}

float SWhell::getInstantSpeed() const {
	return instantSpeed;
}

uint16 SWhell::getPulseCount() const {
	return pulseCount;
}

uint16 SWhell::getPulsesForTimeUnit() const {
	return pulsesForTimeUnit;
}

uint32 SWhell::getTimeBase() const {
	return timeBase;
}

uint16 SWhell::getPulsesCalculated() const {
	return pulsesCalculated;
}

float SWhell::getHalfSpeed() const {
	return halfSpeed;
}

float SWhell::getQuarterSpeed() const {
	return quarterSpeed;
}

float SWhell::getFiltSpeed() const {
	return (quarterSpeed + halfSpeed + instantSpeed)/3;
}
