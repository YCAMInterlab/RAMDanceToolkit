//
//  ofxKsmrStepManager.cpp
//  sheep_ramSceneTest
//
//  Created by Ovis aries on 2014/08/09.
//
//

#include "ofxKsmrStepManager.h"

#pragma mark - Initialize
void ofxKsmrStepManager::setupOsc(string address, int port){
	useOsc = true;
	sender.setup(address, port);
}

void ofxKsmrStepManager::setup(string portName, int baud){
	serial.setup(portName, baud);
}

void ofxKsmrStepManager::setup(int portNum, int baud){
	serial.setup(portNum, baud);
}

void ofxKsmrStepManager::addStepper(string name, int numStep, int SPIch){

	steppers.push_back( virtualSteppingMotor(name, numStep, SPIch) );

}

#pragma mark - SPI Sender
//1命令1パケットとして複数バイトを全モーターに送信する
void ofxKsmrStepManager::sendSPIPacketAll(unsigned char *bytes, int length){

	int sigLength = (2 + steppers.size()) * length;
	unsigned char signals[sigLength];

	int byteCount = 0;
	for (int i = 0;i < sigLength;i+=(2+steppers.size())){

		signals[i  ] = 0x02;
		signals[i+1] = steppers.size();
		for (int j = 0;j < steppers.size();j++){
			signals[i+2+j] = bytes[byteCount];
		}
		byteCount++;

	}

	sendSPIMultiByte(signals, sigLength);

}

void ofxKsmrStepManager::sendSPIPacketSelected(unsigned char *bytes, int length){

	int sigLength = (2 + steppers.size()) * length;
	unsigned char signals[sigLength];

	int byteCount = 0;
	for (int i = 0;i <  sigLength;i+=(2+steppers.size())){

		signals[i  ] = 0x02;
		signals[i+1] = steppers.size();
		for (int j = 0;j < steppers.size();j++){
			if (steppers[j].sendEnable) signals[i+2+j] = bytes[byteCount];
			else						signals[i+2+j] = 0x00;
		}
		byteCount++;

	}

	sendSPIMultiByte(signals, sigLength);

}

void ofxKsmrStepManager::sendSPIMultiByte(unsigned char *bytes, int length){
	if (serial.isInitialized()) serial.writeBytes(bytes, length);
	if (useOsc) sendBytesOnline(bytes, length);
}

void ofxKsmrStepManager::sendSPIByteAll(unsigned char byte){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();
	for (int i = 2;i < steppers.size()+2;i++){
		sig[i] = byte;
	}

	if (serial.isInitialized()) serial.writeBytes(sig, steppers.size()+2);
	if (useOsc) sendBytesOnline(sig, steppers.size()+2);

}

void ofxKsmrStepManager::sendSPIByteSelected(unsigned char byte){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();

	for (int i = 2;i < steppers.size()+2;i++){
		if (steppers[i - 2].sendEnable) sig[i] = byte;
		else sig[i] = 0x0;
	}

	if (serial.isInitialized()) serial.writeBytes(sig, steppers.size()+2);
	if (useOsc) sendBytesOnline(sig, steppers.size()+2);

}

void ofxKsmrStepManager::sendSPIByteSingle(unsigned char byte, int ch){
	unsigned char sig[steppers.size() + 2];

	sig[0] = 0x02;
	sig[1] = steppers.size();
	for (int i = 2;i < steppers.size()+2;i++){
		if (i - 2 == ch) sig[i] = byte;
		else sig[i] = 0x0;
	}

	if (serial.isInitialized()) serial.writeBytes(sig, steppers.size()+2);
	if (useOsc) sendBytesOnline(sig, steppers.size()+2);

}

virtualSteppingMotor &ofxKsmrStepManager::getMotor(int num){

	num = ofClamp(num, 0, steppers.size() - 1);
	return steppers[num];

}

virtualSteppingMotor &ofxKsmrStepManager::getMotor(string name){

	for (int i = 0;i < steppers.size();i++){
		if (steppers[i].getName() == name){
			return steppers[i];
		}
	}
	return steppers[0];

}

void ofxKsmrStepManager::resetAllDevices(){
	for (int i = 0;i < 4;i++)
		sendSPIByteAll(0x00);

	sendSPIByteAll(0xc0);
}

void ofxKsmrStepManager::setupEasy(){

	resetAllDevices();

	sendSPIByteAll(0x07);//最大回転スピード
	sendSPIByteAll(0x00);
	sendSPIByteAll(0x25);

	sendSPIByteAll(0x05);//加速度
	sendSPIByteAll(0x02);
	sendSPIByteAll(0x20);

	sendSPIByteAll(0x06);//減速度
	sendSPIByteAll(0x02);
	sendSPIByteAll(0x20);


	setMicroSteps(7);
	absPos(0);
}

void ofxKsmrStepManager::setMicroSteps(int involution0to7){
	unsigned char sig[2];

	sig[0] = 0x16;
	sig[1] = involution0to7;

	sendSPIPacketAll(sig, 2);

	microStepInv = involution0to7;
}

void ofxKsmrStepManager::selectStepperOne(int ch,bool enable){
	setStepperAll(false);
	steppers[ch].sendEnable = enable;
}

void ofxKsmrStepManager::setStepperAll(bool enable){
	for (int i = 0;i < steppers.size();i++){
		steppers[i].sendEnable = enable;
	}
}

void ofxKsmrStepManager::setStepperSingle(int ch,bool enable){
	ofClamp(ch, 0, steppers.size() - 1);
	steppers[ch].sendEnable = enable;
}

void ofxKsmrStepManager::absPos(int pos){

	int val = pos * pow(2.0f,microStepInv);
	unsigned char data[3];

	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	unsigned char sig[4];
	sig[0] = 0x01;
	sig[1] = data[2];
	sig[2] = data[1];
	sig[3] = data[0];

	sendSPIPacketSelected(sig, 4);

}

void ofxKsmrStepManager::run(int speed, bool dir){
	unsigned char sig[4];

	if (dir) sig[0] = 0x50;
	else	sig[0] = 0x51;
	
	int val = speed;
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sig[1] = data[2];
	sig[2] = data[1];
	sig[3] = data[0];

	sendSPIPacketSelected(sig, 4);
}

void ofxKsmrStepManager::move(int step, bool dir){
	unsigned char sig[4];

	if (dir) sig[0] = 0x40;
	else	sig[0] = 0x41;

	int val = step * pow(2.0f,microStepInv);
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sig[1] = data[2];
	sig[2] = data[1];
	sig[3] = data[0];

	sendSPIPacketSelected(sig, 4);

}

void ofxKsmrStepManager::multi_go_to(int *pos){

	int signal_unitL = 2 + steppers.size();

	unsigned char signal[4*signal_unitL];
	unsigned char datas[steppers.size()][3];

	for (int i = 0;i < steppers.size();i++){

		int val = pos[i] * pow(2.0f,microStepInv);

		for (int j = 0;j < 3;j++){
			datas[i][j] = val & 0xFF;
			val = val >> 8;
		}

	}

	for (int i = 0;i < 4;i++){
		signal[i*signal_unitL] = 0x02;
		signal[i*signal_unitL + 1] = steppers.size();
	}

	for (int i = 0;i < steppers.size();i++){
		signal[0*signal_unitL + 2 + i] = steppers[i].sendEnable ? 0x60 : 0x00;
		signal[1*signal_unitL + 2 + i] = steppers[i].sendEnable ? datas[i][2] : 0x00;
		signal[2*signal_unitL + 2 + i] = steppers[i].sendEnable ? datas[i][1] : 0x00;
		signal[3*signal_unitL + 2 + i] = steppers[i].sendEnable ? datas[i][0] : 0x00;
	}

	sendSPIMultiByte(signal, 4 * signal_unitL);
}

void ofxKsmrStepManager::go_to(int pos){

	unsigned char sig[4];

	sig[0] = 0x60;

	int val = pos * pow(2.0f,microStepInv);
	unsigned char data[3];
	for (int i = 0;i < 3;i++){
		data[i] = val & 0xFF;
		val = val >> 8;
	}

	sig[1] = data[2];
	sig[2] = data[1];
	sig[3] = data[0];

	sendSPIPacketSelected(sig, 4);

}

void ofxKsmrStepManager::softStop(){
	sendSPIByteSelected(0xb0);
}

void ofxKsmrStepManager::hardStop(){
	sendSPIByteSelected(0xb8);
}

void ofxKsmrStepManager::gohome(){
	sendSPIByteSelected(0x70);
}

void ofxKsmrStepManager::setupEasyFromPreset(ofxKsmrStepPreset preset){

	resetAllDevices();

	if (preset == KSMR_STEP_P_PMSA_B56D5){
		setParam_maxSpeed(0x0075);
		setParam_Accel(0x0010);
		setParam_Decel(0x0010);
		setMicroSteps(7);

		unsigned char sig[2];

		sig[0] = 0x0B;	sig[1] = 0x1F;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x0C;	sig[1] = 0x1F;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x09;	sig[1] = 0x1F;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x0A;	sig[1] = 0x1F;
		sendSPIPacketAll(sig, 2);
	}

	if (preset == KSMR_STEP_SM_42BYG011_25){
		setParam_maxSpeed(0x0025);
		setParam_Accel(0x0041);
		setParam_Decel(0x0041);

		unsigned char sig[2];

		sig[0] = 0x0B;	sig[1] = 0xFF;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x0C;	sig[1] = 0xFF;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x09;	sig[1] = 0xFF;
		sendSPIPacketAll(sig, 2);

		sig[0] = 0x0A;	sig[1] = 0xFF;
		sendSPIPacketAll(sig, 2);

		setMicroSteps(0);
	}

	absPos(0);
}

void ofxKsmrStepManager::setParam_maxSpeed(int bit_10){
	unsigned char sig[3];

	sig[0] = 0x07;
	sig[1] = (bit_10 >> 8) & 0x03;
	sig[2] = bit_10 & 0xFF;

	sendSPIPacketAll(sig, 3);

}

void ofxKsmrStepManager::setParam_minSpeed(int bit_13){

	unsigned char sig[3];

	sig[0] = 0x08;
	sig[1] = (bit_13 >> 8) & 31;
	sig[2] = bit_13 & 0xFF;

	sendSPIPacketAll(sig, 3);

}

void ofxKsmrStepManager::setParam_Accel(int bit_12){

	unsigned char sig[3];

	sig[0] = 0x05;
	sig[1] = (bit_12 >> 8) & 0x0F;
	sig[2] = bit_12 & 0xFF;

	sendSPIPacketAll(sig, 3);

}

void ofxKsmrStepManager::setParam_Decel(int bit_12){

	unsigned char sig[3];

	sig[0] = 0x06;
	sig[1] = (bit_12 >> 8) & 0x0F;
	sig[2] = bit_12 & 0xFF;

	sendSPIPacketAll(sig, 3);

}

void ofxKsmrStepManager::setParam_AbsPos(int bit_22){
	absPos(bit_22);
}

void ofxKsmrStepManager::sendBytesOnline(unsigned char *buffer, int length){

	if (useOsc){
		ofxOscMessage m;
		m.setAddress("/dp/hakoniwa/Ksmrmotor");

		if (sendByteSimply){
			m.addIntArg(length);

			for (int i = 0;i < length;i++){
				m.addIntArg(int(buffer[i]));
			}

			sender.sendMessage(m);
			
		}else{

			//上位8ビットにlength
			//残り上位から順番にバイト列を格納
			int32_t data  = 0;
			data += (length << 24);

			bool notAdd = false;
			for (int i = 1;i < length + 1;i++){
				data += buffer[i - 1] << (24 - 8 * i);
				notAdd = true;

				if (i % 4 == 3){
					m.addIntArg(data);
					data = 0;
					notAdd = false;
				}
			}
			if (notAdd) m.addIntArg(data);

			
			sender.sendMessage(m);

		}
	}

}