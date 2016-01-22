//
//  ofxKsmrFragmentFx.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/06.
//
//

#ifndef __no_1_fxGenerator__ofxKsmrFragmentFx__
#define __no_1_fxGenerator__ofxKsmrFragmentFx__

#include "ofMain.h"

#include "FragFXUnits_Fringe.h"
#include "FragFXUnits_verticalNoise.h"
#include "FragFXUnits_TexChip.h"
#include "FragFXUnits_SlantShift.h"
#include "FragFXUnits_noise.h"
#include "FragFXUnits_Invert.h"
#include "FragFXUnits_verticalSlide.h"
#include "FragFXUnits_EdgeOnTop.h"

enum KsmrFragFx_Type{
	KSMR_FRAGFX_FRINGE,
	KSMR_FRAGFX_EDGEONTOP,
	KSMR_FRAGFX_VERTNOISE,
	KSMR_FRAGFX_SLANTSHIFT,
	KSMR_FRAGFX_TEXCHIP,
	KSMR_FRAGFX_NOISE,
	KSMR_FRAGFX_INVERT,
	KSMR_FRAGFX_VERTSLIDE
};

class ofxKsmrFragmentFx{
public:

	void setup(ofFbo* buf, ofFbo::Settings setting);
	void update();
	void draw();

	void applyFx();

	vector<ofxKsmrFragFXUnit*> fxUnits;
	ofxKsmrFragFXUnit* getfxUnit(KsmrFragFx_Type type);

	ofFbo *sourceBuf;
	ofFbo buffer[2];

	ofFbo *srcPtr;
	ofFbo *dstPtr;
};

#endif /* defined(__no_1_fxGenerator__ofxKsmrFragmentFx__) */
