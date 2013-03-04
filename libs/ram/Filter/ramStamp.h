#pragma once

class ramStamp : public ramBaseFilter
{

	deque<ramNodeArray> mStamps;
	float mLastRecordTime;
	float mRecSpan;

	int kMaxStamps;

public:

	ramStamp() : mLastRecordTime(0.0), mRecSpan(5.0), kMaxStamps(30) {}

	void setupControlPanel()
	{
		gui().addSection(getName());

		ofAddListener(gui().addButton("Clear"), this, &ramStamp::onClear);
		gui().addSlider("Recording Span", 2.0, 60.0, &mRecSpan);
	}
	
	void onClear(ofEventArgs &e)
	{
		clear();
	}

	void setup()
	{
		clear();
	}

#pragma mark -

	const ramNodeArray& get(size_t index = 0) const { return mStamps[index]; }
	size_t getSize() const { return mStamps.size(); }

	void clear()
	{
		mLastRecordTime = ofGetElapsedTimef();
		mStamps.clear();
	}

	const ramNodeArray createStamp(const ramNodeArray& src)
	{
		mLastRecordTime = ofGetElapsedTimef();
		ramNodeArray copy = src;
		mStamps.push_back(copy);

		return copy;
	}

	inline void setRecSpan(const float span) { mRecSpan = span; }
	inline deque<ramNodeArray>& getStamps() { return mStamps; }
	inline ramNodeArray& getStamp(const int index) { return mStamps.at(index); }

	const string getName() { return "ramStamp"; }

protected:

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		if (ofGetElapsedTimef() - mLastRecordTime > mRecSpan)
		{
			createStamp(src);
			if (kMaxStamps < mStamps.size())
				mStamps.pop_front();
		}

		return src;
	}
};