//
//  dpScoreMultiFrags.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreMultiFrags_h
#define dpScoreMultiFrags_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

template <typename Flags, typename T = int>
class MultiFlags {
public:
    bool isEnable(Flags flag) const;
    void enable(Flags flag);
    void disable(Flags flag);
private:
    T mFlags;
};

template <typename Flags, typename T>
bool MultiFlags<Flags, T>::isEnable(Flags flag) const
{
    return ((mFlags & flag) == flag);
}

template <typename Flags, typename T>
void MultiFlags<Flags, T>::enable(Flags flag)
{
    mFlags |= flag;
}

template <typename Flags, typename T>
void MultiFlags<Flags, T>::disable(Flags flag)
{
    mFlags ^= (mFlags & flag);
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreMultiFrags_h */
