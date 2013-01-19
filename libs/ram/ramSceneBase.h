#pragma once

class ramSceneBase
{
public:
	ramSceneBase() : bEnabled(false) {}
	
	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { bEnabled ^= true; }
	
//	virtual void setup()
//	{
//		
//	}
//	
//	virtual void update()
//	{
//		
//	}
//	
//	virtual void draw()
//	{
//		
//	}
//	
//	virtual void drawActor(const ramActor& actor)
//	{
//		
//	}
//	
//	virtual void drawRigidBody(const ramRigidBody& rigid)
//	{
//		
//	}
//	
//	virtual void drawFloor()
//	{
//		
//	}
	
private:
	bool bEnabled;
};