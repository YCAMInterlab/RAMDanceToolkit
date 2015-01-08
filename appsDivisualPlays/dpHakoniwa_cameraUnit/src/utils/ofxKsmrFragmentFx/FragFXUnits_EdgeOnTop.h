//
//  FragFXUnits_EdgeOnTop.h
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/12.
//
//

#ifndef no_1_fxGenerator_FragFXUnits_EdgeOnTop_h
#define no_1_fxGenerator_FragFXUnits_EdgeOnTop_h

#include "ofxKsmrFragFXUnit.h"

class FragFXUnits_EdgeOnTop : public ofxKsmrFragFXUnit{

public:
	virtual void setup(){
		Name = "EdgeOnTop";
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
							   vec3 max = vec3(0.0,0.0,0.0);
							   vec3 min = vec3(1.0,1.0,1.0);

							   int i = 0;
							   int j = 0;
							   for (i; i < 3;i++){
								   for (j; j < 3;j++){
									   vec4 c = texture2DRect(tex0, vec2(st.x + float(i) - 1.0,
																		 st.y + float(j) - 1.0));

									   max.r = max(max.r, c.r);
									   min.r = min(min.r, c.r);
									   max.g = max(max.g, c.g);
									   min.g = min(min.g, c.g);
									   max.b = max(max.b, c.b);
									   min.b = min(min.b, c.b);

								   }
							   }

							   col = texture2DRect(tex0, vec2(st.x, st.y));

							   vec3 br = vec3(pow(max.r - min.r,2.0),
											  pow(max.g - min.g,2.0),
											  pow(max.b - min.b,2.0));

							   gl_FragColor = vec4(br,1.0) + col;
						   }
						   
						   );
	}
};


#endif
