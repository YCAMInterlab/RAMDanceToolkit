//
//  delay.h
//  whiteBar
//
//  Created by 神田竜 on 2013/08/26.
//
//

#ifndef whiteBar_delay_h
#define whiteBar_delay_h

class KezDelay{
public:
    KezDelay(){
        limit = 0;
        delayOnce = true;
    }
    
    KezDelay(int _limit){
        limit = _limit;
        delayOnce = true;
    }
   
    bool update(){
        counter++;
        if(counter > limit && delayOnce == false){
            
            delayOnce = true;
            
            return true;
        }else{
            return false;
        }
    };
    
    void start(){
        counter = 0;
        delayOnce = false;
    }
    
    void start(int _limit){
        limit = _limit;
        counter = 0;
        delayOnce = false;
    }
    
    void stop(){
        delayOnce = true;
    }
    
    void setLimit(int _limit){
        limit = _limit;
    }
    
private:
    int counter;
    int limit;
    bool delayOnce;
};

class KezGate{
public:
    
    void update(){
        if(mDeadDelay.update()){
            mIsDead = true;
        }
        
        return mIsDead;
    }
    
    void start(int num){
        mDeadDelay.start(num);
        mIsDead = true;
    }
    
    KezDelay mDeadDelay;
    bool mIsDead = false;
    
};

#endif
