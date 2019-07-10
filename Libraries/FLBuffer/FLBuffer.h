/*
 * FLBuffer.h
 *
 *  Created on: 10/5/2015
 *      Author: joseigna
 */

#ifndef FLBUFFER_H_
#define FLBUFFER_H_

#define BUFSIZE 1024

class FLBuffer {
public:
	FLBuffer();
	virtual ~FLBuffer();
	void addString(unsigned char * string, unsigned short length);
	void resetMemBuff();
	short int getLength() const;
	const unsigned char* getMemBuffer() const;
	unsigned char fullBuff;
private:
	unsigned char memBufferA[BUFSIZE];
	unsigned char memBufferB[BUFSIZE];
	unsigned char writeA;
	short int length;
	
};

#endif /* FLBUFFER_H_ */
