#pragma once
#include "ramMain.h"

// Inherit `rdtk::BaseScene` to make your own scene
class EmptyScene : public rdtk::BaseScene
{
public:
	
  // You must define `getName` to show your scene name in GUI
  std::string getName() const { return "EmptyScene"; }
	
  // Drive your scene with OF event hooks
  void setup();
  void update();
  void draw();
	
  // For drawing GUI, define `drawImGui()`
  void drawImGui();
  
private:
  /*
   This EmptyScene includes a simple example for picking a node with mouse click.
  
   - Run this app 
   - Click a node of an Actor on the screen
   - Click `Push Port` on GUI, then a wire cube will appear on the screen. See EmptyScene::draw()
  */
  rdtk::MotionExtractor mEx;
};
