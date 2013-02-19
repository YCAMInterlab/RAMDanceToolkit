//
//  RiggedBox.h
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#ifndef __ConstrainTest__RiggedBox__
#define __ConstrainTest__RiggedBox__

#include "BaseConstrains.h"

class RiggedBox : public BaseConstrains {
public:
    RiggedBox(class btDynamicsWorld* ownerWorld, const btVector3& positionOffset);
    ~RiggedBox();
    btTransform createTransform(int i);
};

#endif /* defined(__ConstrainTest__RiggedBox__) */
