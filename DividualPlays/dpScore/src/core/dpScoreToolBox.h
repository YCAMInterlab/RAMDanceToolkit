//
//  dpScoreToolBox.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreToolBox_h
#define dpScoreToolBox_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

#pragma mark CounterIterator
#pragma mark ___________________________________________________________________

template<typename T>
class CounterIterator {
public:
    CounterIterator() : i(0) {}
    CounterIterator(T n) : i(n) {}
    
    bool operator == (const CounterIterator& rhs) const
    {
        return i == rhs.i;
    }
    
    bool operator != (const CounterIterator & rhs) const
    {
        return i != rhs.i;
    }
    
    T& operator * ()
    {
        return i;
    }
    
    CounterIterator& operator ++ ()
    {
        ++i;
        return *this;
    }
private:
    T i;
};

template<typename T>
class Loop {
public:
    Loop(T n) : i(n) {}
    
    CounterIterator<T> begin() const
    {
        return CounterIterator<T>();
    }
    
    CounterIterator<T> end() const
    {
        return CounterIterator<T>(i);
    }
    
private:
    T i;
};

template<typename T>
Loop<T> rep(T n)
{
    return Loop<T>(n);
}

inline
Loop<int> operator "" _i(unsigned long long int n)
{
    return Loop<int>(static_cast<int>(n));
}

inline
Loop<size_t> operator "" _s(unsigned long long int n)
{
    return Loop<size_t>(static_cast<size_t>(n));
}

#pragma mark MultiFlags
#pragma mark ___________________________________________________________________

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

#endif /* dpScoreToolBox_h */
