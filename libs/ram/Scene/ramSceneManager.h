#pragma once

#include "ofMain.h"

#include "ramBaseScene.h"
#include "ramGlobal.h"
#include "ramActorsScene.h"

class ramSceneManager : public ramGlobalShortcut
{
public:
	
	static ramSceneManager& instance();

	void setup();
	void addScene(ramBaseScene* scene);
	
	ramActorsScene* getActorsScene();
	void setShowAllActors(bool showAllActors);
	bool getShowAllActors() const;

protected:

	void enableAllEvents();
	void disableAllEvents();

	void actorSetup(ramActor &actor);
	void actorExit(ramActor &actor);

	void rigidSetup(ramRigidBody &rigid);
	void rigidExit(ramRigidBody &rigid);

	vector<ramBaseScene*> scenes;
	
	void update(ofEventArgs& args);
	void draw(ofEventArgs& args);
	void exit(ofEventArgs& args);
	
private:
	
	static ramSceneManager *_instance;
	ramSceneManager();
	ramSceneManager(const ramSceneManager&);
	ramSceneManager& operator=(const ramSceneManager&);
	
	ramActorsScene* actorsScene;
};