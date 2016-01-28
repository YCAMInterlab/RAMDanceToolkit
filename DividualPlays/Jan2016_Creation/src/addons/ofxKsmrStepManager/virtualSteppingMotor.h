//
//  virtualSteppingMotor.h
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#ifndef __sheep_ramSceneTest__virtualSteppingMotor__
#define __sheep_ramSceneTest__virtualSteppingMotor__

#include "ofMain.h"

class virtualSteppingMotor{
public:

	virtualSteppingMotor(string name,int numStep,int SPICh);
	~virtualSteppingMotor(){};

	void absPos(int pos);
	void move(int step,bool dir);
	void go_to(int pos);

	float	getCurrentDegree();
	int		getCurrentStep();
	string	getName(){return Devname;};

	bool	sendEnable;

private:
	void checkcurrentStp();

	int			offset;
	int			currentStp;
	int			numStp;
	int			mySPICh;
	string		Devname;
};

#endif /* defined(__sheep_ramSceneTest__virtualSteppingMotor__) */
