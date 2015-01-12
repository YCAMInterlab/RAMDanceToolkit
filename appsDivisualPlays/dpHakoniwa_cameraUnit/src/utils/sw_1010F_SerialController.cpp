//
//  sw-1010F_SerialController.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/21.
//
//

#include "sw_1010F_SerialController.h"

sw_1010F_SerialController::sw_1010F_SerialController(){
	serial_.listDevices();
	serial_.setup("cu.usbserial-A5002vny",9600);
}

sw_1010F_SerialController::~sw_1010F_SerialController(){

	serial_.close();

}

void sw_1010F_SerialController::setSW(int cameraChannel, int displayChannel){

	unsigned char buf[4] = {cameraChannel+49, ',', displayChannel+49, '\r'};
	serial_.writeBytes(buf, 4);

}

//	if (!ofGetKeyPressed('a')){
//		unsigned char buf[8] = {ofRandom(49,58), ',',
//								ofRandom(49,58), ';',
//								ofRandom(49,58), ',',
//								ofRandom(49,58), '\r'};
//		ser.writeBytes(buf, 8);
//	}