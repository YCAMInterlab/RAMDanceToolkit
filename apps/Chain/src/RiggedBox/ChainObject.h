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
