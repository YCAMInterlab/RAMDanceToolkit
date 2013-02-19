//
//  Chain.h
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#ifndef __ConstrainTest__Chain__
#define __ConstrainTest__Chain__

#include "ofMain.h"
#include "BaseConstrains.h"

#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btTransform.h"

class Chain : public BaseConstrains {
public:
    Chain(class btDynamicsWorld *m_ownerWorld, const btVector3 &origin);
    ~Chain();
private:
    btTransform  createTransform(int i);
    btVector3 m_origin;
    btScalar m_length;
    btScalar m_thickness;
};

#endif /* defined(__ConstrainTest__Chain__) */
