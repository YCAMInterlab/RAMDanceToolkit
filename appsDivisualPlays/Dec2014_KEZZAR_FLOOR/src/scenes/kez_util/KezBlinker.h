//
//  KezBlinker.h
//  SjQRzm
//
//  Created by kezzardrix2 on 2014/11/04.
//
//

#ifndef SjQRzm_KezBlinker_h
#define SjQRzm_KezBlinker_h

class KezBlinker{
public:
    
    void setup(int timing,int maxRepeat,bool eternal = false){
        
        assert(timing > 1);
        
        mTiming = timing;
        mMaxRepeat = maxRepeat * 2; //出る、消えるを繰り返すで１repeat
        isEternal = eternal;
    }
    
    void start(){
        mRepeat = 0;
        mCounter = 0;
        enable = true;
        hasBlinkEnd = false;
        isStop = false;
    }
    
    void stop(){
        isStop = true;
    }
    
    bool update(){
        
        if(isStop == false){
        
        if(isEternal){
            
            mCounter++;
            
            if(mCounter == mTiming){
                mCounter = 0;
                enable = !enable;
                mRepeat++;
            }
            
            return enable;
        
        }else{
        
        if(mRepeat < mMaxRepeat){
        
            mCounter++;
        
            if(mCounter == mTiming){
                mCounter = 0;
                enable = !enable;
                mRepeat++;
            }
            
        
            return enable;
            
        }else{
            
            hasBlinkEnd = true;
            return true;
    
                }
            }
        }else{
            
            return true;
            
        }
    }
    
    bool getBlinkEnd(){
        return hasBlinkEnd;
    }
    
private:
    bool enable;
    int mTiming = 2;
    int mCounter = 0;
    int mRepeat = 4;
    int mMaxRepeat = 4;
    
    bool isEternal = false;
    bool hasBlinkEnd = true;
    bool isStop = false;
};

#endif
