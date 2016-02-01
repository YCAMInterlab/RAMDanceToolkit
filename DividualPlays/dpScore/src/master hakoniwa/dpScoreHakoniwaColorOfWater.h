//
//  dpScoreHakoniwaColorOfWater.h
//  dpScore
//
//  Created by YoshitoONISHI on 2/2/15.
//
//

#ifndef __dpScore__dpScoreHakoniwaColorOfWater__
#define __dpScore__dpScoreHakoniwaColorOfWater__

#include "dpScoreCommon.h"
#include "ofxOsc.h"
#include "ofxUI.h"

DP_SCORE_NAMESPACE_BEGIN

class HakoniwaColorOfWater final {
public:
	void initialize();
	void shutdown();

	void update();
	void draw();

	void setupGui(ofxUITabBar* tabbar);

	void turnOnValve(int index);
	void turnOffAllPins();
	bool getIsOpeningValve(int index);

	void stopAll();
    
    void guiEvent(ofxUIEventArgs& e);

	constexpr static const int kNumValvePins {6};
	constexpr static const int kNumPumpPins {2};

	bool enableOpenValve {false};
	bool enableOscOut {false};

private:
	constexpr static const int kUpdateFrames {10};

	static const int kValvePins[];
	static const int kPumpPins[];

	static const int kPumpPinForward;
	static const int kPumpPinBack;

	struct Valve {
		void update(HakoniwaColorOfWater* owner);
        void setCleanUp(HakoniwaColorOfWater* owner, bool clean);
        
		bool open {false};
		bool prevOpen {false};
		float openedTime {0.f};
		float closedTime {0.f};
		int pin {0};
		int nOpend {0};
        bool cleanUp{false};
	};

	struct Pump {
		void update(HakoniwaColorOfWater* owner);

		bool open {false};
		bool prevOpen {false};
		float openedTime {0.f};
		float closedTime {0.f};
		int pin {0};
	};

	void sendPin(int pin, bool open);

	ofxOscSender mOscSender;

	vector<Valve> mValves;
	vector<Pump> mPumps;

	float mValveOpenDuration {0.3f};
    
    bool mOpenAllValvesArm {false};
    bool mOpenAllValvesTrigger {false};

};

DP_SCORE_NAMESPACE_END

#endif /* defined(__dpScore__dpScoreHakoniwaColorOfWater__) */
