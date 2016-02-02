//
//  dpConstants.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/12.
//  Copyright (c) 2015年 YCAMInterlab. All rights reserved.
//

#include "dpConstants.h"

const ofColor dpColor::MAIN_COLOR       = ofColor(255, 50, 150);
const ofColor dpColor::PALE_PINK_LIGHT  = ofColor(255, 220, 235);
const ofColor dpColor::PALE_PINK_HEAVY  = ofColor(255, 150, 200);
const ofColor dpColor::DARK_PINK_LIGHT  = ofColor(200, 50, 120);
const ofColor dpColor::DARK_PINK_HEAVY  = ofColor(130, 50, 80);

const ofFloatColor dpFloatColor::MAIN_COLOR       = ofFloatColor(dpColor::MAIN_COLOR.r / 255.0, dpColor::MAIN_COLOR.g / 255.0, dpColor::MAIN_COLOR.b / 255.0);
const ofFloatColor dpFloatColor::PALE_PINK_LIGHT  = ofFloatColor(dpColor::PALE_PINK_LIGHT.r / 255.0, dpColor::PALE_PINK_LIGHT.g / 255.0, dpColor::PALE_PINK_LIGHT.b / 255.0);
const ofFloatColor dpFloatColor::PALE_PINK_HEAVY  = ofFloatColor(dpColor::PALE_PINK_HEAVY.r / 255.0, dpColor::PALE_PINK_HEAVY.g / 255.0, dpColor::PALE_PINK_HEAVY.b / 255.0);
const ofFloatColor dpFloatColor::DARK_PINK_LIGHT  = ofFloatColor(dpColor::DARK_PINK_LIGHT.r / 255.0, dpColor::DARK_PINK_LIGHT.g / 255.0, dpColor::DARK_PINK_LIGHT.b / 255.0);
const ofFloatColor dpFloatColor::DARK_PINK_HEAVY  = ofFloatColor(dpColor::DARK_PINK_HEAVY.r / 255.0, dpColor::DARK_PINK_HEAVY.g / 255.0, dpColor::DARK_PINK_HEAVY.b / 255.0);

const ofFloatColor dpDancerFloatColor::MIYASHITA_COLOR = ofFloatColor::fromHsb(0.5, 0.9, 0.7);
const ofFloatColor dpDancerFloatColor::SHIMAJI_COLOR = ofFloatColor::fromHsb(0.95, 0.9, 0.7);
const ofFloatColor dpDancerFloatColor::KOJIRI_COLOR = ofFloatColor::fromHsb(0.3, 0.9, 0.7);
const ofFloatColor dpDancerFloatColor::ANDO_COLOR = ofFloatColor::fromHsb(0.0, 0.9, 0.7);

const ofColor dpDancerColor::MIYASHITA_COLOR = ofColor::fromHsb(0.5 * 255.0, 0.9 * 255.0, 0.7 * 255.0);
const ofColor dpDancerColor::SHIMAJI_COLOR = ofColor::fromHsb(0.95 * 255.0, 0.9 * 255.0, 0.7 * 255.0);
const ofColor dpDancerColor::KOJIRI_COLOR = ofColor::fromHsb(0.3 * 255.0, 0.9 * 255.0, 0.7 * 255.0);
const ofColor dpDancerColor::ANDO_COLOR = ofColor::fromHsb(0.0 * 255.0, 0.9 * 255.0, 0.7 * 255.0);




