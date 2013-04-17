// 
// ChainObject.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
//  ChainObject.h
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#ifndef __ConstrainTest__ChainObject__
#define __ConstrainTest__ChainObject__

#include "ofMain.h"
#include "BaseConstrains.h"

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

class ChainObject : public BaseConstrains {
public:
    ChainObject(class btDynamicsWorld *m_ownerWorld,
          const btVector3 &origin,
          int edges,
          float edgeLength,
          float edgeThickness);
    virtual ~ChainObject();
    
private:
    btTransform  createTransform(int i);
    btVector3 m_origin;
    btScalar m_thickness;
    btScalar m_edgeLength;
    btScalar m_edgeThickness;
};

#endif /* defined(__ConstrainTest__ChainObject__) */
