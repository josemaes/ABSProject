/*
 * FLBuffer.cpp
 *
 *  Created on: 10/5/2015
 *      Author: joseigna
 */

#include "FLBuffer.h"

FLBuffer::FLBuffer() {
	this->fullBuff = 0;
	this->writeA = 0;
	this->length = 0;
}

FLBuffer::~FLBuffer() {
	// TODO Auto-generated destructor stub
}


void FLBuffer::addString(unsigned char * string, unsigned short strLength){
	short int temp = this->length + strLength;
	if(temp<=BUFSIZE) // the info fits in the actual buffer
	{
		if(this->writeA == 0)
		{
			for (int i=this->length; i<temp; i++)
			{
				
					this->memBufferA[i] = string[i-this->length];
			}
		}else
		{
			for (int i=this->length; i<temp; i++)
			{
				
					this->memBufferB[i] = string[i-this->length];
			}
		}
	}else // info doesn´t fit in actual buffer, must write in both
	{
		if(this->writeA == 0)
		{
			temp = temp - BUFSIZE;
			int progress = 0;
			for (int i=this->length; i<BUFSIZE; i++)
			{				
				this->memBufferA[i] = string[progress++];
			}
			this->length = 0;
			for (int i=this->length; i<temp; i++)
			{	
				this->memBufferB[i] = string[progress++];
			}
			this->writeA = 1;
			this->fullBuff = 1;
		}else
		{
			temp = temp - BUFSIZE;
			int progress = 0;
			for (int i=this->length; i<BUFSIZE; i++)
			{				
				this->memBufferB[i] = string[progress++];
			}
			this->length = 0;
			for (int i=this->length; i<temp; i++)
			{	
				this->memBufferA[i] = string[progress++];
			}
			this->writeA = 0;
			this->fullBuff = 1;
		}
	}
	this->length = temp;
	if(this->writeA == 0)
	{
		if(this->length!=BUFSIZE)
			this->memBufferA[this->length] = 0;
		else
		{
			this->writeA = 1;
			this->fullBuff = 1;
			this->length = 0;
		}
	}
	else
	{
		if(this->length!=BUFSIZE)
			this->memBufferB[this->length] = 0;
		else
		{
			this->writeA = 0;
			this->fullBuff = 1;
			this->length = 0;
		}
	}
}

void FLBuffer::resetMemBuff(){
	this->length = 0;
}

short int FLBuffer::getLength() const {
	return this->length;
}

const unsigned char* FLBuffer::getMemBuffer() const {
	if (this->writeA == 0)
		return this->memBufferB;
	else
		return this->memBufferA;
}
