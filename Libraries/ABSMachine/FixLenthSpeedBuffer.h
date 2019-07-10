/*
 * FixLenthSpeedBuffer.h
 *
 *  Created on: Jul 30, 2015
 *      Author: j.maestra
 */

#ifndef FIXLENTHSPEEDBUFFER_H_
#define FIXLENTHSPEEDBUFFER_H_

#define MAX_RING_LENGTH 10

class FixLenthSpeedBuffer {
public:
	FixLenthSpeedBuffer();
	virtual ~FixLenthSpeedBuffer();

	void add(float newOne);

	float lastNCoef(unsigned short number);

private:
	float ringBuffer[MAX_RING_LENGTH];
	unsigned short first;
};

#endif /* FIXLENTHSPEEDBUFFER_H_ */
