//
//  FragFXUnits_verticalSlide.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/12.
//
//

#ifndef no_1_fxGenerator_FragFXUnits_verticalSlide_h
#define no_1_fxGenerator_FragFXUnits_verticalSlide_h

#include "ofxKsmrFragFXUnit.h"

class FragFXUnits_verticalSlide : public ofxKsmrFragFXUnit{

public:
	virtual void setup(){
		Name = "VerticalSlide";
		u_Volume = 1.0;

		source = STRINGIFY(
						   uniform float Volume;
						   uniform float Phase;
						   uniform float ScaleW;
						   uniform float ScaleH;

						   uniform sampler2DRect tex0;

						   void main(void){
							   vec2 st = gl_TexCoord[0].st;
							   vec4 col;

							   col = texture2DRect(tex0, vec2(st.x,mod(st.y+Phase*Volume,ScaleH)));

							   gl_FragColor = vec4(col);
						   }
						   
						   );
	}
};


#endif
