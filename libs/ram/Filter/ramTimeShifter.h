#pragma once


enum ramTimeShifterType
{
	RAM_TIMESHIFT_BY_TIME = 0,
	RAM_TIMESHIFT_BY_FRAMES = 1
};

enum ramTimeShifterMaxValue
{
	RAM_TIMESHIFT_MAX_TIME = 180, // 3min
	RAM_TIMESHIFT_MAX_FRAMES = 10800 // 180 sec * 60fps
};

class ramTimeShifter : public ramBaseFilter
{
	deque<ramNodeArray> buffer;
	
	float mShiftTime;
	float mShiftFrames;
	
	float mRecStartTime;
	float mRecStartFrameNum;
	
	ramTimeShifterType mType;
	
public:
	
	ramTimeShifter () : mShiftTime(2), mShiftFrames(120), mType(RAM_TIMESHIFT_BY_TIME) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Shift time", 0, RAM_TIMESHIFT_MAX_TIME, &mShiftTime, gui.kLength, gui.kDim);
		panel->addSlider("Shift frames", 0, RAM_TIMESHIFT_MAX_FRAMES, &mShiftFrames, gui.kLength, gui.kDim);
		panel->addSlider("Shift frames", 0, RAM_TIMESHIFT_MAX_FRAMES, &mShiftFrames, gui.kLength, gui.kDim);
		
		ofAddListener(panel->newGUIEvent, this, &ramTimeShifter::onPanelChanged);
	}
	
	void setup()
	{
		clear();
	}
	
	const ramNodeArray& update(ramNodeArray &src)
	{
		const ramNodeArray copy = src;
		buffer.push_back(copy);
		
		if(isOverLimit())
			buffer.pop_front();
			
		return src;
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
	}
	
	const string getName() { return "ramTimeShifter"; }
	
	
#pragma mark -
	
	
	inline ramNodeArray& getNodeArray() { return buffer.front(); }
	inline void setShiftType(ramTimeShifterType type) { mType = type; }
	inline void setShiftTime(const float time) { mShiftTime = time > RAM_TIMESHIFT_MAX_TIME ? RAM_TIMESHIFT_MAX_TIME : time; }
	inline void setShiftFrames(const int frames) { mShiftFrames = frames > RAM_TIMESHIFT_MAX_FRAMES ? RAM_TIMESHIFT_MAX_FRAMES : frames; }
	
	inline bool isPlayable() { return isOverLimit(); }
	
	bool isOverLimit()
	{
		if ((mType == RAM_TIMESHIFT_BY_TIME) && (mShiftTime <= getElapsedRecTime()))
		{
			return true;
		}
		
		if ((mType == RAM_TIMESHIFT_BY_FRAMES) && (mShiftFrames < getElapsedRecFrames()))
		{
			return true;
		}
		return false;
	}
	
	float getElapsedRecTime()
	{
		return ofGetElapsedTimef() - mRecStartTime;
	}
	
	float getElapsedRecFrames()
	{
		return ofGetFrameNum() - mRecStartFrameNum;
	}
	
	void clear()
	{
		buffer.clear();
		mRecStartFrameNum = ofGetFrameNum();
		mRecStartTime = ofGetElapsedTimef();
	}
};