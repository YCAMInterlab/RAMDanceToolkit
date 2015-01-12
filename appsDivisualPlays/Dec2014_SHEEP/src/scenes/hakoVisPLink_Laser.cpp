//
//  hakoVisPLink_Laser.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/11.
//
//

#include "hakoVisPLink_Laser.h"

hakoVisPLink_Laser::hakoVisPLink_Laser(){

	receiver.addAddress("/dp/cameraUnit/");
	ramOscManager::instance().addReceiverTag(&receiver);

}

void hakoVisPLink_Laser::setupControlPanel(){

}

void hakoVisPLink_Laser::update(){

	while (receiver.hasWaitingMessages()){
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		if (m.getAddress() == "/dp/cameraUnit/plink/pixelate"){
			pix_w = m.getArgAsInt32(0);
			pix_h = m.getArgAsInt32(1);

			pixels.clear();
			pixels.assign(m.getArgAsInt32(0) * m.getArgAsInt32(1), false);
			for (int64_t i = 0;i < pixels.size();i++){
				int64_t bt = m.getArgAsInt64(2 + i / 64);
				int64_t targetBit = int64_t(1) << (i % 64);
				pixels[i] = (bt & targetBit) > 0;
			}
		}
	}

}

void hakoVisPLink_Laser::draw(){

	int cnt = 0;
	for (int j = 0;j < pix_h;j++){
		for (int i = 0;i < pix_w;i++){
			if (pixels.size() > cnt){
				ofCircle(300+i*10, 300+j*10, pixels[cnt]*5);
			}
			cnt++;
		}
	}

}

void hakoVisPLink_Laser::onPanelChanged(ofxUIEventArgs &e){

}

void hakoVisPLink_Laser::drawActor(const ramActor &actor){

}