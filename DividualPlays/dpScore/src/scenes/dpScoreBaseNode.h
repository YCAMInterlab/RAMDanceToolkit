//
//  dpScoreBaseNode.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/23/16.
//
//

#ifndef dpScoreBaseNode_h
#define dpScoreBaseNode_h

#include "dpScoreCommon.h"

DP_SCORE_NAMESPACE_BEGIN

class BaseNode: public ofNode {
public:
    BaseNode();
    virtual ~BaseNode() = default;
    
    virtual void customDraw() override;

    size_t getNumAimingPositions() const;
    ofVec3f getAimingPosition(size_t idx) const;
    void addAimingOffset(const ofVec3f& offset);
    void clearAimingOffsets();

    ofCamera& getCamera();
    const ofCamera& getCamera() const;

	float t {0.f};
	string title;
	string titleJP;
	string description;
	string descriptionJP;
    
    weak_ptr<BaseNode> parent;
    weak_ptr<BaseNode> child;

protected:
	vector<ofVec3f> mOffsets;
	ofCamera mCamera;
};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreBaseNode_h */
