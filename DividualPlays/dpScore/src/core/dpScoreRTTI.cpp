//
//  dpScoreRTTI.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#include "dpScoreRTTI.h"
#include <cxxabi.h>

DP_SCORE_NAMESPACE_BEGIN

struct _handle {
    char* p;
    _handle(char* ptr) : p(ptr)
    {
    }
    ~_handle()
    {
        std::free(p);
    }
};

string demangle(const char* name)
{
    int status {-4};
    
    _handle result(abi::__cxa_demangle(name, nullptr, nullptr, &status));
    
    return (status == 0) ? result.p : name;
}

DP_SCORE_NAMESPACE_END