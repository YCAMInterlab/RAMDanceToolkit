//
//  Chaser_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "Chaser_ext.h"

void Chaser_ext::setupControlPanel()
{
	
	mex.setupControlPanel(this);
}

void Chaser_ext::update()
{
	mex.update();
}

void Chaser_ext::draw()
{
	ramBeginCamera();
	
	
	
	ramEndCamera();
	mex.draw();
}


void Chaser_ext::addNewBuffer(string name)
{
	
	buffers.insert(pair<string, ramNodeArrayBuffer>(name, ramNodeArrayBuffer()));
}