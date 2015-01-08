//
//  ofxKsmrFragmentFx.cpp
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/06.
//
//

#include "ofxKsmrFragmentFx.h"

void ofxKsmrFragmentFx::setup(ofFbo* buf, ofFbo::Settings setting){

	sourceBuf = buf;
	buffer[0].allocate(setting);
	buffer[1].allocate(setting);

	fxUnits.push_back(new FragFXUnits_Fringe());
	fxUnits.push_back(new FragFXUnits_EdgeOnTop());
	fxUnits.push_back(new FragFXUnits_verticalNoise());
	fxUnits.push_back(new FragFXUnits_SlantShift());
	fxUnits.push_back(new FragFXUnits_TexChip());
	fxUnits.push_back(new FragFXUnits_Noise());
	fxUnits.push_back(new FragFXUnits_Invert());
	fxUnits.push_back(new FragFXUnits_verticalSlide());

	for (int i = 0;i < fxUnits.size();i++){
		fxUnits[i]->setup();
		fxUnits[i]->linkProgram();
		fxUnits[i]->u_BufScaleW = setting.width;
		fxUnits[i]->u_BufScaleH = setting.height;
	}
}

void ofxKsmrFragmentFx::applyFx(){

	buffer[0].begin();
	sourceBuf->draw(0,0);
	buffer[0].end();

	srcPtr = &buffer[0];
	dstPtr = &buffer[1];

	for (int i = 0;i < fxUnits.size();i++){

		if (fxUnits[i]->bEnable){
			dstPtr->begin();

			fxUnits[i]->shader.begin();
			fxUnits[i]->shader.setUniformTexture("tex0", srcPtr->getTextureReference(), 0);
			fxUnits[i]->shader.setUniform1f("Volume", fxUnits[i]->u_Volume);
			fxUnits[i]->shader.setUniform1f("ScaleW", fxUnits[i]->u_BufScaleW);
			fxUnits[i]->shader.setUniform1f("ScaleH", fxUnits[i]->u_BufScaleH);
			fxUnits[i]->shader.setUniform1f("Phase", ofGetElapsedTimef());

			glBegin(GL_TRIANGLE_STRIP);

			glTexCoord2d(0.0, 0.0);
			glVertex2d(0.0, 0.0);

			glTexCoord2d(buffer[0].getWidth(), 0.0);
			glVertex2d(buffer[0].getWidth(), 0.0);

			glTexCoord2d(0.0, buffer[0].getHeight());
			glVertex2d(0.0, buffer[0].getHeight());

			glTexCoord2d(buffer[0].getWidth(), buffer[0].getHeight());
			glVertex2d(buffer[0].getWidth(), buffer[0].getHeight());

			glEnd();
			
			fxUnits[i]->shader.end();

			dstPtr->end();

			swap(srcPtr, dstPtr);
		}

	}

	sourceBuf->begin();

	srcPtr->draw(0,0);

	sourceBuf->end();
}

ofxKsmrFragFXUnit* ofxKsmrFragmentFx::getfxUnit(KsmrFragFx_Type type){
	return fxUnits[type];
}