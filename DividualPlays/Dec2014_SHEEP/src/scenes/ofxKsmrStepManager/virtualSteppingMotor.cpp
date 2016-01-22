//
//  virtualSteppingMotor.cpp
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#include "virtualSteppingMotor.h"

virtualSteppingMotor::virtualSteppingMotor(string name,int numStep,int SPICh){

	Devname			= name;
	numStp			= numStep;
	mySPICh			= SPICh;
	currentStp		= 0;
	offset			= 0;
	sendEnable		= false;

}

void virtualSteppingMotor::move(int step,bool dir){

	currentStp += step * (dir ? -1.0 : 1.0);

}

void virtualSteppingMotor::go_to(int pos){

	currentStp = pos;

}

void virtualSteppingMotor::absPos(int pos){

	offset = currentStp + pos;

}

int virtualSteppingMotor::getCurrentStep(){
	return offset + currentStp;
}

float virtualSteppingMotor::getCurrentDegree(){
	return currentStp * (360.0 / numStp);
}

//==============

void virtualSteppingMotor::checkcurrentStp(){

	while (currentStp < 0)
		currentStp += numStp;

	currentStp %= numStp;

}
