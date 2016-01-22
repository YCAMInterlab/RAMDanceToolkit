//
//  dpScoreUniqueStack.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/15.
//
//

#ifndef __dpScore__dpScoreUniqueStack__
#define __dpScore__dpScoreUniqueStack__

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

template<class T>
class UniqueStack {
public:
    typedef vector<T> List;
    typedef typename vector<T>::iterator ListIt;
    
    void setInitialList(const List& initialList);
    const List& getInitialList() const { return mInitialList; }
    
    size_t size() const;
    bool empty() const;
    
    void reset();
    T get(int index);
    const T& ref(int index);
    
    ListIt begin();
    ListIt end();
    ListIt find(const T& t);
    
private:
    List mInitialList;
    List mUniqueList;
};

template<class T>
void UniqueStack<T>::setInitialList(const List& initialList)
{
    mInitialList = initialList;
    reset();
}

template<class T>
size_t UniqueStack<T>::size() const
{
    return mUniqueList.size();
}

template<class T>
bool UniqueStack<T>::empty() const
{
    return mUniqueList.empty();
}

template<class T>
void UniqueStack<T>::reset()
{
    mUniqueList = mInitialList;
}

template<class T>
T UniqueStack<T>::get(int index)
{
    if (index < 0 || index >= mUniqueList.size()) {
        assert(false);
        ofxThrowExceptionf(ofxException, "index %d out of range", index);
    }
    const T t{mUniqueList.at(index)};
    mUniqueList.erase(mUniqueList.begin() + index);
    
    if (mUniqueList.empty()) reset();
    
    return t;
}

template<class T>
const T& UniqueStack<T>::ref(int index)
{
    if (index < 0 || index >= mUniqueList.size()) {
        assert(false);
        ofxThrowExceptionf(ofxException, "index %d out of range", index);
    }
    return mUniqueList.at(index);
}

template<class T>
typename UniqueStack<T>::ListIt UniqueStack<T>::begin()
{
    return mUniqueList.begin();
}

template<class T>
typename UniqueStack<T>::ListIt UniqueStack<T>::end()
{
    return mUniqueList.end();
}

template<class T>
typename UniqueStack<T>::ListIt UniqueStack<T>::find(const T& t)
{
    return std::find(mUniqueList.begin(), mUniqueList.end(), t);
}

typedef UniqueStack<string> UniqueStringStack;
typedef UniqueStack<int> UniqueIntStack;
typedef UniqueStack<float> UniqueFloatStack;

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreUniqueStack__) */
