#pragma once

#include "ofxUITabbedCanvas.h"
//#include "ramTSVCoder.h"

class ramPlaybackTab : public ofxUITab
{

	
//	ramTSVCoder coder;
	
	
public:
	
	ramPlaybackTab()
	:ofxUITab("Playback", false) {
		
		ofAddListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
		
	}
	
	
	void onFileDrop(ofDragInfo &e)
	{
		for(int i=0; i<e.files.size(); i++)
		{
			const string filePath = e.files.at(i);
//			ramSession session = coder.load(filePath);
		}
	}
};