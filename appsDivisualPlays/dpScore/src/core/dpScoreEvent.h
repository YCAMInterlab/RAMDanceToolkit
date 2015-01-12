//
//  dpScoreEvent.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#ifndef __dpScore__dpScoreEvent__
#define __dpScore__dpScoreEvent__

#include "ofMain.h"
#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________

class ObjectEventArgs {
public:
    void setAddress(const string& addr);
    const string& getAddress() const;
    
    template<class T> void addObject(ofPtr<T> ptr);
    template<class T> ofPtr<T> getObject(size_t index) const;
    size_t getNumObjects() const;
    
    const string& getClassName(size_t index) const;
    
private:
    string mAddress;
    vector<ofPtr<void>> mObjects;
    vector<string> mClassNames;
};


#pragma mark ___________________________________________________________________
template<class T>
void ObjectEventArgs::addObject(ofPtr<T> ptr)
{
    mClassNames.push_back(score::getClassName<T>());
    mObjects.push_back(ptr);
}

template<class T>
ofPtr<T> ObjectEventArgs::getObject(size_t index) const
{
    return static_pointer_cast<T>(mObjects.at(index));
}

#pragma mark ___________________________________________________________________
typedef ofEvent<ObjectEventArgs> ObjectEvent;

extern ObjectEvent& objectEvent();
extern void notifyObjectEvent(ObjectEventArgs& e);

template<class ListenerClass>
void registerObjectEvent(ListenerClass * listener,
                         int prio=OF_EVENT_ORDER_AFTER_APP)
{
    ofAddListener(objectEvent(),
                  listener,
                  &ListenerClass::onObjectReceived,
                  prio);
}

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreEvent__) */
