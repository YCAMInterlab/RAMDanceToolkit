#include "ramBaseApp.h"
#include "ramGraphics.h"
#include "ramPhysics.h"
#include "ramPrimitive.h"
#include "ramUtils.h"

#pragma mark - ramColor

const ofColor ramColor::RED_NORMAL		= ofColor::fromHex(0xff6666);
const ofColor ramColor::RED_DEEP		= ofColor::fromHex(0x993333);
const ofColor ramColor::RED_LIGHT	 	= ofColor::fromHex(0xff9898);

const ofColor ramColor::GREEN_NORMAL 	= ofColor::fromHex(0x66cc33);
const ofColor ramColor::GREEN_DEEP		= ofColor::fromHex(0x339900);
const ofColor ramColor::GREEN_LIGHT 	= ofColor::fromHex(0x99cc99);

const ofColor ramColor::BLUE_NORMAL 	= ofColor::fromHex(0x0099cc);
const ofColor ramColor::BLUE_DEEP   	= ofColor::fromHex(0x003366);
const ofColor ramColor::BLUE_LIGHT  	= ofColor::fromHex(0x99cccc);

const ofColor ramColor::YELLOW_NORMAL	= ofColor::fromHex(0xffcc00);
const ofColor ramColor::YELLOW_DEEP 	= ofColor::fromHex(0xcc9900);
const ofColor ramColor::YELLOW_LIGHT	= ofColor::fromHex(0xffff00);

const ofColor ramColor::BLACK			= ofColor::fromHex(0x000000);
const ofColor ramColor::GRAY			= ofColor::fromHex(0x666666);
const ofColor ramColor::WHITE			= ofColor::fromHex(0xffffff);

const ofColor ramColor::SHADOW			= ofColor(0,0,0,60);

//

void ramBox(const ramNode& o, float size)
{
	o.beginTransform();
	ofBox(size);
	o.endTransform();
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramBoxPrimitive *p = new ramBoxPrimitive(o.getTransformMatrix(), size);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

void ramSphere(const ramNode& o, float radius)
{
	o.beginTransform();
	ofSphere(radius);
	o.endTransform();
	
	if (ramGetEnablePhysicsPrimitive())
	{
		ramSpherePrimitive *p = new ramSpherePrimitive(o.getGlobalPosition(), radius);
		ramPhysics::instance().registerTempraryPrimitive(p);
	}
}

//

void ramBasicFloor(const int floorPattern,
				   const float floorSize,
				   const float tileSize,
				   const ofColor& c1,
				   const ofColor& c2)
{
	if (floorPattern == ramFloor::FLOOR_NONE) return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(4.0, 1.0);
	
	int division = floorSize/tileSize;
	
	ofPushStyle();
	ofSetLineWidth(1.0);
	
	ofPushMatrix();
    ofRotate( 90.0f, 1.0f, 0.0f, 0.0f );
	
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
	
	ofPopMatrix();
	ofPopStyle();

	glPopAttrib();
}

void ramBasicActor(const ramActor& actor,
				   const ofColor& jointColor,
				   const ofColor& lineColor)
{
	assert(actor.getNumNode() == ramActor::NUM_JOINTS);
	
	glPushMatrix();
	for (int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.beginTransform();
		ofSetColor( jointColor );
		ofBox( jointSize );
		node.endTransform();
		
		if (node.hasParent())
		{
			ofSetColor( lineColor );
			ofLine(node, *node.getParent());
		}
	}
	glPopMatrix();
}

void ramActorCube(ramActor& actor, ofColor c)
{
	ofVec3f maxPos = actor.getNode( ramActor::JOINT_CHEST ).getGlobalPosition();
	ofVec3f minPos = actor.getNode( ramActor::JOINT_CHEST ).getGlobalPosition();
	
	for (int j=0; j<actor.getNumNode(); j++)
	{
		ofVec3f pos = actor.getNode(j).getGlobalPosition();
		
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
	
	ofPushStyle();
	{
		ofSetLineWidth( 2 );
		ofSetColor( c );
		
		ofPushMatrix();
		{
			ofTranslate( axis.x, axis.y, axis.z );
			ofScale( scale.x, scale.y, scale.z );
			ofNoFill();
			ofBox(1);
		}
		ofPopMatrix();
	}
	ofPopStyle();
}
