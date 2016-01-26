//
//  dpSwitchingManager.h
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2015/01/09.
//
//

#ifndef __dpHakoniwa_cameraUnit__dpSwitchingManager__
#define __dpHakoniwa_cameraUnit__dpSwitchingManager__

#include "ofMain.h"
#include "sw_1010F_SerialController.h"
#include "dpCameraUnit_cvFX.h"
#include "dpCameraUnit_cvAnalysis.h"
#include "ofxOsc.h"

#define NETWORK_ISSTOP false

#define DISPLAY_SHIMO_OKU 4
#define DISPLAY_KAMI_OKU 5
#define DISPLAY_SHIMO_TEMAE 6
#define DISPLAY_KAMI_TEMAE 7

#define CVSW_1 1
#define CVSW_2 2
#define CVSW_3 3
#define CVSW_4 4

#define SHUTTER_CHANNEL 10

#define CV_SLOT_NUM 4

#define RDTK_TWIN (false)//for KAAT: RDTK２台体制で、箱庭シーンを振り分ける用フラグ
#define SENDLASTHAKO (true)//最後に選択された箱庭を独立チャンネルに送る

enum hakoniwaType{
    HAKO_COLOROFWATER,
    HAKO_WORM,
    HAKO_SERVOPENDULUM,
    HAKO_STRUGGLE,
    HAKO_MAGPENDULUM,
    HAKO_SANDSTORM,
    HAKO_TORNADO,
    HAKO_STAGE,
	HAKO_PLINK_PRISM,
	HAKO_PLINK_LASER,
	HAKO_PLINK_OIL,
	HAKO_THETA,
	HAKO_FROZENICE,

	HAKO_GEAR,

	HAKO_METABALL,
	HAKO_LINE,
	HAKO_FOURPOINT,
	HAKO_FUTURE,
    HAKO_FUTURE_LEG,
    HAKO_ONNOTE,
    HAKO_EYEBALLDANCER,
	HAKO_KIOKU,

	HAKO_TESTA,
	HAKO_TESTB,
	HAKO_TESTC,
	HAKO_TESTD,

	HAKO_BLANK,
};

struct hakoniwaPresets{
public:
	hakoniwaPresets(){
		isEnable = false;
	};
	~hakoniwaPresets(){};

	hakoniwaType	type;
	string			CVPreset;
	vector<string>	sceneNames;
	int				sourceCh;
	bool			isEnable;

	bool getIsVis(int num){
		return sceneNames[num].substr(0,1) == "V";
	}
};

class cvSlot{
public:
	cvSlot(){
		isEmpty = true;
		hakoType = HAKO_BLANK;
		sourceCh = -1;
	}

	bool			isEmpty;
	hakoniwaType	hakoType;
	int				sourceCh;
	vector<int>		targetDisplay;
	string			presetFile;
	int				matrixInputCh;

	bool displayIsExist(int num){
		for (int i = 0;i < targetDisplay.size();i++){
			if (targetDisplay[i] == num) return true;
		}
		return false;
	}
};

class dpSwitchingManager{
public:

	void setup(dpCameraUnit_cvFX* fxP,
			   dpCameraUnit_cvAnalysis* anP);
	void update();
	void draw();

	void receiveOscMessage(ofxOscMessage &m);

	void SelectHakoniwa(hakoniwaType type, int slot);

	void enableDisplay(hakoniwaType type,int displayNum,bool newHako);
	void disableHakoniwa(hakoniwaType type);
	void disableDisplay(int displayNum);

	void sendCurrentHakoniwa();

    bool totalManage;
	bool isSlave;

	dpCameraUnit_cvFX* FXPtr;
	dpCameraUnit_cvAnalysis* AnalysisPtr;

	hakoniwaPresets* getHakoniwaPreset(hakoniwaType type);

	vector<hakoniwaPresets*> hakoniwas;

	ofxOscSender senderToSlave;
	ofxOscSender senderToRDTK1;
	ofxOscSender senderToRDTK2;
    ofxOscSender senderToFloor;
    
	cvSlot mSlots[CV_SLOT_NUM];
	sw_1010F_SerialController matrixSW;

	void refleshSceneforRDTK();
	int	getHakoniwaIndex(string sceneName);
	int searchHakoniwaIsActive(hakoniwaType type);

	bool mVisEnable;

	//MultiCasting
	ofxOscSender sender;
	void multiCast(ofxOscMessage &m);
	bool				senderSW[9];
	vector<string>		*oscListPtr;
};

#endif /* defined(__dpHakoniwa_cameraUnit__dpSwitchingManager__) */
