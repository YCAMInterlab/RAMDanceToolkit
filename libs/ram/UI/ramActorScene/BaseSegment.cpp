#include "BaseSegment.h"



string BaseSegment::getName() const
{
    return name;
}

#pragma mark -
#pragma mark save & load from cache file


void BaseSegment::loadCache()
{
	if ( !ofFile::doesFileExist(getCacheFilePath()) ) return;
	
	XML.clear();
	XML.loadFile(getCacheFilePath());
	
	/// color
	XML.pushTag("color");
	jointColor.r = XML.getValue("r", 0.8);
	jointColor.g = XML.getValue("g", 0.8);
	jointColor.b = XML.getValue("b", 0.8);
	XML.popTag();
	
	/// position
	XML.pushTag("position");
	position.x = XML.getValue("x", 0.0);
	position.y = XML.getValue("y", 0.0);
	XML.popTag();
	
	/// boolean state
	XML.pushTag("state");
	bHideActor = XML.getValue("hideActor", 0);
	btnHideActor->setValue(bHideActor);
	btnHideActor->stateChange();
	XML.popTag();
}

void BaseSegment::saveCache()
{
	XML.clear();
	
	/// color
	XML.addTag("color");
	XML.pushTag("color");
	XML.addValue("r", jointColor.r);
	XML.addValue("g", jointColor.g);
	XML.addValue("b", jointColor.b);
	XML.popTag();
	
	/// position
	XML.addTag("position");
	XML.pushTag("position");
	XML.addValue("x", position.x);
	XML.addValue("y", position.y);
	XML.popTag();
	
	/// boolean states
	XML.addTag("state");
	XML.pushTag("state");
	XML.addValue("hideActor", bHideActor);
	XML.popTag();
	
	XML.saveFile(getCacheFilePath());
}




#pragma mark -
#pragma mark protected methods

void BaseSegment::init()
{
    bHideActor = false;
    bNeedsResetPos = false;
    jointColor = ofFloatColor(1.0, 0.15, 0.4);
    position = ofPoint(0, 0);
}

string BaseSegment::getCacheFilePath() const
{
	return ramToResourcePath("Settings/Actors/color."+name+".xml");
}

