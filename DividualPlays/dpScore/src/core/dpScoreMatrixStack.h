//
//  dpScoreMatrixStack.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/1/16.
//
//

#ifndef dpScoreMatrixStack_h
#define dpScoreMatrixStack_h

#include "dpScoreCommon.h"
#include "ofxException.h"

DP_SCORE_NAMESPACE_BEGIN

typedef auto_ptr<class ScopedMatrixImpl> ScopedMatrixPtr;

class MatrixStack {
public:
	MatrixStack();

	void setMode(int mode);

	void pushMatrix();
	void popMatrix();

	void loadIdentity();

	void loadMatrix(const ofMatrix4x4& m);
	void multMatrix(const ofMatrix4x4& m);

	void translate(float x, float y, float z);
	void translate(const ofVec3f& pos);

	void scale(float x, float y, float z);
	void scale(const ofVec3f& scale);

	void rotate(float degrees, float x, float y, float z);
	void rotate(const ofQuaternion& quat);

	ScopedMatrixPtr scopedMatrix();
	ScopedMatrixPtr scopedMultMatrix(const ofMatrix4x4& mat);
	ScopedMatrixPtr scopedTranslate(const ofVec3f& v);
	ScopedMatrixPtr scopedTranslate(float x, float y, float z);
	ScopedMatrixPtr scopedScale(const ofVec3f& s);
	ScopedMatrixPtr scopedScale(float x, float y, float z);
	ScopedMatrixPtr scopedRotate(float degrees, float x, float y, float z);
	ScopedMatrixPtr scopedRotate(const ofQuaternion& quat);

#define dpScopedMatrix(m) auto __dpScopedMatrix__ = m.scopedMatrix()
#define dpScopedMultMatrix(m, mat) auto __dpScopedMultMatrix__ = m.scopedMultMatrix(mat)
#define dpScopedTranslate3fv(m, v) auto __dpScopedTranslate3fv__ = m.scopedTranslate(v)
#define dpScopedTranslate3f(m, x, y, z) auto __dpScopedTranslate3f__ = m.scopedTranslate(z, y, z)
#define dpScopedScale3fv(m, v) auto __dpScopedScale3fv__ = m.scopedScale(v)
#define dpScopedScale3f(m, x, y, z) auto __dpScopedScale3f__ = m.scopedScale(z, y, z)
#define dpScopedRotate4f(m, degrees, x, y, z) auto __dpScopedRotate4f__ = m.scopedRotate(degrees, x, y, z)
#define dpScopedRotate4fv(m, quat) auto __dpScopedRotate4fv__ = m.scopedRotate(quat)

	ofMatrix4x4& currentMatrix();
	stack<ofMatrix4x4>& currentStack();
	ofMatrix4x4& getMatrix(int mode);
	void addStack();

	ofMatrix4x4 getModelViewProjectionMatrix();

private:
	vector<stack<ofMatrix4x4> > mStacks;
	vector<ofMatrix4x4> mMatrices;
	int mMode;

};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreMatrixStack_h */
