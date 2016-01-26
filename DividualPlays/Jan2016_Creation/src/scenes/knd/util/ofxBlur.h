#pragma once

#include "ofMain.h"

class ofxBlur {
protected:
	ofFbo base;
	vector<ofFbo> ping, pong;

	ofShader blurShader, combineShader;
	float scale, rotation;
	float downsample;
	float brightness;
public:
	ofxBlur();

	void setup(int width, int height, int radius = 32, float shape = .2, int passes = 1, float downsample = .5);

	void setScale(float scale);
	void setRotation(float rotation);
	void setBrightness(float brightness); // only applies to multipass

	void begin();
	void end();
	void draw();
    void draw(ofRectangle rect);

	ofTexture& getTextureReference();
};

// <3 kyle
