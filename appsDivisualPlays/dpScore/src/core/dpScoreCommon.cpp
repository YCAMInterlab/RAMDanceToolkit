//
//  dpScoreCommon.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 12/25/14.
//
//

#include "dpScoreCommon.h"
#include <cxxabi.h>

#ifdef USE_CUSTOM_MEMORY_ALLOCATOR
template<typename T>
struct track_alloc : std::allocator<T> {
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::size_type size_type;
    
    template<typename U>
    struct rebind {
        typedef track_alloc<U> other;
    };
    
    track_alloc() {}
    
    template<typename U>
    track_alloc(track_alloc<U> const& u)
    :std::allocator<T>(u) {}
    
    pointer allocate(size_type size, std::allocator<void>::const_pointer = 0)
    {
        void* p = std::malloc(size * sizeof(T));
        if(p == 0) {
            throw std::bad_alloc();
        }
        return static_cast<pointer>(p);
    }
    
    void deallocate(pointer p, size_type)
    {
        std::free(p);
    }
};

typedef std::map< void*, std::size_t, std::less<void*>,
track_alloc< std::pair<void* const, std::size_t> > > track_type;

struct track_printer {
    track_type* track;
    track_printer(track_type* track):track(track) {}
    ~track_printer()
    {
        for (const auto& pair : *track) {
            ofLogError() << "leaked at " << pair.first << ", " << pair.second << " bytes";
        }
    }
};

track_type* get_map()
{
    // don't use normal new to avoid infinite recursion.
    static track_type* track = new (std::malloc(sizeof *track)) track_type;
    static track_printer printer(track);
    return track;
}

void* operator new(size_t size) throw(std::bad_alloc)
{
    void* mem = malloc(size == 0 ? 1 : size);
    if (mem == nullptr) throw std::bad_alloc();
    (*get_map())[mem] = size;
    return mem;
}

void* operator new[](size_t size) throw(std::bad_alloc)
{
    void* mem = malloc(size == 0 ? 1 : size);
    if (mem == nullptr) throw std::bad_alloc();
    (*get_map())[mem] = size;
    return mem;
}

void operator delete(void* mem) throw()
{
    if(get_map()->erase(mem) == 0) {
        // this indicates a serious bug
        ofLogError() << "memory at " << mem << " wasn't allocated by us";
    }
    free(mem);
    mem = nullptr;
}

void operator delete[](void* mem) throw()
{
    if(get_map()->erase(mem) == 0) {
        // this indicates a serious bug
        ofLogError() << "memory at " << mem << " wasn't allocated by us";
    }
    free(mem);
    mem = nullptr;
}
#endif

DP_SCORE_NAMESPACE_BEGIN

const int kW = 1920;
const int kH = 1080;

const int kFrameRate = 30;
const int kOscClientPort = 10000;

const string kOscAddrPendulumVec2 = "/dp/cameraUnit/pendulum/vector";

const string kAddrVec2 = "/dp/score/vec2";
const string kAddrVec2Arr = "/dp/score/vec2Arr";
const string kAddrMotioner = "/dp/score/motioner";

const string kSettingsDir = "settings/";
const string kSettingsPrefix = "scoreUI-";

struct _handle {
    char* p;
    _handle(char* ptr) : p(ptr) {}
    ~_handle() { std::free(p); }
};

string demangle(const char* name)
{    
    int status = -4;
    
    _handle result(abi::__cxa_demangle(name, nullptr, nullptr, &status));
    
    return (status==0) ? result.p : name;
}

float alignf(float f)
{
    return ::floor(f) + 0.5f;
}
        
ofVec3f randVec3f()
{
    const float phi = ofRandom((float)M_PI * 2.0f);
    const float costheta = ofRandom(-1.0f, 1.0f);
    
    const float rho = ::sqrt(1.0f - costheta * costheta);
    const float x = rho * ::cos(phi);
    const float y = rho * ::sin(phi);
    const float z = costheta;
    
    return ofVec3f(x, y, z);
}

DP_SCORE_NAMESPACE_END

OFX_MOTIONER_NAMESPACE_BEGIN
string getJointNameLower(int index)
{
    if (index==JOINT_HIPS)				return "hips";
    if (index==JOINT_ABDOMEN)			return "abdomen";
    if (index==JOINT_CHEST)				return "chest";
    if (index==JOINT_NECK)				return "neck";
    if (index==JOINT_HEAD)				return "head";
    if (index==JOINT_LEFT_HIP)			return "left hip";
    if (index==JOINT_LEFT_KNEE)			return "left knee";
    if (index==JOINT_LEFT_ANKLE)		return "left ankle";
    if (index==JOINT_LEFT_TOE)			return "left toe";
    if (index==JOINT_RIGHT_HIP)			return "right hip";
    if (index==JOINT_RIGHT_KNEE)		return "right knee";
    if (index==JOINT_RIGHT_ANKLE)		return "right ankle";
    if (index==JOINT_RIGHT_TOE)			return "right toe";
    if (index==JOINT_LEFT_COLLAR)		return "left collar";
    if (index==JOINT_LEFT_SHOULDER)		return "left shoulder";
    if (index==JOINT_LEFT_ELBOW)		return "left elbow";
    if (index==JOINT_LEFT_WRIST)		return "left wrist";
    if (index==JOINT_LEFT_HAND) 		return "left hand";
    if (index==JOINT_RIGHT_COLLAR) 		return "right collar";
    if (index==JOINT_RIGHT_SHOULDER) 	return "right shoulder";
    if (index==JOINT_RIGHT_ELBOW) 		return "right elbow";
    if (index==JOINT_RIGHT_WRIST) 		return "right wrist";
    if (index==JOINT_RIGHT_HAND)		return "right hand";
    
    return "unknown joint index";
}
OFX_MOTIONER_NAMESPACE_END