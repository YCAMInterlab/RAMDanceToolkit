#include "ControlSegment.h"

#pragma mark -
#pragma mark constructor, destructor

ControlSegment::ControlSegment(string segmentName)
{
    name = segmentName;
    
	init();
	
	btnHideActor = new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show");
	btnResetActor = new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"reset");
	btnRecordActor = new ofxUIImageToggle(32, 32, &bRecording, ramToResourcePath("Images/record.png"),"record");
}

ControlSegment::~ControlSegment()
{
	delete btnHideActor;
	delete btnResetActor;
	delete btnRecordActor;
}




#pragma mark -
#pragma mark public methods

ramActorUISegmentType ControlSegment::getType()
{
    return RAM_UI_SEGMENT_TYPE_CONTROL;
}

ofxUICanvasPlus* ControlSegment::createPanel(const string targetName)
{
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
    const float padding = ramGetGUI().kXInit*2;
	
	
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(targetName, OFX_UI_FONT_MEDIUM));
	child->addSpacer(width-padding, 2);
	
	
	/// Icons
	child->addWidgetDown(btnHideActor);
	child->addWidgetRight(btnResetActor);
	child->addWidgetRight(btnRecordActor);
	btnHideActor->setValue(!ramShowActorsEnabled());
	
    
	/// actor color
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	child->addSlider("R", 0, 1, &jointColor.r, 61, height);
	child->addSlider("G", 0, 1, &jointColor.g, 61, height);
	child->addSlider("B", 0, 1, &jointColor.b, 60, height);
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
	
	/// actor position
	child->add2DPad("Position",
					ofPoint(-500, 500),
					ofPoint(-500, 500),
					&position,
					width-padding, 100);
	
	ofAddListener(child->newGUIEvent, this, &ControlSegment::onValueChanged);
	
	child->autoSizeToFitWidgets();
	
	return child;
}



void ControlSegment::toggleRecording(const bool bStart)
{
	if (bStart)
	{
		/// rec start
		session.startRecording();
	}
	else
	{
		/// rec stop
		session.stopRecording();
		
		
		/// save data as tsv file
		ofFileDialogResult result = ofSystemSaveDialog(ofGetTimestampString("%Y.%m.%d_%H.%M.%S-") + name + ".tsv" , "Save motion data.");
		
		if(result.bSuccess)
		{
			coder.save(session, result.getPath());
		}
	}
	
	/// update button state programmatically
	bRecording = bStart;
	btnRecordActor->setValue(bStart);
	btnRecordActor->stateChange();
}



#pragma mark -
#pragma mark Events

void ControlSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string widgetName = e.widget->getName();
	
	if (widgetName == "record")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *)e.widget;
		const bool value = toggle->getValue();
		
		toggleRecording(value);
	}
	else
	{
		saveCache();
	}
}





#pragma mark -
#pragma mark getter, setter
bool ControlSegment::isRecording()
{
    return bRecording;
}



#pragma mark -
#pragma mark private methods

void ControlSegment::init()
{
    BaseSegment::init();
	bRecording = false;
}


