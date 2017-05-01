//
//  ramBaseHasFbo.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/11/15.
//  Copyright (c) 2015 YCAMInterlab. All rights reserved.
//

#include "ramBaseHasFbo.h"

using namespace rdtk;

BaseHasFbo::BaseHasFbo() :
mAllocated(false)
{
    
}

BaseHasFbo::~BaseHasFbo()
{
    
}

void BaseHasFbo::allocateFbo(int w, int h)
{
    if (mAllocated) return;
    mAllocated = true;
    
    mFbo = ofPtr<ofFbo>(new ofFbo());
    if (w) mFboSettings.width = w;
    if (h) mFboSettings.height = h;
    mFboSettings.useDepth = true;
    mFboSettings.internalformat = GL_RGBA;
    mFbo->allocate(mFboSettings);
    
    mFbo->begin();
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    mFbo->end();
}

void BaseHasFbo::deallocateFbo()
{
    if (!mAllocated) return;
    mAllocated = false;
    mFbo.reset();
}
