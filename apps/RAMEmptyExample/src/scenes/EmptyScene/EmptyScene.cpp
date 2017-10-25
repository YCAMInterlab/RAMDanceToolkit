#include "EmptyScene.h"

void EmptyScene::setup()
{
	mEx.setup(this);
}

void EmptyScene::update()
{
	mEx.update();
}

void EmptyScene::draw()
{
  rdtk::BeginCamera();
  
  /*  Debug preview  */
  mEx.draw();
  
  for (int i = 0;i < mEx.getNumPort();i++)
  {
    /*   Get extractor informations */
    
    ofMatrix4x4 mat = mEx.getNodeAt(i).getGlobalTransformMatrix();
    ofVec3f pos		= mEx.getPositionAt(i);
    ofVec3f next	= mEx.getPositionAt((i+1) % mEx.getNumPort());
    ofVec3f vel		= mEx.getVelocityAt(i);
    
    
    /*   Draw Primitives  */
    
    ofPushMatrix();
    {
      ofMultMatrix(mat);
      
      ofSetColor(255);
      ofDrawBox(0, 0, 0, vel.length() * 10.0);
      ofNoFill();
      ofSetColor(0);
      ofDrawBox(0, 0, 0, vel.length() * 11.0);
      ofFill();
    }
    ofPopMatrix();
    
    ofSetColor(0, 255, 150);
    ofDrawLine(pos, next);
  }
		
	rdtk::EndCamera();
}

void EmptyScene::drawImGui()
{
	mEx.drawImGui();
}
