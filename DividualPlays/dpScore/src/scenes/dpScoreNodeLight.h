//
//  dpScoreNodeLight.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/21/16.
//
//

#ifndef dpScoreNodeLight_h
#define dpScoreNodeLight_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"

DP_SCORE_NAMESPACE_BEGIN

class NodeLight: public BaseNode {
public:
	NodeLight();
	virtual ~NodeLight();

	void customDraw() override;

    constexpr static float getHeight() { return 800.f; }
    static int getNumX() { return 8; }
    static int getNumZ() { return 3; }
    
private:
    struct ParLight {
        ParLight();
        
        Box top, left, right;
        Cylinder body;
        float angle;
        ofVec3f position;
        
        void draw();
    };
    
    vector<ParLight> mParLights;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeLight_h */
