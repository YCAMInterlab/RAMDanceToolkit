//
//  dpPhongShading.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/04.
//
//

#include "dpPhongShading.h"
dpPhongShading* dpPhongShading::__instance = NULL;

void dpPhongShading::setup(){
#define _S(src) #src
    
    const char *vs = _S(
                        
                        varying vec3 normal;
                        varying vec4 worldPos;
                        
                        void main(){
                            gl_Position = ftransform();
                            worldPos = gl_ModelViewMatrix * gl_Vertex;
                            
                            normal = gl_NormalMatrix * gl_Normal;
                            gl_FrontColor = gl_Color;
                        }
                        );
    
    const char *fs = _S(
                        
                        varying vec3 normal;
                        varying vec4 worldPos;
                        uniform vec3 light;
                        
                        uniform float specular;
                        
                        vec3 ads(){
                            vec3 n = normalize(normal.xyz);
                            vec3 s = normalize(light);//-v_Vertex.xyz);
                            vec3 v = normalize(vec3(-worldPos));
                            vec3 r = reflect(-s,-n);
                            
                            if(dot(n,s) > 0.0)return vec3(max(dot(s,n), 0.0) + vec3(pow(max(dot(r,v),0.0),specular)) * 1.0) * 1.0;
                            else return vec3(0,0,0);
                        }
                        
                        void main(){
                            gl_FragColor = vec4(gl_Color.rgb * ads(),1.0);
                        }
                        );
    
#undef _S
    
    mShader.setupShaderFromSource(GL_VERTEX_SHADER, vs);
    mShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fs);
    mShader.linkProgram();
    
}


void dpPhongShading::begin(){
    mShader.begin();
    mShader.setUniform3f("light", 0, 0, 100);
    mShader.setUniform1f("specular", 10.0);
}

void dpPhongShading::begin(ofPoint lightPos,float spec){
    mShader.begin();
    mShader.setUniform3fv("light", &lightPos.x);
    mShader.setUniform1f("specular", spec);
}

void dpPhongShading::end(){
    mShader.end();
}
