//
//  oncVao.h
//  one
//
//  Created by Onishi Yoshito on 6/1/13.
//
//

#ifndef __one__oncVao__
#define __one__oncVao__

#pragma mark onc::Vao
#pragma mark - includs

#include <GL/glew.h>

#pragma mark - namespace onc
namespace onc {
    
    #pragma mark Vao
    // vartex array object class //
    // use w/ ofVbo and ofShader
    class Vao {
    public:
        Vao();
        ~Vao();
        
        void create(GLint slot);
        void destoroy();
        void setData(GLsizeiptr size, const GLvoid *data, GLint stride, GLenum usage);
        void updateData(GLsizeiptr size, const GLvoid *data);
        
        void bind();
        void unbind();
        
    private:
        GLint   mSlot;
        GLuint  mVbo;
        GLint   mStride;
        bool    mHasData;
    };
    
} // namespace onc

#endif /* defined(__one__oncVao__) */
