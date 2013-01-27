#pragma once


static float REC_SPAN = 10.0;

class PastMe : public ramSceneBase
{
	
	string key_toggle, key_add, key_box, key_actor;
	
	deque<ramActor> pastMe;
    float lastRecordTime;
	
	
	
	bool showBox, showActor;
	ofVec3f maxPos, minPos;
	
public:
	
	PastMe()
	{
		setSceneName("Past Me");
		
		// ---
		
		key_toggle = "Clear";
		key_add = "add";
		
		key_box = "show box";
		key_actor = "show actor";
		
		showBox = false;
		showActor = false;
	}
	
	
	void setup()
	{
		lastRecordTime = 0.0;
		pastMe.clear();
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		gui.addToggle( key_toggle );
		gui.addToggle( key_add );
		gui.addToggle( key_box );
		gui.addToggle( key_actor );
		
		guiPtr = &gui;
	}
	
	void update()
	{
		
		if ( getActorManager().getNumActor() > 0 )
		{
			if ( ofGetElapsedTimef() > lastRecordTime )
			{
				lastRecordTime += REC_SPAN;
				pastMe.push_back( getActor( myActorName ) );
			}
		}
		
		if( guiPtr->hasValueChanged( key_add ) )
		{
			pastMe.push_back( getActor( myActorName ) );
		}
		
		if( guiPtr->hasValueChanged( key_toggle ) )
		{
			pastMe.clear();
		}
		
		if( guiPtr->hasValueChanged( key_actor ) )
		{
			showActor = guiPtr->getValueB( key_actor );
		}
		
		if( guiPtr->hasValueChanged( key_box ) )
		{
			showBox = guiPtr->getValueB( key_box );
		}
		
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		if (!bEnabled) return;
		
		ofPushStyle();
		ofColor currSklColor( 110, 20, 20 );
		ofColor recSklColor( 20, 20, 110 );
		ofColor locusColor( 20, 110, 20 );
		
		
		for ( int i=0; i<pastMe.size(); i++ )
		{
			ramActor &skl = pastMe.at( i );
			
			
			if (showActor)
			{
				ramBasicActor(skl, ramColor::BLUE_DEEP, ramColor::BLUE_DEEP, getMatrix().getPtr());
			}
			
			if (showBox)
			{
				drawBox(skl);
			}
			
			/*!
				Actor name
			 */
//			ofSetColor( recSklColor );
//			ofSetDrawBitmapMode( OF_BITMAPMODE_MODEL_BILLBOARD );
//			ofVec3f namePos = skl.getNode( ramActor::JOINT_HEAD ).getPosition();
//			namePos.y += 20.0;
//			ofDrawBitmapString( actor.getName(), namePos );
		}
		ofPopStyle();
		
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
	
	}
	void drawBox(ramActor& actor)
	{
		
		maxPos = actor.getNode( ramActor::JOINT_CHEST );
		minPos = actor.getNode( ramActor::JOINT_CHEST );
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
		
		ofColor shadowColor(20, 20, 20, 100);
		
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
};

