//
//  CounterFireVector.h
//  Graph
//
//  Created by kezzardrix2 on 2015/06/25.
//
//

#ifndef Graph_CounterFireVector_h
#define Graph_CounterFireVector_h

template <typename T>
class CounterFireVector{
public:
    
    void push_backObjs(int num){
        for(int i = 0; i < num; i++){
            mObjs.push_back(T());
        }
    }
    
    void setup(){
        for(auto &v:mObjs){
            v.setup();
        }
    }
    
    void update(){
        for(auto &v:mObjs){
            v.update();
        }
    }
    
    void draw(){
        for(auto &v:mObjs){
            v.draw();
        }
    }
    
    void randomFire(){
        mObjs[(int)ofRandom(0,mObjs.size())].fire();
    }
    
    void fire(){
        mObjs[mCounter].fire();
        countUp();
    }
    
    void fire(float val){
        mObjs[mCounter].fire(val);
        countUp();
    }
   
    void fire(ofPoint pos){
        mObjs[mCounter].fire(pos);
        countUp();
    }
    
    void multiFire(int num){
        for(int i = 0; i < num; i++){
            fire();
        }
    }
    
    template<typename P>
    void fire(P p){
        mObjs[mCounter].fire(p);
        countUp();
    }
    
    T & getCurrentObj(){
        return mObjs.at(mCounter);
    }
    
    T & at(int i){
        return mObjs.at(i);
    }
    
    vector<T> & getObjs(){
        return mObjs;
    }
    
    int getCurrent(){
        return mCounter;
    }
    
    vector<T>mObjs;
    
private:
    int mCounter = 0;
    
    void countUp(){
        mCounter++;
        mCounter %= mObjs.size();
    }
};

#endif
