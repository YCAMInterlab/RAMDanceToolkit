#pragma once

class Stamp : public ramBaseScene
{
	
	ramStamp mStamp;
	bool mShowActor;
	bool mShowBox;
	
	float line_width;
	
	ofFloatColor color;
	
	class StampTimer : public ramTimerdMovementAnalyser
	{
	public:
		
		Stamp *stamp;
		string name;
		
		void setup(Stamp *stamp, string name)
		{
			this->stamp = stamp;
			this->name = name;
			setTargetName(name);
		}
		
		void onTimerdStop()
		{
			if (hasNodeArray(name))
			{
				stamp->mStamp.createStamp(getNodeArray(name));
			}
		}
	};
	
	map<string, StampTimer> timers;
	
	ofFloatColor timer_display_color;
	
	float timer_duration;
	float threshold;
	
	bool use_idle_timer;
	bool debug_draw;
	
public:
	
	const string getName() { return "Stamp"; }
	
	Stamp() : mShowActor(true), mShowBox(true) {}
	
	void setupControlPanel()
	{		
		mStamp.setupControlPanel();
		
		ramGetGUI().addToggle("Show Actor", &mShowActor);
		ramGetGUI().addColorSelector("Box line color", &color);
		ramGetGUI().addSlider("Line width", 0, 6, &line_width);
		
		use_idle_timer = false;
		timer_duration = 0.5;
		threshold = 3;
		debug_draw = false;
		
		ramGetGUI().addToggle("use_idle_timer", &use_idle_timer);
		
		ramGetGUI().addSlider("timer_duration", 0, 4, &timer_duration);
		ramGetGUI().addSlider("threshold", 0, 10, &threshold);
		
		ramGetGUI().addToggle("debug_draw", &debug_draw);
	}
	
	void setup()
	{
		mStamp.clear();
	}
	
	void update()
	{
		const int num = getNumNodeArray();
		
		if (use_idle_timer)
		{
			for (int i = 0; i < num; i++)
			{
				const ramNodeArray &NA = getNodeArray(i);
				if (timers.find(NA.getName()) == timers.end())
				{
					timers[NA.getName()].setup(this, NA.getName());
				}
			}

			map<string, StampTimer>::iterator it = timers.begin();
			while (it != timers.end())
			{
				it->second.setThreshold(threshold);
				it->second.setTime(timer_duration);
				it->second.update();
				it++;
			}
		}
		else
		{
			timers.clear();
			
			if(num > 0)
			{
				mStamp.update( getNodeArray(ofRandom(0, num)) );
			}
		}
	}
	
	void draw()
	{
		ramBeginCamera();
		
		if (use_idle_timer && debug_draw)
		{
			map<string, StampTimer>::iterator it = timers.begin();
			while (it != timers.end())
			{
				it->second.draw();
				it++;
			}
		}
		
		for (int i=0; i<mStamp.getSize(); i++)
		{
			ramNodeArray& nodeArray = mStamp.getStamp(i);
			
			if (mShowActor)
			{
				ramDrawNodes(nodeArray);
			}
			
			if (color.a > 0)
			{
				ofPushStyle();
				ofSetColor(color);
				ofSetLineWidth(line_width);
				ofSetColor(ramColor::RED_LIGHT);
				ramDrawActorCube(nodeArray);
				ofPopStyle();
			}
		}
		
		ramEndCamera();
	}
	
};

