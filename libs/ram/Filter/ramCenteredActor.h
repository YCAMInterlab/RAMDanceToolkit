//
//  ramCenterringActor.h
//  SJQ_GlindingCircle
//
//  Created by 神田竜 on 2014/07/11.
//
//

#ifndef Kezzar_ramCenteredActor_h
#define Kezzar_ramCenteredActor_h


namespace rdtk {

	class CenteredActor : public BaseFilter
	{
	public:
		
		CenteredActor(){}
		
		const NodeArray& get(size_t index = 0) const { return copy; }
		size_t getSize() const { return 1; }
		
		void setupControlPanel()
		{
			
		}
		
		string getName() const { return "ramCenteredActor"; }
		
#pragma mark -
		
		
		const NodeArray& filter(const NodeArray& src)
		{
			if (src.getNumNode() != copy.getNumNode()) copy = src;
			
			const ofPoint center = src.getNode(Actor::JOINT_ABDOMEN).getGlobalPosition();
			
			for (int i = 0; i < src.getNumNode(); i++)
			{
				ofVec3f input = src.getNode(i).getGlobalPosition();
				copy.getNode(i).setGlobalPosition(input.x - center.x,input.y,input.z - center.z);
			}
			
			return copy;
		}
		
	protected:
		
		NodeArray copy;
		
	};

	
}

typedef rdtk::CenteredActor RAMDEPRECATED(ramCenteredActor);

#endif
