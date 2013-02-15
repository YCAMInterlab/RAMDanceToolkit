#pragma once

class ramStamp : public ramBaseFilter
{

	deque<ramNodeArray> mStamps;
    float mLastRecordTime;
	float mRecSpan;

	
public:
	
	ramStamp() : mLastRecordTime(0.0), mRecSpan(5.0) {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Clear", false, 30, 30);
		panel->addSlider("Recording Span", 0.5, 60.0, &mRecSpan, gui.kLength, gui.kDim);
		
		ofAddListener(panel->newGUIEvent, this, &ramStamp::onPanelChanged);
	}
	
	void setup()
	{
		clear();
	}
	
	const ramNodeArray& update(ramNodeArray& src)
	{
		if (ofGetElapsedTimef() > mLastRecordTime)
		{
			createStamp(src);
		}
		
		return src;
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		if (name == "Clear")
		{
			clear();
		}
		
		if (name == "Recording Span")
		{
			mLastRecordTime = ofGetElapsedTimef() + mRecSpan;
		}
	}
	
	const string getName() { return "ramStamp"; }
	
	
#pragma mark -
	
	
	void clear()
	{
		mLastRecordTime = ofGetElapsedTimef() + mRecSpan;
		mStamps.clear();
	}
	
	const ramNodeArray createStamp(ramNodeArray& src)
	{
		mLastRecordTime += mRecSpan;
		ramNodeArray copy = src;
		mStamps.push_back(copy);
		
		return copy;
	}
	
	inline void setRecSpan(const float span) { mRecSpan = span; }
	
	inline int getNumStamps() { return mStamps.size(); }
	inline const deque<ramNodeArray>& getStamps() { return mStamps; }
	inline const ramNodeArray& getStamp(const int index) { return mStamps.at(index); }
	
};