//
//  dpHakoniwaNodeArray.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/11.
//
//

#ifndef RAMDanceToolkit_dpHakoniwaNodeArray_h
#define RAMDanceToolkit_dpHakoniwaNodeArray_h

#include "ramMain.h"

class dpHakoniwaNodeArray : public ramNodeArray{
public:
    
    dpHakoniwaNodeArray()
    {
        type = RAM_NODEARRAY_TYPE_RIGIDBODY;
        reserveNodes(1);
    }
    
    dpHakoniwaNodeArray(int nodeNum)
    {
        type = RAM_NODEARRAY_TYPE_RIGIDBODY;
        reserveNodes(nodeNum);
    }
    
    dpHakoniwaNodeArray& operator=(const ramNodeArray &copy)
    {
        this->ramNodeArray::operator=(copy);
        return *this;
    }
private:
    void reserveNodes(int num)
    {
        nodes.resize(num);
    }
};


#endif
