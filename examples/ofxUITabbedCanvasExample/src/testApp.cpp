#include "testApp.h"




//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    //set some sketch parameters
    //Background Color
    red = 233;
    blue = 240;
    green = 52;
    alpha = 200;
    radius = 150;
    drawFill = true;
    backgroundColor = ofColor(233, 52, 27);
    resolution = 30;
    position = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);
    ofSetCircleResolution(resolution);
    
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
	
    guiA = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());
	guiA->addWidgetDown(new ofxUILabel("SIMPLE EXAMPLE", OFX_UI_FONT_LARGE));
    
    guiA->addSpacer(length, 2);
    guiA->addWidgetDown(new ofxUILabel("BACKGROUND CONTROL", OFX_UI_FONT_MEDIUM));
    guiA->addSlider("BGR", 0, 255, backgroundColor.r, 95, dim);
    guiA->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiA->addSlider("BGG", 0, 255, backgroundColor.g, 95, dim);
    guiA->addSlider("BGB", 0, 255, backgroundColor.b, 95, dim);
    guiA->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiA->addSpacer(length, 2);
    
    guiA->addWidgetDown(new ofxUILabel("CIRCLE CONTROL", OFX_UI_FONT_MEDIUM));
    guiA->addSlider("RED", 0.0, 255.0, red, length,dim);
	guiA->addSlider("GREEN", 0.0, 255.0, green, length,dim);
    guiA->addSlider("BLUE", 0.0, 255.0, blue, length,dim);
    guiA->addSlider("ALPHA", 0.0, 255.0, alpha, length,dim);
    
    guiB = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());
    guiB->addSlider("RADIUS", 0.0, 600.0, radius, length,dim);
	guiB->addSlider("RESOLUTION", 3, 60, resolution, length,dim);
    
    guiB->addWidgetDown(new ofxUILabelToggle(drawFill, "DRAW FILL", OFX_UI_FONT_MEDIUM));
    
    float padWidth = length;
    float padHeight = length*((float)ofGetHeight()/(float)ofGetWidth());
    
    guiB->addWidgetDown(new ofxUI2DPad(padWidth, padHeight, ofPoint(padWidth*.5, padHeight*.5), "POSITION"));
    
    guiB->addSpacer(length, 2);
    guiB->addWidgetDown(new ofxUILabel("HIDE & SHOW GUI BY PRESSING 'g'", OFX_UI_FONT_MEDIUM));
    guiB->addWidgetDown(new ofxUILabel("MOUSE OVER A SLIDER AND", OFX_UI_FONT_MEDIUM));
    guiB->addWidgetDown(new ofxUILabel("PRESS UP, DOWN, LEFT, RIGHT", OFX_UI_FONT_MEDIUM));
    
    ofAddListener(guiA->newGUIEvent,this,&testApp::guiEvent);
    ofAddListener(guiB->newGUIEvent,this,&testApp::guiEvent);
	ofBackground(backgroundColor);
    
    tabbedCanvas.add(guiA);
    tabbedCanvas.add(guiB);
    
    tabbedCanvas.loadSettings("GUI/guiSettings.xml");
}

//--------------------------------------------------------------
void testApp::update()
{
    
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(backgroundColor);
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetColor(red, green, blue, alpha);
    if(drawFill)
    {
        ofFill();
    }
    else
    {
        ofNoFill();
    }
    ofCircle(position.x,position.y, radius);
    
    
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		red = slider->getScaledValue();
	}
	else if(name == "GREEN")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		green = slider->getScaledValue();
	}
	else if(name == "BLUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		blue = slider->getScaledValue();
	}
	else if(name == "BGR")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.r = rslider->getScaledValue();
	}
	else if(name == "BGG")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.g = rslider->getScaledValue();
	}
	else if(name == "BGB")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.b = rslider->getScaledValue();
	}
	else if(name == "ALPHA")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		alpha = slider->getScaledValue();
	}
	else if(name == "RADIUS")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		radius = slider->getScaledValue();
	}
	else if(name == "RESOLUTION")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		resolution = slider->getScaledValue();          //gets the value from the specified range, otherwise you can get a normalized value (0.0 -> 1.0);
        ofSetCircleResolution(resolution);
        slider->setValue(resolution);                   //shows the int value on the slider
	}
    else if(name == "POSITION")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
		position.x = pad->getPercentValue().x*ofGetWidth();
		position.y = pad->getPercentValue().y*ofGetHeight();
	}
    else if(name == "DRAW FILL")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        drawFill = toggle->getValue();
    }
}
//--------------------------------------------------------------
void testApp::exit()
{
    //tabbedCanvas.saveSettings("GUI/guiSettings.xml");
}
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    switch (key)
    {
        case '1': tabbedCanvas.select(0); break;
        case '2': tabbedCanvas.select(1); break;
        case 'g': tabbedCanvas.toggleVisible(); break;
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}