//
//  dpCameraUnit_ShaderFX.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_ShaderFX.h"

dpCameraUnit_ShaderFX::dpCameraUnit_ShaderFX(){

	ofFbo::Settings settings;
	settings.width  = buffer_width;
	settings.height = buffer_height;

	mBuffer.allocate(settings);
	mFx.setup(&mBuffer, settings);

	gui.setup();
	gui.addLabel("ShaderFX",OFX_UI_FONT_LARGE);

	gui.addToggle("EdgeOnTop", &mFx.getfxUnit(KSMR_FRAGFX_EDGEONTOP)->bEnable);
	gui.addToggle("Fringe", &mFx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable);
	gui.addToggle("Invert", &mFx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable);
	gui.addToggle("noise", &mFx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable);
	gui.addToggle("SlangShift", &mFx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable);
	gui.addToggle("TexChip", &mFx.getfxUnit(KSMR_FRAGFX_TEXCHIP)->bEnable);

	gui.autoSizeToFitWidgets();

}

dpCameraUnit_ShaderFX::~dpCameraUnit_ShaderFX(){

}

void dpCameraUnit_ShaderFX::update(ofImage& pix){

	mBuffer.begin();
	ofClear(0, 0, 0, 255);
	ofSetColor(255);
	pix.draw(0,0,mBuffer.getWidth(), mBuffer.getHeight());

	mBuffer.end();

	mFx.applyFx();

}

void dpCameraUnit_ShaderFX::draw(int x,int y){
	gui.setPosition(x, y);

	ofPushMatrix();
	ofTranslate(x, y);

	ofSetColor(255);
	mBuffer.draw(100,0,mBuffer.getWidth()/4.0, mBuffer.getHeight()/4.0);

	ofPopMatrix();
}