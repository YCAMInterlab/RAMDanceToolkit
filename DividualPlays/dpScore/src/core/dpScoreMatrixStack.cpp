//
//  dpScoreMatrixStack.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/1/16.
//
//

#include "dpScoreMatrixStack.h"

DP_SCORE_NAMESPACE_BEGIN

MatrixStack::MatrixStack() :
	mMode(0),
	mMatrices(OF_MATRIX_TEXTURE + 1),
	mStacks(OF_MATRIX_TEXTURE + 1)
{
	for (auto& m : mMatrices) {
		m.makeIdentityMatrix();
	}
}

void MatrixStack::setMode(int mode)
{
	if (mode < 0 || mode >= mMatrices.size()) {
		ofxThrowException(ofxException, "MatrixStack out of range");
	}
	mMode = mode;
}

void MatrixStack::pushMatrix()
{
	mStacks.at(mMode).push(mMatrices.at(mMode));
}

void MatrixStack::popMatrix()
{
	mMatrices.at(mMode) = mStacks.at(mMode).top();
	mStacks.at(mMode).pop();
}

void MatrixStack::loadIdentity()
{
	currentMatrix().makeIdentityMatrix();
}

void MatrixStack::loadMatrix(const ofMatrix4x4& m)
{
	currentMatrix().set(m);
}

void MatrixStack::multMatrix(const ofMatrix4x4& m)
{
	currentMatrix().preMult(m);
}

void MatrixStack::translate(float x, float y, float z)
{
	currentMatrix().glTranslate(x, y, z);
}

void MatrixStack::translate(const ofVec3f& pos)
{
	currentMatrix().glTranslate(ofVec4f(pos.x, pos.y, pos.z, 1.f));
}

void MatrixStack::scale(float x, float y, float z)
{
	currentMatrix().glScale(x, y, z);
}

void MatrixStack::scale(const ofVec3f& scale)
{
	currentMatrix().glScale(ofVec4f(scale.x, scale.y, scale.z, 1.f));
}

void MatrixStack::rotate(float degrees, float x, float y, float z)
{
	currentMatrix().glRotate(degrees, x, y, z);
}

void MatrixStack::rotate(const ofQuaternion& quat)
{
	ofVec3f axis;
	float angle;
	quat.getRotate(angle, axis);
	if (fabsf(angle) > FLT_EPSILON) {
		rotate(angle, axis.x, axis.y, axis.z);
	}
}

class ScopedMatrixImpl {
friend class MatrixStack;
public:
virtual ~ScopedMatrixImpl()
{
	m.popMatrix();
}
protected:
explicit ScopedMatrixImpl(MatrixStack& m) :
	m(m)
{
	m.pushMatrix();
}
ScopedMatrixImpl(const ScopedMatrixImpl&) = delete;
ScopedMatrixImpl& operator =(const ScopedMatrixImpl&) = delete;
MatrixStack& m;
};

class ScopedMultMatrixImpl : public ScopedMatrixImpl {
friend class MatrixStack;
public:
virtual ~ScopedMultMatrixImpl()
{
}
private:
ScopedMultMatrixImpl(MatrixStack& m, const ofMatrix4x4& mat) :
	ScopedMatrixImpl(m)
{
	m.multMatrix(mat);
}
};

class ScopedTranslateImpl : public ScopedMatrixImpl {
friend class MatrixStack;
public:
virtual ~ScopedTranslateImpl()
{
}
private:
ScopedTranslateImpl(MatrixStack& m, const ofVec3f& v) :
	ScopedMatrixImpl(m)
{
	m.translate(v);
}
ScopedTranslateImpl(MatrixStack& m, float x, float y, float z) :
	ScopedMatrixImpl(m)
{
	m.translate(x, y, z);
}
};

class ScopedScaleImpl : public ScopedMatrixImpl {
friend class MatrixStack;
public:
virtual ~ScopedScaleImpl()
{
}
private:
ScopedScaleImpl(MatrixStack& m, const ofVec3f s) :
	ScopedMatrixImpl(m)
{
	m.scale(s);
}
ScopedScaleImpl(MatrixStack& m, float x, float y, float z) :
	ScopedMatrixImpl(m)
{
	m.scale(x, y, z);
}
};

class ScopedRotateImpl : public ScopedMatrixImpl {
friend class MatrixStack;
public:
virtual ~ScopedRotateImpl()
{
}
private:
ScopedRotateImpl(MatrixStack& m, const float degrees, float x, float y, float z) :
	ScopedMatrixImpl(m)
{
	m.rotate(degrees, x, y, z);
}
ScopedRotateImpl(MatrixStack& m, const ofQuaternion& quat) :
	ScopedMatrixImpl(m)
{
	m.rotate(quat);
}
};

ScopedMatrixPtr MatrixStack::scopedMatrix()
{
	return ScopedMatrixPtr(new ScopedMatrixImpl(*this));
}

ScopedMatrixPtr MatrixStack::scopedMultMatrix(const ofMatrix4x4& mat)
{
	return ScopedMatrixPtr(new ScopedMultMatrixImpl(*this, mat));
}

ScopedMatrixPtr MatrixStack::scopedTranslate(const ofVec3f& v)
{
	return ScopedMatrixPtr(new ScopedTranslateImpl(*this, v));
}

ScopedMatrixPtr MatrixStack::scopedTranslate(float x, float y, float z)
{
	return ScopedMatrixPtr(new ScopedTranslateImpl(*this, x, y, z));
}

ScopedMatrixPtr MatrixStack::scopedScale(const ofVec3f& s)
{
	return ScopedMatrixPtr(new ScopedScaleImpl(*this, s));
}

ScopedMatrixPtr MatrixStack::scopedScale(float x, float y, float z)
{
	return ScopedMatrixPtr(new ScopedScaleImpl(*this, x, y, z));
}

ScopedMatrixPtr MatrixStack::scopedRotate(float degrees, float x, float y, float z)
{
	return ScopedMatrixPtr(new ScopedRotateImpl(*this, degrees, x, y, z));
}

ScopedMatrixPtr MatrixStack::scopedRotate(const ofQuaternion& quat)
{
	return ScopedMatrixPtr(new ScopedRotateImpl(*this, quat));
}

ofMatrix4x4& MatrixStack::currentMatrix()
{
	return mMatrices.at(mMode);
}

std::stack<ofMatrix4x4>& MatrixStack::currentStack()
{
	return mStacks.at(mMode);
}

ofMatrix4x4& MatrixStack::getMatrix(int mode)
{
	if (mode < 0 || mode >= mMatrices.size()) {
		ofxThrowException(ofxException, "MatrixStack out of range");
	}
	return mMatrices.at(mode);
}

void MatrixStack::addStack()
{
	mStacks.push_back(std::stack<ofMatrix4x4>());
	mMatrices.push_back(ofMatrix4x4());
	mMatrices.back().makeIdentityMatrix();
}

ofMatrix4x4 MatrixStack::getModelViewProjectionMatrix()
{
	return getMatrix(OF_MATRIX_MODELVIEW) * getMatrix(OF_MATRIX_PROJECTION);
}

DP_SCORE_NAMESPACE_END