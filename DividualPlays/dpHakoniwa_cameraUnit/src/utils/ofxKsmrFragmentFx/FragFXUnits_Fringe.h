//
//  FragFXUnits_Fringe.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/06.
//
//

#ifndef __no_1_fxGenerator__FragFXUnits_Fringe__
#define __no_1_fxGenerator__FragFXUnits_Fringe__

#include "ofxKsmrFragFXUnit.h"

class FragFXUnits_Fringe : public ofxKsmrFragFXUnit{

public:
	virtual void setup(){
		Name = "Fringe";

		u_Volume = 0.01;

		source = STRINGIFY(
						   uniform float Volume;
						   uniform sampler2DRect tex0;
						   uniform float ScaleW;
						   uniform float ScaleH;

						   void main(void){
							   vec2 center = vec2(ScaleW/2.0, ScaleH/2.0);
							   vec2 st = gl_TexCoord[0].st;
							   vec2 rev = (center - st) * Volume;

							   vec3 col;
							   vec3 col_fringe;

							   col = texture2DRect(tex0, vec2(st.x,st.y)).rgb;
							   col_fringe.r = texture2DRect(tex0, vec2(st.x,st.y) + rev).r;
							   col_fringe.b = texture2DRect(tex0, vec2(st.x,st.y) - rev).b;
							   col_fringe.g = 0.0;

							   col.r = col_fringe.r;
							   col.b = col_fringe.b;

							   gl_FragColor = vec4(col,1.0);
						   }
						   
						   );
	}

};

#endif /* defined(__no_1_fxGenerator__FragFXUnits_Fringe__) */
