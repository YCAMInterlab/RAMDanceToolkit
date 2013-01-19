#include "ramGraphics.h"

#include "ramPhysics.h"
#include "ramPrimitive.h"

void ramBasicFloor(const float floorSize, const float tileSize, ofColor c1, ofColor c2)
{
	int division = floorSize/tileSize;
	
	ofPushStyle();
	ofFill();
	
	ofPushMatrix();
    ofRotate( 90.0f, 1.0f, 0.0f, 0.0f );
	
	if ( ofGetRectMode() != OF_RECTMODE_CENTER )
	{
		float w = division*tileSize;
		ofTranslate( -w/2.0f+tileSize/2.0f, -w/2.0f+tileSize/2.0f );
	}
	
	glNormal3f( 0.0f, 1.0f, 0.0f );
	
	for (int i=0; i<division; i++)
	{
		for (int j=0; j<division; j++)
		{
			ofSetColor( ( i%2==0 && j%2== 0 ) || ( i%2==1 && j%2== 1 ) ? c1 : c2 );
			ofRect( i*tileSize, j*tileSize, tileSize, tileSize );
		}
	}
	
	ofPopMatrix();
	ofPopStyle();
}


void ramBasicActor(ramActor& actor)
{
	glEnable(GL_DEPTH_TEST);
	for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		node.transformBegin();
		ofBox( i==(ramActor::JOINT_HEAD) ? 6 : 3 );
		node.transformEnd();
		
		if (node.hasParent())
			ofLine(node, *node.getParent());
	}
	glDisable(GL_DEPTH_TEST);
}

void ramBox(const ramNode& o, float size)
{
	ofBox(o, size);
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramBoxPrimitive *p = new ramBoxPrimitive(o.getPosition(), size);
//		p->setTransformMatrix(o.getMatrix());
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	ofSphere(o, radius);
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getPosition(), radius);
//		p->setTransformMatrix(o.getMatrix());
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}