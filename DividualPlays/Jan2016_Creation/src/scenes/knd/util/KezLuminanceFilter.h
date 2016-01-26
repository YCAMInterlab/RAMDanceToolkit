//
//  KezLuminanceFilter.h
//  AccumlateBufferTest
//
//  Created by kezzardrix2 on 2016/01/25.
//
//

#ifndef AccumlateBufferTest_KezLuminanceFilter_h
#define AccumlateBufferTest_KezLuminanceFilter_h

#define STRINGIFY(A) #A

class KezLuminanceFilter{
public:
    void setup(){
        string frag = STRINGIFY(
                                
                                uniform sampler2DRect src;
                                uniform sampler2DRect buffer;
                                uniform vec3 dpColor;
                                uniform float thresh;
                                
                                float colorToLumi(in vec3 color){
                                    return dot(color.rgb,vec3(0.2126,0.7152,0.0722));
                                }
                                
                                void main(void){
                                    
                                    vec2 st = gl_TexCoord[0].st;
                                    
                                    vec4 srcColor = texture2DRect(src, st);
                                    vec4 bufferColor = texture2DRect(buffer, st);
                                    
                                    srcColor -= bufferColor;
                                    
                                    float lumi = colorToLumi(srcColor.rgb);
                                    
                                    lumi = pow(lumi,1.0);
                                    
                                    if(lumi < thresh)discard;
                                    
                                    gl_FragColor = vec4(vec3(lumi) * dpColor / 255.0,1.0);
                                    
                                }
                        );
        
        mShader.setupShaderFromSource(GL_FRAGMENT_SHADER,frag);
        mShader.linkProgram();
    }
    
    void setThresh(float thresh){
        mThresh = thresh;
    }
    
    void begin(ofTexture &buffer){
        mShader.begin();
        mShader.setUniformTexture("buffer", buffer, 1);
        mShader.setUniform3f("dpColor",dpColor::MAIN_COLOR.r,dpColor::MAIN_COLOR.g,dpColor::MAIN_COLOR.b);
        mShader.setUniform1f("thresh",mThresh);
    }
    
    void end(){
        mShader.end();
    }
    
private:
    ofShader mShader;
    float mThresh = 0.12;
};

#endif
