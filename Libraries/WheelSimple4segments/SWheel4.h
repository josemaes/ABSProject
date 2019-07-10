/*
 * Wheel4.h
	wheel 4 segments adapted to arduino STM32duino
 *
 *  Created on: May 3, 2013
 *      Author: jose.maestra
 */

#ifndef SWHEEL_H_
#define SWHEEL_H_

#include "libmaple_types.h"
#include "timer.h"
#include "wirish_types.h"

#ifdef MAPLE_IDE
#include "wirish.h"
#endif

#define maple
/*eclipse*/
/*maple*/

#ifdef eclipse
#include <stdint.h>

	#define uint8 uint8_t
	#define uint16 uint16_t
	#define uint32 uint32_t
	#define uint8 uint8_t
	#define uint8 uint8_t
#endif

#define T50MILIS         50000  /* constant for the timer -> 50 miliseconds */
#define T25MILIS         25000  /* constant for the timer -> 25 miliseconds */
#define T10MILIS         10000  /* constant for the timer -> 10 miliseconds */
#define T5MILIS          5000  /* constant for the timer -> 5 miliseconds */
#define T1MILIS          1000  /* constant for the timer -> 1 miliseconds */
#define SEGMENTWHEEL      4
#define HALFWHEEL		  2
#define QUARTERWHEEL	  1
#define WHEEL_LENGTH      0.294
#define SEGMENT_LENGTH    0.0735


typedef enum {FrontRight, FrontLeft, RearRight, RearLeft} WheelPosition_t;

class SWhell4 {
public:
	SWhell4(WheelPosition_t position, uint32 timeBase);
	~SWhell4();

	void wheelStart(void);

	WheelPosition_t getIdNumber() const;
	float getInstantSpeed() const;
	uint16 getPulseCount() const;
	uint16 getPulsesForTimeUnit() const;

	void onPulse(unsigned long long time); /* function incrementing the pulse count */
	float instantSpeedCalc();	/* function calculating the instant speed */
	uint32 getTimeBase() const;
	void handlerTime();
	void timerCalcule(void);
	uint16 getPulsesCalculated() const;
	float getHalfSpeed() const;
	float getQuarterSpeed() const;
	float getFiltSpeed() const;

	float getMeanSpeed() const {
		return meanSpeed;
	}

private:
	WheelPosition_t idNumber; /* Position of the wheel in the vehicle */
	uint32 timeBase; /* timer base time for the instant speed calculation in milliseconds */
	//Wheel segment pulse count -> for loop calculation purpose
	uint16 pulseCount;

	/* Time pulses */
	uint16 pulsesForTimeUnit;

	/* Speed Calculation variables */
	uint16 pulsesCalculated;
	uint16 pulseQuarter;
	uint16 pulsesHalf;

	unsigned long long quarterInterval;
	unsigned long long halfInterval;
	unsigned long long microsInterval;
	unsigned long long microsIncrement;
	unsigned long long timeNoPulse;

	float instantSpeed;
	float quarterSpeed;
	float halfSpeed;
	float lastValidSpeed;

	float meanSpeed;
};

#endif /* WHEEL_H_ */
