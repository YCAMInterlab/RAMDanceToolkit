//
//  FragFXUnits_Invert.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/07.
//
//

#ifndef no_1_fxGenerator_FragFXUnits_Invert_h
#define no_1_fxGenerator_FragFXUnits_Invert_h

#include "ofxKsmrFragFXUnit.h"

class FragFXUnits_Invert : public ofxKsmrFragFXUnit{

public:
	virtual void setup(){
		Name = "Invert";
		u_Volume = 1.0;

		source = STRINGIFY(
						   uniform float Volume;
						   uniform float Phase;
						   uniform sampler2DRect tex0;

						   void main(void){
							   vec2 st = gl_TexCoord[0].st;
							   vec3 col;

							   col.r = texture2DRect(tex0, vec2(st.x,st.y)).r;
							   col.b = texture2DRect(tex0, vec2(st.x,st.y)).b;
							   col.g = texture2DRect(tex0, vec2(st.x,st.y)).g;

							   col.r += ((1.0 - col.r) - col.r) * Volume;
							   col.g += ((1.0 - col.g) - col.g) * Volume;
							   col.b += ((1.0 - col.b) - col.b) * Volume;

							   gl_FragColor = vec4(col,1.0);
						   }
						   
						   );
	}
};


#endif
