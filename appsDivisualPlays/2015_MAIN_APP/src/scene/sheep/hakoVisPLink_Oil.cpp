//
//  hakoVisPLink_Oil.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/13.
//
//

#include "hakoVisPLink_Oil.h"

hakoVisPLink_Oil::hakoVisPLink_Oil(){

	receiver.addAddress("/dp/cameraUnit/Plink_Oil");
	ramOscManager::instance().addReceiverTag(&receiver);
    
    pix_w = 0;
    pix_h = 0;
}

void hakoVisPLink_Oil::setupControlPanel(){
	gui = ramGetGUI().getCurrentUIContext();

	vector<string> patterns;
	patterns.push_back("Pattern_A");
	patterns.push_back("Pattern_B");
	patterns.push_back("Pattern_C");

	gui->addToggle("DebugDraw", &mDebugDraw);

}

void hakoVisPLink_Oil::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/dp/cameraUnit/Plink_Oil/pixelate"){

			pix_w = m.getArgAsInt32(0);
			pix_h = m.getArgAsInt32(1);

			pixels.clear();
			pixels.assign(m.getArgAsInt32(0) * m.getArgAsInt32(1), false);
			offsets.assign(pix_w * pix_h, ofVec2f(0,0));
			scales.assign(pix_w * pix_h, 10.0);

			for (int64_t i = 0;i < pixels.size();i++){
				int64_t bt = m.getArgAsInt64(2 + i / 64);
				int64_t targetBit = int64_t(1) << (i % 64);
				pixels[i] = (bt & targetBit) > 0;
				offsets[i].set(ofNoise(i*32.41)*10,
							   ofNoise(i*67.32)*10);
				scales[i] = ofNoise(i*43.135)*30+5;
			}
		}

	}

	while (scaleTarg.size() < scales.size()) {
		scaleTarg.push_back(0.0);
	}

	while (scaleTarg.size() > scales.size()){
		scaleTarg.pop_back();
	}

	for (int i = 0;i < scaleTarg.size();i++){
		scaleTarg[i] += (scales[i] * pixels[i] - scaleTarg[i]) / 25.0;
	}

}

void hakoVisPLink_Oil::draw(){

	ofSetCircleResolution(6);
	for (int i = 0;i < pix_w;i++){
		for (int j = 0;j < pix_h;j++){
			ofPushMatrix();
			ofTranslate(0+i*(SINGLE_SCREEN_WIDTH / pix_w),
						j*(SINGLE_SCREEN_HEIGHT / pix_h));
			ofTranslate(offsets[i*pix_w + j]);

			ofSetColor(255);
			if (ofNoise(i*35.65, j*23.42) < 0.5) ofNoFill();
			if (ofNoise(i*95.34, j*43.14) < 0.2) ofSetColor(dpColor::MAIN_COLOR);
			ofCircle(0, 0, scaleTarg[i*pix_w + j]);
			ofFill();
			ofPopMatrix();
		}
	}
	ofSetCircleResolution(15);
}

void hakoVisPLink_Oil::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Oil::draw_PatternA(){

}

void hakoVisPLink_Oil::draw_PatternB(){

}
