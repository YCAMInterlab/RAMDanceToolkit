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
#include "ramAnalyzer.h"
#include "ramGhost.h"
#include "ramPendulum.h"
#include "ramExpansion.h"
#include "ramUpsideDown.h"
#include "ramSession.h"
#include "ramStamp.h"
#include "ramNodeTransform.h"
#include "ramTimeFilter.h"
#include "ramLowPassFilter.h"

/// events
#include "ramTimeEvent.h"
#include "ramCollisionEvent.h"

/// utility
#include "ramControlPanel.h"
#include "ramCameraSettings.h"
#include "ramNodeFinder.h"

#define _USE_MATH_DEFINES
#include <math.h>