//
//  ofxKsmrFragFXUnit.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/06.
//
//

#ifndef __no_1_fxGenerator__ofxKsmrFragFXUnit__
#define __no_1_fxGenerator__ofxKsmrFragFXUnit__

#include "ofMain.h"

#define STRINGIFY(A) #A

class ofxKsmrFragFXUnit {
public:

	ofxKsmrFragFXUnit(){

	}
	~ofxKsmrFragFXUnit(){
		
	}

	virtual void setup();
	void linkProgram();

	bool		bEnable;
	ofShader	shader;
	string		source;
	string		Name;

	float u_Volume;
	float u_BufScaleW;
	float u_BufScaleH;
};

#endif /* defined(__no_1_fxGenerator__ofxKsmrFragFXUnit__) */
