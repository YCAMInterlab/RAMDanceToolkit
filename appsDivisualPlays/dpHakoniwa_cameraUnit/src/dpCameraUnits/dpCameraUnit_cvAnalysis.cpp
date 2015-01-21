//
//  dpCameraUnit_cvAnalysis.cpp
//  dpHakoniwa_cameraUnit
//
//  Created by Ovis aries on 2014/12/19.
//
//

#include "dpCameraUnit_cvAnalysis.h"

dpCameraUnit_cvAnalysis::dpCameraUnit_cvAnalysis(){

	mGui.setup();
	mGui.addLabel("Analysis",OFX_UI_FONT_LARGE);
	mGui.addSpacer();
	mGui.addLabel("Hakoniwa Name");
	mGui.addTextInput("hakoniwaName", "")->setAutoClear(false);
	mGui.addButton("loadPreset", false);
	mGui.addButton("savePreset", false);
	mGui.addLabel("OSCOption");
	mGui.addSpacer();
	mGui.addToggle("SendOSC",		&mEnableSendOSC);
	mGui.addTextInput("OSCAddress", "localhost")->setAutoClear(false);
	mGui.addLabel("OSCSplit", "OSCSplit");
	oscMatrixUI = mGui.addToggleMatrix("OSCSpliter", 1, 9);
	mGui.addTextInput("OSCPort", "10000")->setAutoClear(false);
	mGui.addToggle("ContourFinder",		&mEnableContourFinder);
	mGui.addToggle("OptFlow",			&mEnableOptFlow);
	mGui.addToggle("Flow_FarneBack",	&mEnableOptFlowFarne);
	mGui.addToggle("Mean",				&mEnableMean);
	mGui.addToggle("Pixelate",			&mEnablePixelate);
	mGui.addToggle("RGBPixelate",		&mEnablePixelateRGB);
	mGui.addIntSlider("Pixlthreshold", 0, 255, &mParamPixelate_thr)->setValue(128);
	mGui.addIntSlider("Res_X", 1, 60, &mParamPixelate_ResX);
	mGui.addIntSlider("Res_Y", 1, 60, &mParamPixelate_ResY);
//	mGui.addToggle("FAST",				&mEnableFAST);
//	mGui.addToggle("Histgram",			&mEnableHistgram);
	mGui.addSpacer();
	mGui.addToggle("ViewSource", &mViewSource);

	mGui.addLabel("ContourFinder");
	mGui.addSpacer();
//	mGui.addToggle("Blob"	, false);
//	mGui.addToggle("Pts"	, false);
//	mGui.addToggle("Simplify"		, &mParamCF_Simplify);
//	mGui.addToggle("UseTargetColor"	, &mParamCF_UseTargetColor);
	mGui.addRangeSlider("Area", 0.0, 10000.0, &mParamCF_MinArea, &mParamCF_MaxArea);
	mGui.addSlider("MaxBlobNum", 0.0, 500.0, &mParamCF_MaxBlobNum);
	mGui.addSlider("Threshold", 0.0, 255.0, &mParamCF_Threshold)->setValue(200);
	mGui.addLabel("OptFlow");
	mGui.addSpacer();
	mGui.addSlider("filter_Speed", 0.0, 100.0, &mOptFlow_filterSpd);
	mGui.addSlider("smooth", 1.0, 10.0, &mOptFlowSmooth);

	ofAddListener(mGui.newGUIEvent, this, &dpCameraUnit_cvAnalysis::guiEvent);
	
	imgRefColor = NULL;
	imgRefGray = NULL;

	mGui.autoSizeToFitWidgets();

	//OSC Initialize
	addUI = (ofxUITextInput*)mGui.getWidget("OSCAddress");
	portUI = (ofxUITextInput*)mGui.getWidget("OSCPort");

	int pt = ofToInt(portUI->getTextString());
	string address = addUI->getTextString();

	sender.setup(address, pt);


	mEnableSendOSC			= false;
	mEnableContourFinder	= false;
	mEnableOptFlow			= false;
	mEnableFAST				= false;
	mEnableMean				= false;
	mEnableHistgram			= false;
	
	mParamPixelate_ResX		= 10;
	mParamPixelate_ResY		= 10;
	
	mOptFlow_filterSpd = 100.0;

	mContFinder.getTracker().setPersistence(15);
	mContFinder.getTracker().setMaximumDistance(16);

	mParamCF_MaxArea = 1000.0;
	mParamCF_MinArea = 50.0;

}

dpCameraUnit_cvAnalysis::~dpCameraUnit_cvAnalysis(){

}

void dpCameraUnit_cvAnalysis::update(ofImage &pixColor, ofImage &pixGray,bool isFrameNew){

	if (ofGetFrameNum() % 30 == 0){
		ofxUITextInput* ti = ((ofxUITextInput*)(mGui.getWidget("hakoniwaName")));
		hakoniwa_name = ti->getTextString();
	}

	imgRefColor = &pixColor;
	imgRefGray = &pixGray;
    
    float width = pixGray.getWidth();
    float height = pixGray.getHeight();

#pragma mark ContourFinder
	if (mEnableContourFinder){
		ofxCv::RectTracker& tracker = mContFinder.getTracker();

		mContFinder.setMaxArea(mParamCF_MaxArea);
		mContFinder.setMinArea(mParamCF_MinArea);
		mContFinder.setSimplify(mParamCF_Simplify);
		mContFinder.setTargetColor(mParamCF_targColor);
//		mContFinder.setUseTargetColor(mParamCF_UseTargetColor);
		mContFinder.setThreshold(mParamCF_Threshold);

		mContFinder.findContours(pixGray);

		ofxOscMessage bRectM;
		ofxOscMessage blobM;
		ofxOscMessage areaM;

		bRectM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/contour/boundingRect");
		blobM .setAddress("/dp/cameraUnit/"+hakoniwa_name+"/contour/blob");
		areaM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/contour/sarea");

		bRectM.addIntArg(mContFinder.getContours().size());
		blobM.addIntArg(mContFinder.getContours().size());
		areaM.addIntArg(mContFinder.getContours().size());

//		mContFinder.getContourArea(0)

		for (int i = 0;i < mContFinder.getContours().size();i++){
			ofRectangle rt = ofxCv::toOf(mContFinder.getBoundingRect(i));
			bRectM.addIntArg(mContFinder.getLabel(i));
			bRectM.addFloatArg(ofClamp(rt.x / width,-1.0,1.0));
			bRectM.addFloatArg(ofClamp(rt.y / height,-1.0,1.0));
			bRectM.addFloatArg(ofClamp(rt.width / width,-1.0,1.0));
			bRectM.addFloatArg(ofClamp(rt.height / height,-1.0,1.0));

			//Set Blobs
			int cnt = 0;
			int blob_step = 1;
			for (int j = 0;j < mContFinder.getContour(i).size();j+=blob_step) cnt++;
			
			blobM.addIntArg(cnt);
			
			for (int j = 0;j < mContFinder.getContour(i).size();j++){
				ofVec2f pt = ofxCv::toOf(mContFinder.getContour(i)[j]);
				blobM.addFloatArg(ofClamp(pt.x / width,-1.0,1.0));
				blobM.addFloatArg(ofClamp(pt.y / height,-1.0,1.0));
			}

			areaM.addFloatArg(mContFinder.getContourArea(i));
		}

		if (mEnableSendOSC){
			sendMessageMulti(bRectM);
			sendMessageMulti(blobM);
			sendMessageMulti(areaM);
		}

	}

#pragma mark mean
	if (mEnableMean){
		means = ofxCv::mean(ofxCv::toCv(pixColor));
		means_gray = ofxCv::mean(ofxCv::toCv(pixGray));

		if (mEnableSendOSC){
			ofxOscMessage m;
			m.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/mean");
			m.addIntArg(means[0]);
			m.addIntArg(means[1]);
			m.addIntArg(means[2]);
			m.addIntArg(means_gray[0]);
			sendMessageMulti(m);
		}
	}

#pragma mark FarneBack
	if (mEnableOptFlowFarne){

		if ((ofxCv::mean(ofxCv::toCv(pixGray))[0] > 1.0f) &&
			(isFrameNew)) mOptFlowFarne.calcOpticalFlow(pixGray);


	}

#pragma mark Pixelate
	if (mEnablePixelate){
		int res_x = mParamPixelate_ResX;
		int res_y = mParamPixelate_ResY;

		int64_t pixelInt = 0;
		int64_t Pixelcounter = 0;
		ofxOscMessage pixelateM;
		pixelateM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/pixelate");
		pixelateM.addIntArg(res_x);
		pixelateM.addIntArg(res_y);

		debug_px.clear();

		for (int64_t j = 0;j < res_y;j++){
			for (int64_t i = 0;i < res_x;i++){
				bool pix = (pixGray.getColor(width/res_x * i, height/res_y * j).r > mParamPixelate_thr);
				debug_px.push_back(pix);
				int64_t tg = int64_t(pix) << (Pixelcounter % 64);
				pixelInt = pixelInt | tg;
				Pixelcounter++;

				if (Pixelcounter % 64 == 0){
					pixelateM.addInt64Arg(pixelInt);
					pixelInt = 0;
				}
			}
		}
		if (Pixelcounter % 64 != 0){
			pixelateM.addInt64Arg(pixelInt);
		}

		if (mEnableSendOSC) sendMessageMulti(pixelateM);

	}

#pragma mark PixelateRGB
	if (mEnablePixelateRGB){
		int res_x = mParamPixelate_ResX;
		int res_y = mParamPixelate_ResY;

		int64_t pixelInt[3] = {0,0,0};
		int64_t Pixelcounter= 0;

		ofxOscMessage pixelateM[3];
		pixelateM[0].setAddress("/dp/cameraUnit/"+hakoniwa_name+"/pixelate/r");
		pixelateM[1].setAddress("/dp/cameraUnit/"+hakoniwa_name+"/pixelate/g");
		pixelateM[2].setAddress("/dp/cameraUnit/"+hakoniwa_name+"/pixelate/b");

		for (int i = 0;i < 3;i++){
			debug_rgb[i].clear();
			pixelateM[i].addIntArg(res_x);
			pixelateM[i].addIntArg(res_y);
		}

		for (int64_t j = 0;j < res_y;j++){
			for (int64_t i = 0;i < res_x;i++){
				bool pix[3];
				pix[0] = (pixColor.getColor(width/res_x * i, height/res_y * j).r > mParamPixelate_thr);
				pix[1] = (pixColor.getColor(width/res_x * i, height/res_y * j).g > mParamPixelate_thr);
				pix[2] = (pixColor.getColor(width/res_x * i, height/res_y * j).b > mParamPixelate_thr);

				for (int o = 0;o < 3;o++){
					debug_rgb[o].push_back(pix[o]);
					int64_t tg = int64_t(pix[o]) << (Pixelcounter % 64);
					pixelInt[o] = pixelInt[o] | tg;
				}
				Pixelcounter++;


				if (Pixelcounter % 64 == 0){
					for (int o = 0;o < 3;o++){
						pixelateM[o].addInt64Arg(pixelInt[o]);
						pixelInt[o] = 0;
					}
				}
			}
		}

		for (int i = 0;i < 3;i++){
			if (Pixelcounter % 64 != 0){
				pixelateM[i].addInt64Arg(pixelInt[i]);
			}
			sendMessageMulti(pixelateM[i]);
		}

	}

#pragma mark PyrLK
	if (mEnableOptFlow){
		if ((ofxCv::mean(ofxCv::toCv(pixGray))[0] > 5.0f) &&
			(ofxCv::mean(ofxCv::toCv(pixGray))[0] < 250.0f) &&
			(isFrameNew)){
			float f = ofxCv::mean(ofxCv::toCv(pixGray))[0];
			mOptFlow.calcOpticalFlow(pixGray);
		}
		
		if ((ofGetFrameNum() % 150 == 0) ||
			(ofGetKeyPressed(' '))) mOptFlow.resetFeaturesToTrack();

		vector <ofVec2f> mot = mOptFlow.getMotion();

		for (int i = 0;i < 10;i++) mOptFlow_sumVecs[i].set(0.0,0.0);
		
		mOptFlow_angleVec = ofVec2f(0.0,0.0);

		for (int i = 0;i < mot.size();i++){
			if (mot[i].lengthSquared() < pow(mOptFlow_filterSpd,2.0f)){
				mOptFlow_sumVecs[i % 10] += mot[i];
				mOptFlow_angleVec += mot[i];
				mOptFlow_sumVecs[0].x;
			}
		}
		mOptFlow_angleVec /= 100.0;

		for (int i = 0;i < 10;i++){
			mOptFlow_smoothVecs[i] += (mOptFlow_sumVecs[i] - mOptFlow_smoothVecs[i]) / mOptFlowSmooth;
		}

		if (mEnableSendOSC){
			ofxOscMessage feat;
			feat.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/features");
			feat.addIntArg(mOptFlow.getFeatures().size());
			for (int i = 0;i < mOptFlow.getFeatures().size();i++){
				feat.addFloatArg(ofClamp(mOptFlow.getFeatures()[i].x / width,-1.0,1.0));
				feat.addFloatArg(ofClamp(mOptFlow.getFeatures()[i].y / height,-1.0,1.0));
				if (mOpt_previous.size() > i){
					feat.addFloatArg(ofClamp((mOptFlow.getFeatures()[i].x -
									  mOpt_previous[i].x) / width,-1.0,1.0));
					feat.addFloatArg(ofClamp((mOptFlow.getFeatures()[i].y -
									  mOpt_previous[i].y) / height,-1.0,1.0));

				}else{
					feat.addFloatArg(0.0);
					feat.addFloatArg(0.0);
				}
			}

			ofxOscMessage vectorM;
			vectorM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector");
			for (int i = 0;i < 10;i++){
				vectorM.addFloatArg(mOptFlow_smoothVecs[i].x);
				vectorM.addFloatArg(mOptFlow_smoothVecs[i].y);
			}

			ofxOscMessage lengthM;
			lengthM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/length");
			for (int i = 0;i < 10;i++){
				lengthM.addFloatArg(mOptFlow_smoothVecs[i].length());
			}
			
			ofxOscMessage totalM;
			totalM.setAddress("/dp/cameraUnit/"+hakoniwa_name+"/vector/total");
			totalM.addFloatArg(mOptFlow_angleVec.x);
			totalM.addFloatArg(mOptFlow_angleVec.y);

			sendMessageMulti(feat);
			sendMessageMulti(vectorM);
			sendMessageMulti(lengthM);
			sendMessageMulti(totalM);
			
		}
		mOpt_previous = mOptFlow.getFeatures();

	}

}

void dpCameraUnit_cvAnalysis::drawUI(int x, int y){
	mGui.setPosition(x, y);
}

void dpCameraUnit_cvAnalysis::drawThumbnail(int x, int y, float scale){
	
	ofPushMatrix();
	ofTranslate(x, y);
	glScaled(scale, scale, scale);
	if (mViewSource && imgRefGray != NULL) imgRefGray->draw(0,0);

	ofSetColor(255, 0, 0);
	for (int i = 0;i < mOptFlow.getFeatures().size();i++){
		if (mOptFlow.getMotion()[i].x != 0){
			ofCircle(mOptFlow.getFeatures()[i], 15);
		}
	}

	ofSetColor(255, 255, 0);
	if (mEnableContourFinder)	mContFinder.draw();
	if (mEnableOptFlow)			mOptFlow.draw();
	if (mEnableOptFlowFarne)	mOptFlowFarne.draw();

	ofSetColor(255);

	if (mEnablePixelate){
		for (int j = 0;j < mParamPixelate_ResY;j++){
			for (int i = 0;i < mParamPixelate_ResX;i++){
				if (debug_px[j*mParamPixelate_ResX+i]){
					ofRect(i*10, j*10, 10, 10);
				}
			}
		}
	}
	if (mEnablePixelateRGB){
		for (int j = 0;j < mParamPixelate_ResY;j++){
			for (int i = 0;i < mParamPixelate_ResX;i++){
				for (int o = 0;o < 3;o++){
					if (debug_rgb[o][j*mParamPixelate_ResX+i]){
						if (o == 0) ofSetColor(255,0,0);
						if (o == 1) ofSetColor(0,255,0);
						if (o == 2) ofSetColor(0,0,255);
						ofRect(i*10+o*3, j*10, 3, 10);
					}
				}
			}
		}
	}

	if (mEnableOptFlow){
		ofSetColor(255, 0, 0);
		ofPushMatrix();
		ofTranslate(160, 120);
		for (int i = 0;i < 10;i++){
			ofLine(0, 0, mOptFlow_smoothVecs[i].x*4.0, mOptFlow_smoothVecs[i].y*4.0);
			ofCircle(mOptFlow_smoothVecs[i], 3);
		}
		ofSetColor(0, 255, 0);
		ofLine(0, 0, mOptFlow_angleVec.x*30.0, mOptFlow_angleVec.y*30.0);
		ofPopMatrix();
	}

	ofSetColor(255, 0, 0);
	ofRect(0, imgRefGray->getHeight()     , means[0], 10);
	ofSetColor(0, 255, 0);
	ofRect(0, imgRefGray->getHeight() + 10, means[1], 10);
	ofSetColor(0, 0, 255);
	ofRect(0, imgRefGray->getHeight() + 20, means[2], 10);
	ofSetColor(255);
	ofRect(0, imgRefGray->getHeight() + 30, means_gray[0], 10);

	ofPopMatrix();

	ofSetColor(255);
}

void dpCameraUnit_cvAnalysis::draw(int x,int y){
	drawUI(x, y);
	drawThumbnail(x, ofGetHeight() - 300);
}

void dpCameraUnit_cvAnalysis::guiEvent(ofxUIEventArgs &ev){
	ofxUIWidget* w = ev.widget;

	if (w->getName() == "loadPreset"){
		loadPreset(hakoniwa_name);
		pairFXUnit->loadPreset(hakoniwa_name);
		cout << "Load Preset at:" << hakoniwa_name << endl;
	}
	if (w->getName() == "savePreset"){
		savePreset(hakoniwa_name);
		pairFXUnit->savePreset(hakoniwa_name);
		cout << "Save Preset at:" << hakoniwa_name << endl;
	}

	if (w->getName() == "hakoniwaName" && w->getState() == OFX_UI_STATE_DOWN){
		ofxUITextInput* ti = (ofxUITextInput*)w;
		ti->setTextString(ti->getTextString());
		hakoniwa_name = ti->getTextString();
	}
	
}

void dpCameraUnit_cvAnalysis::sendMessageMulti(ofxOscMessage &m){
	if (!mEnableSendOSC) return;

	string defAdd = addUI->getTextString();
	int defPort = ofToInt(portUI->getTextString());

	sender.setup(defAdd, defPort);
	sender.sendMessage(m);

	if (oscListPtr != NULL){
		for (int i = 0;i < oscListPtr->size();i++){

			if (oscMatrixUI->getState(0, i)){
				sender.setup((*oscListPtr)[i], defPort);
				sender.sendMessage(m);
			}
			
		}
	}else{
		cout << "oscListPtr is NULL" << endl;
	}
}

void dpCameraUnit_cvAnalysis::savePreset(string hakoniwaName){

	if (hakoniwaName == "") return;

	ofDirectory::createDirectory("Preset_"+hakoniwaName);
	mGui.saveSettings("Preset_"+hakoniwaName+"/AnalysisUIPreset.xml");

}

void dpCameraUnit_cvAnalysis::loadPreset(string hakoniwaName){

	mGui.loadSettings("Preset_"+hakoniwaName+"/AnalysisUIPreset.xml");
	ofxUITextInput* ti = ((ofxUITextInput*)(mGui.getWidget("hakoniwaName")));
	hakoniwa_name = ti->getTextString();

}