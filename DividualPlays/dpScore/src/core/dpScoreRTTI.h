//
//  dpScoreRTTI.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#ifndef dpScoreRTTI_h
#define dpScoreRTTI_h

#include "dpScorePP.h"

DP_SCORE_NAMESPACE_BEGIN

string demangle(const char* name);

template <class T> string getClassName(const T& t);
template <class T> string getClassName();

template <class T>
string getClassName(const T& t)
{
	return demangle(typeid(t).name());
}

template <class T>
string getClassName()
{
	return demangle(typeid(T).name());
}

DP_SCORE_NAMESPACE_END

#endif /* dpScoreRTTI_h */
