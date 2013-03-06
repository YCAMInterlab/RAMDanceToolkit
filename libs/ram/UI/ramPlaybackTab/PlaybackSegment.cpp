#include "PlaybackSegment.h"
#include "ramControlPanel.h"



#pragma mark -
#pragma mark constructor, destructor

PlaybackSegment::PlaybackSegment()
{
	reset();
	
	btnHideActor = new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show");
	btnResetActor = new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"reset");
	btnDeleteActor = new ofxUIImageToggle(32, 32, &bDelete, ramToResourcePath("Images/delete.png"),"delete");
}

PlaybackSegment::~PlaybackSegment()
{
	delete btnHideActor;
	delete btnResetActor;
	delete btnDeleteActor;
}




#pragma mark -
#pragma mark public methods

ofxUICanvasPlus* PlaybackSegment::createPanel(const ramSession &session)
{
	name = session.getNodeArrayName();
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
	
	
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	child->addSpacer(width, 2);
	
	
	/// Icons
	child->addWidgetDown(btnHideActor);
	child->addWidgetRight(btnResetActor);
	child->addWidgetRight(btnDeleteActor);
	
	
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
	
	ofAddListener(child->newGUIEvent, this, &PlaybackSegment::onValueChanged);
	
	child->autoSizeToFitWidgets();
	
	return child;
}

void PlaybackSegment::loadCache()
{
//	if ( !ofFile::doesFileExist(getXMLFilePath()) ) return;
//	
//	XML.clear();
//	XML.loadFile(getXMLFilePath());
//	
//	/// color
//	XML.pushTag("color");
//	jointColor.r = XML.getValue("r", 0.8);
//	jointColor.g = XML.getValue("g", 0.8);
//	jointColor.b = XML.getValue("b", 0.8);
//	XML.popTag();
//	
//	/// position
//	XML.pushTag("position");
//	position.x = XML.getValue("x", 0.0);
//	position.y = XML.getValue("y", 0.0);
//	XML.popTag();
//	
//	/// boolean state
//	XML.pushTag("state");
//	bHideActor = XML.getValue("hideActor", 0);
//	btnHideActor->setValue(bHideActor);
//	btnHideActor->stateChange();
//	XML.popTag();
}

void PlaybackSegment::saveCache()
{
//	XML.clear();
//	
//	/// color
//	XML.addTag("color");
//	XML.pushTag("color");
//	XML.addValue("r", jointColor.r);
//	XML.addValue("g", jointColor.g);
//	XML.addValue("b", jointColor.b);
//	XML.popTag();
//	
//	/// position
//	XML.addTag("position");
//	XML.pushTag("position");
//	XML.addValue("x", position.x);
//	XML.addValue("y", position.y);
//	XML.popTag();
//	
//	/// boolean states
//	XML.addTag("state");
//	XML.pushTag("state");
//	XML.addValue("hideActor", bHideActor);
//	XML.popTag();
//	
//	XML.saveFile(getXMLFilePath());
}



#pragma mark -
#pragma mark Events

void PlaybackSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string widgetName = e.widget->getName();
	
	saveCache();
}




#pragma mark -
#pragma mark private methods

void PlaybackSegment::reset()
{
	bHideActor = false;
	bNeedsResetPos = false;
	bDelete = false;
	
	jointColor = ofFloatColor(1.0, 0.15, 0.4);
	position = ofPoint(0, 0);
}

const string PlaybackSegment::getXMLFilePath() const
{
	return ramToResourcePath("Settings/Actors/color."+name+".xml");
}

