//
//  KezSlidePoint.h
//  ExtractionExample
//
//  Created by Kanda Ryo on 12/04/18.
//  Copyright 2012 Kwansei Gakuin. All rights reserved.
//
#ifndef MY_SLIDEPT_H
#define MY_SLIDEPT_H

#include "ofMain.h"

class KezSlidePoint :public ofPoint{
public:
   
    float speed;
    float px;
    float py;
    float pz;
    
    bool quickSet;
    
    KezSlidePoint(){
        speed = 0.1;
        
        px = py = pz = x = y = z = 0;
        
        quickSet = false;
    };
    
    KezSlidePoint(float _x,float _y,float _z){
        speed = 0.1;
        px = _x;
        py = _y;
        pz = _z;
    };
    
    bool update(){
        x += (px - x) * speed;
        y += (py - y) * speed;
        z += (pz - z) * speed;
        
        if(quickSet){
            if(sqrt(pow(px - x,2) + pow(py - y,2) + pow(pz - z,2)) < 1.0){
                x = px;
                y = py;
                z = pz;
            }
        }
    };
    
    void randomSet(float _x,float _y,float _z){
        int rnd = ofRandom(0,2);
        
        if(rnd == 0)set(_x,_y,_z);
        else imSet(_x,_y,_z);
    }
    
    void set(float _x,float _y,float _z){
        px = _x;
        py = _y;
        pz = _z;
    };
    
    void set(float _x,float _y){
        px = _x;
        py = _y;
    };
    
    void setX(float _x){
        px = _x;
    };
    
    void setY(float _y){
        py = _y;
    };
    
    void setZ(float _z){
        pz = _z;
    };
    
    void set(ofPoint pt){
        px = pt.x;
        py = pt.y;
        pz = pt.z;
    };
    
    KezSlidePoint& operator += (const ofPoint &a){
        x += a.x;
        y += a.y;
        z += a.z;
        px += a.x;
        py += a.y;
        pz += a.z;
        return *this;
    }
    
    void imSetX(float _x){
        x = _x;
        px = _x;
    };
    
    void imSetY(float _y){
        y = _y;
        py = _y;
    };
    
    void imSetZ(float _z){
        z = _z;
        pz = _z;
    };
    
    void imSet(float _x,float _y,float _z){
        x = _x;
        y = _y;
        z = _z;
        px = _x;
        py = _y;
        pz = _z;
    };
    
    
    
    /*void imSet(ofPoint &p){
        x = p.x;
        y = p.y;
        z = p.z;
        px = p.x;
        py = p.y;
        pz = p.z;
    };*/
    
    void imSet(ofPoint p){
        x = p.x;
        y = p.y;
        z = p.z;
        px = p.x;
        py = p.y;
        pz = p.z;
    };
    
    void imSet(float _x,float _y){
        x = _x;
        y = _y;
        px = _x;
        py = _y;
    };
    
    void add(float _x,float _y,float _z){
        x += _x;
        px = x;
        y += _y;
        py = y;
        z += _z;
        pz = z;
    }
    
    void add(ofPoint pt){
        x += pt.x;
        px = x;
        y += pt.y;
        py = y;
        z += pt.z;
        pz = z;
    }
    
    void xAdd(float _val){
        x += _val;
        px = x;
    }
    
    void yAdd(float _val){
        y += _val;
        py = y;
    }
    
    void zAdd(float _val){
        z += _val;
        pz = z;
    }
    
    bool isArrived(){
        float a  = ofDist(x,y,px,py);
        if(a < 1)return true;
        else false;
    }
    
private:
    
};

class KezSlide{
public:
    
    float speed;
    float val;
    bool quickSet;
    
    KezSlide(){
        speed = 0.1;
        val = pVal = 0.0;
        quickSet = false;
    };
    
    KezSlide(float _val){
        speed = 0.1;
        val = _val;
    };
    
    void update(){
        val += (pVal - val) * speed;
        if(quickSet){
            if(pVal - val < 0.01 )val = pVal;
        }
    };
    
    KezSlide &operator +=(float a){
        val += a;
        pVal += a;
        return *this;
    }
    
    void set(float _pVal){
        pVal = _pVal;
    };
    
    void imSet(float _pVal){
        pVal = _pVal;
        val = _pVal;
    };
    
private:
    float pVal;
};

#endif