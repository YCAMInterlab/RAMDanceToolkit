//
//  btPicker.h
//  DanceWithRagdall2
//
//  Created by Onishi Yoshito on 1/24/13.
//
//

#ifndef __DanceWithRagdall2__btPicker__
#define __DanceWithRagdall2__btPicker__

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
