// 
// btPicker.h - RAMDanceToolkit
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
//  btPicker.h
//  DanceWithRagdall2
//
//  Created by Onishi Yoshito on 1/24/13.
//
//

#ifndef __DanceWithRagdall2__btPicker__
#define __DanceWithRagdall2__btPicker__

#include <assert.h>

#include "LinearMath/btVector3.h"

class btPicker {
public:
    btPicker();
    ~btPicker();
    
public:
    void setWorld(class btDynamicsWorld *world);
    void updatePosition(const btVector3 &position);
    void attatchRigidBody(class btRigidBody *body);
    
    btVector3 getPosition() const;
    
private:
    void clear();
    class btDynamicsWorld   *m_world;
    class btTypedConstraint *m_pickConstraint;
    class btRigidBody       *m_pickedBody;
    btVector3   m_position;
    
};


#endif /* defined(__DanceWithRagdall2__btPicker__) */
