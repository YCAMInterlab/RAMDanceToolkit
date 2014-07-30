// 
// BaseSegment.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "BaseSegment.h"



string BaseSegment::getName() const
{
    return name;
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
	return ramToResourcePath("Settings/cache/color."+name+".xml");
}

