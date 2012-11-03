#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ramActor.h"
#include "ramActorManager.h"

class ramBaseApp : public ofBaseApp
{
public:

	ramBaseApp() {};
	virtual ~ramBaseApp() {};

	virtual void drawFloor() {}
	virtual void drawJoint(const ramNode &joint) {}
	virtual void drawBone(const ramNode &jointA, const ramNode &jointB) {}

	// shortcut to ramActorManager

	size_t getNumActor() { return getActorManager().getNumActor(); }
	ramActor& getActor(int index) { return getActorManager().getActor(index); }
	ramActor& getActor(const string& name) { return getActorManager().getActor(name); }

	// physics

	virtual void collision(const ramNode &jointA, const ramNode &jointB) {}

public:

	// events

	void ramEnableAllEvents()
	{
		ofAddListener(ofEvents().update, this, &ramBaseApp::update);
		ofAddListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofAddListener(ofEvents().exit, this, &ramBaseApp::exit);
	}

	void ramDisableAllEvents()
	{
		ofRemoveListener(ofEvents().update, this, &ramBaseApp::update);
		ofRemoveListener(ofEvents().draw, this, &ramBaseApp::draw);
		ofRemoveListener(ofEvents().exit, this, &ramBaseApp::exit);
	}

	// event callback

	void exit(ofEventArgs &args)
	{

	}

	void update(ofEventArgs &args)
	{
		getActorManager().update();
	}

	void draw(ofEventArgs &args)
	{
		getActorManager().draw();

		// actors
		{
			for (int n = 0; n < getActorManager().getNumActor(); n++)
			{
				ramActor &o = getActorManager().getActor(n);

				for (int i = 0; i < o.getNumNode(); i++)
				{
					ramNode &node = o.getNode(i);

					{
						glPushAttrib(GL_ALL_ATTRIB_BITS);
						glPushMatrix();
						ofPushStyle();

						drawJoint(node);

						ofPopStyle();
						glPopMatrix();
						glPopAttrib();
					}

					// たまーにHipのgetParent() が0じゃない場合あり。drawBone(node, *parent_node); で落ちる
					if (node.getID())
					{
						ramNode *parent_node = node.getParent();

						glPushAttrib(GL_ALL_ATTRIB_BITS);
						glPushMatrix();
						ofPushStyle();

						drawBone(node, *parent_node);

						ofPopStyle();
						glPopMatrix();
						glPopAttrib();
					}
				}
			}
		}

		// rigids
		{
			for (int n = 0; n < getActorManager().getNumRigidBody(); n++)
			{
				ramRigidBody &o = getActorManager().getRigidBody(n);

				for (int i = 0; i < o.getNumNode(); i++)
				{
					ramNode &node = o.getNode(i);

					{
						glPushAttrib(GL_ALL_ATTRIB_BITS);
						glPushMatrix();
						ofPushStyle();

						drawJoint(node);

						ofPopStyle();
						glPopMatrix();
						glPopAttrib();
					}
				}
			}
		}
	}

public:

	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }

	void updateWithOscMessage(const ofxOscMessage &m)
	{
		getActorManager().updateWithOscMessage(m);
	}

	// Rec & Play....
	void record()
	{

	}
	void play()
	{

	}
};
