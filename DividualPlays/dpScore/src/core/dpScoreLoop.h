//
//  dpScoreLoop.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreLoop_h
#define dpScoreLoop_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

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

DP_SCORE_NAMESPACE_END

#endif /* dpScoreLoop_h */
