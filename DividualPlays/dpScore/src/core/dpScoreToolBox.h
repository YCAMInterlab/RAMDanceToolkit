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

#pragma mark AnyDraw
#pragma mark ___________________________________________________________________

class AnyDraw {
public:
    template < typename DrawerType>
    AnyDraw(const DrawerType& drawer)
    : mContent(new Holder<DrawerType>(drawer))
    {
    }
    
    AnyDraw(const AnyDraw& other)
    : mContent(other.mContent.get() ? other.mContent->clone() : nullptr)
    {
    }
    
    AnyDraw& operator = (const AnyDraw& rhs)
    {
        if (this != &rhs) {
            mContent.reset(rhs.mContent->clone());
        }
        return *this;
    }
    
    template <typename DrawerType>
    AnyDraw& operator = (const AnyDraw& rhs)
    {
        mContent.reset(new Holder<DrawerType>(rhs));
        return *this;
    }
    
    void draw()
    {
        mContent->draw();
    }
    
private:
    class Placeholder {
    public:
        virtual ~Placeholder() {}
        virtual Placeholder * clone() = 0;
        virtual void draw() = 0;
    } ;
    
    template <typename DrawerType>
    class Holder : public Placeholder {
    public:
        explicit Holder(DrawerType const& drawer)
        : mHeld(drawer)
        {
        }
        
        virtual Placeholder* clone()
        {
            return new Holder(mHeld);
        }
        
        virtual void draw()
        {
            mHeld.draw();
        }
    private:
        DrawerType mHeld;
    };
    
    std::auto_ptr<Placeholder> mContent;
};

#pragma mark AnyDraw
#pragma mark ___________________________________________________________________

// no r-value references
template <typename T>
inline shared_ptr<T> makeShared()
{
    return shared_ptr<T>(new T());
}


DP_SCORE_NAMESPACE_END

#endif /* dpScoreToolBox_h */
