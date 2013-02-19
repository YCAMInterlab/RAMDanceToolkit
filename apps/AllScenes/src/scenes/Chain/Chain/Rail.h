//
//  Rail.h
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#ifndef __ConstrainTest__Rail__
#define __ConstrainTest__Rail__

#include "ofMain.h"
#include "BaseConstrains.h"

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

class Rail : public BaseConstrains {
public:
    Rail(class btDynamicsWorld *m_ownerWorld, const btVector3 &origin);
    ~Rail();
    
    virtual void draw();
    
private:
    btAlignedObjectArray<btTransform> m_initialTransforms;
    btTransform  createTransform(int i);
    btVector3 m_origin;
    btScalar m_length;
    btScalar m_thickness;
};

#endif /* defined(__ConstrainTest__Rail__) */
