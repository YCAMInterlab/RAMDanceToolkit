#include "ControlSegment.h"
	

#pragma mark -
#pragma mark constructor

ControlSegment::ControlSegment()
{
	reset();
}



#pragma mark -
#pragma mark constructor

ofxUICanvasPlus* ControlSegment::setup(ofxUICanvasPlus* panel, const ramNodeArray &NA)
{
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(NA.getName(), OFX_UI_FONT_MEDIUM));
	child->addSpacer(width, 2);
	
	
	// Icons
	child->addWidgetDown(new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show"));
	child->addWidgetRight(new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"reset"));
	child->addWidgetRight(new ofxUIImageToggle(32, 32, &bRecording, ramToResourcePath("Images/record.png"),"record"));
	
	
	/// actor color
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	child->addSlider("R", 0, 1, &jointColor.r, 63, height);
	child->addSlider("G", 0, 1, &jointColor.g, 63, height);
	child->addSlider("B", 0, 1, &jointColor.b, 63, height);
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
	
	/// actor position
	child->add2DPad("Position",
					ofPoint(-500, 500),
					ofPoint(-500, 500),
					&position,
					width, 100);
	
	ofAddListener(child->newGUIEvent, this, &ControlSegment::onValueChanged);
	
	child->autoSizeToFitWidgets();
	panel->addWidget( child );
	
	return child;
}

void ControlSegment::reset()
{
	bHideActor = false;
	bNeedsResetPos = false;
	bRecording = false;
	
	jointColor = ofFloatColor(1.0, 0.15, 0.4);
	position = ofPoint(0, 0);
}

void ControlSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string name = e.widget->getName();
	
	cout << name << endl;
	cout << position << endl;
}

