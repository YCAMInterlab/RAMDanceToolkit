//
//  ramCenterringActor.h
//  SJQ_GlindingCircle
//
//  Created by 神田竜 on 2014/07/11.
//
//

#ifndef Kezzar_ramCenteredActor_h
#define Kezzar_ramCenteredActor_h

class ramCenteredActor : public ramBaseFilter
{
public:
    
	ramCenteredActor(){}
    
	const ramNodeArray& get(size_t index = 0) const { return copy; }
	size_t getSize() const { return 1; }
    
	void setupControlPanel()
	{
		
	}
    
	string getName() const { return "ramCenteredActor"; }
    
#pragma mark -
    

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		if (src.getNumNode() != copy.getNumNode()) copy = src;
        
        const ofPoint centroid = src.getCentroid();
        
		for (int i = 0; i < src.getNumNode(); i++)
		{
			ofVec3f input = src.getNode(i).getGlobalPosition();
			copy.getNode(i).setGlobalPosition(input.x - centroid.x,input.y,input.z - centroid.z);
		}
        
		return copy;
	}
    
protected:
    
	ramNodeArray copy;
	
};

#endif
