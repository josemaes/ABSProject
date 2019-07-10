/*
 * SWhell.cpp
 *
 *  Created on: May 3, 2013
 *      Author: jose.maestra
 */

#include "SWheel4.h"

SWhell4::SWhell4(WheelPosition_t position, uint32 timeBase) {
	this->pulseCount = 0;
	this->idNumber = FrontLeft;
	this->instantSpeed = 0.0;
	this->pulsesForTimeUnit = 0;
	this->timeBase = timeBase;
	this->pulsesCalculated = 0;
	this->timeNoPulse = 1000;
	this->lastValidSpeed = 0.000001;
}

void SWhell4::wheelStart(void)
{
}

SWhell4::~SWhell4() {
	// TODO Auto-generated destructor stub
}

void SWhell4::onPulse(unsigned long long time)
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


void SWhell4::handlerTime()
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
	  /* instantSpeed =  */
  }
  /* Reset */
  this->pulseCount = 0;
}

void SWhell4::timerCalcule(void)
{
}

WheelPosition_t SWhell4::getIdNumber() const {
	return idNumber;
}

float SWhell4::getInstantSpeed() const {
	return instantSpeed;
}

uint16 SWhell4::getPulseCount() const {
	return pulseCount;
}

uint16 SWhell4::getPulsesForTimeUnit() const {
	return pulsesForTimeUnit;
}

uint32 SWhell4::getTimeBase() const {
	return timeBase;
}

uint16 SWhell4::getPulsesCalculated() const {
	return pulsesCalculated;
}

float SWhell4::getHalfSpeed() const {
	return halfSpeed;
}

float SWhell4::getQuarterSpeed() const {
	return quarterSpeed;
}

float SWhell4::getFiltSpeed() const {
	return (quarterSpeed + halfSpeed + instantSpeed)/3;
}