//
//  ofxKsmrFragFXUnit.cpp
//  no_1_fxGenerator
//
//  Created by Ovis aries on 2014/12/06.
//
//

#include "ofxKsmrFragFXUnit.h"

void ofxKsmrFragFXUnit::setup(){

	Name = "Basic";

	source = STRINGIFY(

					   uniform sampler2DRect tex0;

					   void main(void){
						   vec2 st = gl_TexCoord[0].st;
						   vec3 col;

						   col.r = texture2DRect(tex0, vec2(st.x,st.y)).r;
						   col.g = texture2DRect(tex0, vec2(st.x,st.y)).g;
						   col.b = texture2DRect(tex0, vec2(st.x,st.y)).b;

						   gl_FragColor = vec4(col,1.0);
					   }

				);

}

void ofxKsmrFragFXUnit::linkProgram(){
	bEnable = false;
	
	shader.unload();
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, source);
	shader.linkProgram();
}