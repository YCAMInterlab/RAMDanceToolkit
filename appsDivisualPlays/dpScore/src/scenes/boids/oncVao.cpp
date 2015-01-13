//
//  oncVao.cpp
//  one
//
//  Created by Onishi Yoshito on 6/1/13.
//
//

#pragma mark - includs
#include "oncVao.h"
#include "ofLog.h"

#pragma mark - namespace onc
namespace onc {
    
    #pragma mark Vao methods
    Vao::Vao() :
    mSlot(0),
    mVbo(0),
    mStride(0),
    mHasData(false)
    {
    }
    
    Vao::~Vao()
    {
        destoroy();
    }
    
    void Vao::create(GLint slot)
    {
        assert(mVbo==0);
        
        if (mVbo==0) {
            mSlot = slot;
            glGenBuffers(1, &mVbo);
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            ofLogWarning("Vao") << "already generated a vbo";
        }
    }
    
    void Vao::destoroy()
    {
        if (mVbo!=0) {
            glDeleteBuffers(1, &mVbo);
            mVbo = 0;
        }
    }
    
    void Vao::setData(GLsizeiptr size, const GLvoid *data, GLint stride, GLenum usage)
    {
        assert(!mHasData);
        
        if (!mHasData) {
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            glBufferData(GL_ARRAY_BUFFER, size, data, usage);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            mHasData = true;
            mStride = stride;
        } else {
            ofLogWarning("Vao") << "already has data";
        }
    }
    
    void Vao::updateData(GLsizeiptr size, const GLvoid *data)
    {
        assert(mHasData);
        
        if (mHasData) {
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            ofLogWarning("Vao") << "dosen't have any data";
        }
    }
    
    void Vao::bind()
    {
        assert(mHasData);
        
        if (mHasData) {
            glEnableVertexAttribArray(mSlot);
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            glVertexAttribPointer(mSlot, mStride, GL_FLOAT, GL_FALSE, mStride, 0);
        } else {
            ofLogWarning("Vao") << "dosen't have any data";
        }
    }
    
    void Vao::unbind()
    {
        glDisableVertexAttribArray(mSlot);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
} // namespace onc
