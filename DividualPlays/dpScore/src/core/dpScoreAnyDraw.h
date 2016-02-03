//
//  dpScoreAnyDraw.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#ifndef dpScoreAnyDraw_h
#define dpScoreAnyDraw_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

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

DP_SCORE_NAMESPACE_END

#endif /* dpScoreAnyDraw_h */
