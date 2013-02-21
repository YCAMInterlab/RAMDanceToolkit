#pragma once

class Abacus : public ramBaseScene
{
	
	const int box_size;
	const float margin;
	const float width;
	
	ofLight light;
	
public:
	
	Abacus() : box_size(20), margin(5), width(500) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
	}
	
	void setup()
	{
		light.setPosition(300, 0, 300);
	}
	
	void draw()
	{
		ramBeginCamera();
		
		light.enable();
		glEnable(GL_DEPTH_TEST);
		
		int numNodeArrays = getNumNodeArray();
		
		for(int i=0; i<numNodeArrays; i++)
		{
			drawAbacus(getNodeArray(i), i, numNodeArrays);
		}
		
		ramEndCamera();
	}
	
	const string getName() { return "Abacus"; }
	
	
	
#pragma mark - 
	
	
	
	void drawAbacus(ramNodeArray& nodeArray, int index, int numNodeArrays)
	{
		const int numNodes = nodeArray.getNumNode();
		
		ofColor c1 = ramColor::BLUE_DEEP;
		ofColor c2 = ramColor::GREEN_DEEP;
		
		glTranslatef(0.0, -(margin+box_size) * numNodes / 2, 0.0);
		
		ofPushStyle();
		for(int i=0; i<numNodes; i++)
		{
			glPushMatrix();
			ramNode& node = nodeArray.getNode(i);
			ofVec3f pos = node.getGlobalPosition();
			
			pos.y = i*margin + i*box_size;
			pos.z = 0;
			
			ofSetColor(i%2==0 ? c1 : c2);
			ofSetLineWidth(2);
			ofLine(ofVec3f(-width, pos.y, 0), ofVec3f(width, pos.y, 0));
			
			glTranslatef(width*2/(numNodeArrays+1)*(index+1) - width, 0, 0);
			ofBox(pos, box_size);
			
			glPopMatrix();
		}
		ofPopStyle();
	}
};
