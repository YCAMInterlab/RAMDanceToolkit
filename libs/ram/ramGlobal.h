#pragma once

// TODO: add setting panel, shadow matrix

#include "ofMain.h"

#include "ramControlPanel.h"

class ramGlobalContext
{
public:
	
	ramControlPanel& getGUI();
	
	void init();

private:
	
	ramControlPanel *gui;
	
};

ramGlobalContext& ramGlobal();
string ramToResourcePath(string path);