//
//  AccumlateWeightedFilter.h
//  Ghosting
//
//  Created by kezzardrix2 on 2016/01/14.
//
//

#ifndef Ghosting_AccumlateWeightedFilter_h
#define Ghosting_AccumlateWeightedFilter_h

#include "ofxOsc.h"

#define STRINGIFY(A) #A

class KezAccumlateWeightedFilter{
public:
    
    void setup(){
        
        string frag = STRINGIFY(
                                    uniform sampler2DRect src;
                                    uniform sampler2DRect dst;
                                    uniform float srcAlpha;
                                    uniform float dstAlpha;
                                    uniform float speed;
                                
                                    vec3 rgb2hsb(vec3 c){
                                        vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
                                        vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
                                        vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
                                        
                                        float d = q.x - min(q.w, q.y);
                                        float e = 1.0e-10;
                                        return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
                                    }
                                
                                    vec3 hsb2rgb(vec3 c){
                                        vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
                                        vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
                                        return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
                                    }

                                
                                    void main(void){
                                        
                                        vec2 st = gl_TexCoord[0].st;
                                        
                                        vec3 srcColor = texture2DRect(src, st).rgb;
    
                                        vec3 dstColor = texture2DRect(dst, st + vec2(speed,0.0)).rgb;
                             
                                        vec3 hsb = rgb2hsb(dstColor);
                                        hsb.y -= 0.002;
                                        dstColor = hsb2rgb(hsb);
                                    
                                        vec3 finalColor = dstAlpha * dstColor + srcAlpha * srcColor;
                                        
                                        gl_FragColor = vec4(finalColor.rgb,1.0);
                                        
                                    }
                                );
        
        mAccumlateWeighted.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
        mAccumlateWeighted.linkProgram();
        
        mSrc.allocate(ofGetWidth(),ofGetHeight(),GL_RGB32F);
        mDst.allocate(ofGetWidth(),ofGetHeight(),GL_RGB32F);
       
        clear();
        
        mSpeed = 2.0;
        
    }
    
    void setSpeed(float speed){
        mSpeed = speed;
    }
    
    void setDstAlpha(float alpha){
        mDstAlpha = alpha;
    }
    
    void setSrcAlpha(float alpha){
        mSrcAlpha = alpha;
    }
    
    void begin(){
        mSrc.begin();
        ofClear(0);
    }
    
    void end(){
        mSrc.end();
    }
    
    void clear(){
        
        mSrc.begin();
        ofClear(0,0);
        mSrc.end();
        
        mDst.begin();
        ofClear(0,0);
        mDst.end();
    }
    
    void process(){
        
        mAccumlateWeighted.begin();
        mAccumlateWeighted.setUniform1f("dstAlpha",mDstAlpha);
        mAccumlateWeighted.setUniform1f("srcAlpha",mSrcAlpha);
        mAccumlateWeighted.setUniform1f("speed", mSpeed);
        mAccumlateWeighted.setUniformTexture("dst",mDst.getTextureReference(),1);
        
        mDst.begin();
        mSrc.draw(0,0);
        mDst.end();
        
        mAccumlateWeighted.end();

    }
    
    void update(){}
    void draw(){
        mDst.draw(0,0);
    }
private:
    
    // dstが残像
    ofFbo mDst;
    ofFbo mSrc;
    ofShader mAccumlateWeighted;
    
    float mDstAlpha = 0.99;
    float mSrcAlpha = 1.0;
    
    float mSpeed = 20.0;
};

#endif
