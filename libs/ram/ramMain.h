// 
// ramMain.h - RAMDanceToolkit
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

#pragma once

#include "ofMain.h"

/// app
#include "ramBaseApp.h"
#include "ramActorManager.h"
#include "ramCameraManager.h"

/// physics
#include "ramPrimitive.h"
#include "ramSoftBodyPrimitive.h"

/// scenes
#include "ramSceneManager.h"
#include "ramBaseScene.h"

/// graphics
#include "ramGraphics.h"
#include "ramNodeLine.h"
#include "ramGraphView.h"

/// filters
#include "ramGhost.h"
#include "ramPendulum.h"
#include "ramExpansion.h"
#include "ramUpsideDown.h"
#include "ramSession.h"
#include "ramStamp.h"
#include "ramNodeTransform.h"
#include "ramTimeFilter.h"
#include "ramLowPassFilter.h"

/// recognizers
#include "ramRecognizer.h"

/// events
#include "ramTimeEvent.h"
#include "ramCollisionEvent.h"

/// utility
#include "ramControlPanel.h"
#include "ramCameraSettings.h"
#include "ramNodeFinder.h"
#include "ramTSVCoder.h"

#define _USE_MATH_DEFINES
#include <math.h>