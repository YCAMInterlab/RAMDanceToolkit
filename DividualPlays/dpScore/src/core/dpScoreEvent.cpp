//
//  dpScoreEvent.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 1/9/15.
//
//

#include "dpScoreEvent.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark ___________________________________________________________________
void ObjectEventArgs::setAddress(const string& addr)
{
    mAddress = addr;
}

const string& ObjectEventArgs::getAddress() const
{
    return mAddress;
}

size_t ObjectEventArgs::getNumObjects() const
{
    return mObjects.size();
}

const string& ObjectEventArgs::getClassName(size_t index) const
{
    return mClassNames.at(index);
}

#pragma mark ___________________________________________________________________
ObjectEvent& objectEvent()
{
    static ObjectEvent event;
    return event;
}
void notifyObjectEvent(ObjectEventArgs& e)
{
    ofNotifyEvent(objectEvent(), e);
}

DP_SCORE_NAMESPACE_END