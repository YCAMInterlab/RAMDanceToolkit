#include "ramBaseApp.h"
#include "ramGraphics.h"
#include "ramPhysics.h"
#include "ramPrimitive.h"
#include "ramUtils.h"



void ramBox(const ramNode& o, float size)
{
	o.transformBegin();
	ofBox(size);
	o.transformEnd();
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramBoxPrimitive *p = new ramBoxPrimitive(o.getTransformMatrix(), size);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	ofSphere(o, radius);
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getPosition(), radius);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}


void ramBasicFloor(const int floorPattern,
				   const float floorSize,
				   const float tileSize,
				   const ofColor& c1,
				   const ofColor& c2)
{
	
	if (floorPattern == ramFloor::FLOOR_NONE) return;
	
	int division = floorSize/tileSize;
	
	ofPushStyle();
	ofSetLineWidth(1.0);
	
	ofPushMatrix();
    ofRotate( 90.0f, 1.0f, 0.0f, 0.0f );
	
	glEnable(GL_DEPTH_TEST);
	
	if (ofGetRectMode() != OF_RECTMODE_CENTER)
	{
		float w = division*tileSize;
		ofTranslate( -w/2.0f, -w/2.0f );
	}
	
	glNormal3f( 0.0f, 1.0f, 0.0f );
	
	
	if(floorPattern == ramFloor::FLOOR_PLANE)
	{
		ofFill();
		ofSetColor(c1);
		ofRect(0, 0, 0, division*tileSize, division*tileSize);
	}
	
	for (int i=0; i<division; i++)
	{
		for (int j=0; j<division; j++)
		{
			switch (floorPattern)
			{
					
				case ramFloor::FLOOR_CHECKER_PATTERN :
					ofFill();
					ofSetColor( ( i%2==0 && j%2== 0 ) || ( i%2==1 && j%2== 1 ) ? c1 : c2 );
					ofRect( i*tileSize, j*tileSize, tileSize, tileSize );
					break;
					
				case ramFloor::FLOOR_GRID_LINES :
					ofNoFill();
					ofSetColor(c1);
					ofRect( i*tileSize, j*tileSize, tileSize, tileSize );
					break;
			}
		}
	}
	
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
	ofPopStyle();
}


void ramBasicActor(ramActor& actor,
				   ofColor jointColor,
				   ofColor lineColor,
				   float* matrixPtr)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.transformBegin();
		ofSetColor( jointColor );
		ofBox( jointSize );
		node.transformEnd();
		
		if (node.hasParent())
		{
			ofSetColor( lineColor );
			ofLine(node, *node.getParent());
		}
		
		
		if (matrixPtr != NULL)
		{
			ofColor shadowColor = ramColor::GRAY;
			shadowColor.a = 50;
			glPushMatrix();
			glDisable(GL_DEPTH_TEST);
			glMultMatrixf(matrixPtr);
			ofEnableAlphaBlending();
			ofSetColor(shadowColor);
			
			ofBox(node, jointSize);
			if (node.hasParent())
				ofLine(node, *node.getParent());
			glPopMatrix();
		}
	}
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glPopAttrib();
	
}

void ramBasicActor(ramActor& actor, float* matrixPtr)
{
	ramBasicActor(actor, ramColor::BLUE_LIGHT, ramColor::GRAY, matrixPtr);
}


void ramActorCube(ramActor& actor)
{
	ofVec3f maxPos = actor.getNode( ramActor::JOINT_CHEST ).getPosition();
	ofVec3f minPos = actor.getNode( ramActor::JOINT_CHEST ).getPosition();
	
	for (int j=0; j<actor.getNumNode(); j++)
	{
		ofVec3f pos = actor.getNode(j).getPosition();
		
		if( maxPos.x <= pos.x ) maxPos.x = pos.x;
		if( maxPos.y <= pos.y ) maxPos.y = pos.y;
		if( maxPos.z <= pos.z ) maxPos.z = pos.z;
		
		if( minPos.x > pos.x ) minPos.x = pos.x;
		if( minPos.y > pos.y ) minPos.y = pos.y;
		if( minPos.z > pos.z ) minPos.z = pos.z;
	}
	
	ofVec3f scale, axis;
	scale = (maxPos - minPos);
	axis = (maxPos + minPos) / 2;
	
	ofSetLineWidth( 2 );
	ofSetColor( ramColor::BLUE_DEEP );
	
	ofPushMatrix();
	{
		ofTranslate( axis.x, axis.y, axis.z );
		ofScale( scale.x, scale.y, scale.z );
		ofNoFill();
		ofBox(1);
	}
	ofPopMatrix();

}
