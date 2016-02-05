//
//  dpScorePP.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/5/16.
//
//

#ifndef dpScorePP_h
#define dpScorePP_h

#define NAMESPACE_BEGIN(name) namespace name {
#define NAMESPACE_END }

#ifndef STRINGIFY
#define STRINGIFY(A)  #A
#endif

#define GLSL_SOURCE(version, shader)  "#version " #version "\n" #shader

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))
#endif

#define DP_SCORE_NAMESPACE_BEGIN namespace dp { namespace score {
#define DP_SCORE_NAMESPACE_END } }

#define dpDebugFunc() ofLogVerbose() << getClassName(*this) << ", " << __FUNCTION__

#endif /* dpScorePP_h */
