/*
 * FixLenthSpeedBuffer.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: j.maestra
 */

#include "FixLenthSpeedBuffer.h"

FixLenthSpeedBuffer::FixLenthSpeedBuffer() {
	this->first = 0U;
}

FixLenthSpeedBuffer::~FixLenthSpeedBuffer() {
	this->first = 0U;
}

void FixLenthSpeedBuffer::add(float newOne)
{
	this->ringBuffer[this->first++] = newOne;
	if(this->first == MAX_RING_LENGTH)
	{
		this->first = 0;
	}
}

using namespace std;

float FixLenthSpeedBuffer::lastNCoef(unsigned short number)
{
	if(number > MAX_RING_LENGTH/2) return 0.0;
	float temp = 0.0;

	int primero = 0;
	int segundo = 0;

	for(int i=0; i<number; i++)
	{
		primero = (this->first - (i + 1))>=0?(this->first - (i + 1)):MAX_RING_LENGTH -  ((i + 1) - this->first);
		segundo = (primero - 1)>=0?(primero - 1):MAX_RING_LENGTH - 1;
		temp = temp + (this->ringBuffer[primero] - this->ringBuffer[segundo]);
	}
	return temp;
}
