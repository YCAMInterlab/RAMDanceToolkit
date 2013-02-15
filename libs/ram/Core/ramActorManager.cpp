#include "ramActorManager.h"

#include "ramCameraManager.h"
#include "ramNodeFinder.h"

using namespace ofxInteractivePrimitives;

inline static void billboard()
{
	ofMatrix4x4 m;
	glGetFloatv(GL_MODELVIEW_MATRIX, m.getPtr());
	
	ofVec3f s = m.getScale();
	
	m(0, 0) = s.x;
	m(0, 1) = 0;
	m(0, 2) = 0;
	
	m(1, 0) = 0;
	m(1, 1) = s.y;
	m(1, 2) = 0;
	
	m(2, 0) = 0;
	m(2, 1) = 0;
	m(2, 2) = s.z;
	
	glLoadMatrixf(m.getPtr());
}

#pragma mark - ramActorManager::NodeSelector

class ramActorManager::NodeSelector : public ofxInteractivePrimitives::Node
{
public:
	
	enum
	{
		NODE_SELECTOR_PREFIX_ID = 1000
	};
	
	ofEvent<ramNodeIdentifer> selectStateChanged;
	ramNodeIdentifer identifer;
	
	NodeSelector(RootNode &root) { setParent(&root); }
	
	void draw()
	{
		ramActorManager &AM = ramActorManager::instance();
		const vector<GLuint> &NS = getCurrentNameStack();
		
		ofNoFill();
		
		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			ramNodeArray &NA = AM.getNodeArray(n);
			
			for (int i = 0; i < NA.getNumNode(); i++)
			{
				ramNode &node = NA.getNode(i);
				
				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				billboard();
				
				if (NS.size() == 3
					&& NS[1] == n
					&& NS[2] == i)
				{
					ofSetColor(255, 0, 0);
					ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 20) * 5);
				}
				
				glPopMatrix();
			}
		}
	}
	
	void hittest()
	{
		ofFill();
		
		pushID(NODE_SELECTOR_PREFIX_ID);
		
		ramActorManager &AM = ramActorManager::instance();
		for (int n = 0; n < AM.getNumNodeArray(); n++)
		{
			ramNodeArray &NA = AM.getNodeArray(n);
			
			pushID(n);
			
			for (int i = 0; i < NA.getNumNode(); i++)
			{
				ramNode &node = NA.getNode(i);
				
				pushID(i);
				
				glPushMatrix();
				ofTranslate(node.getGlobalPosition());
				billboard();
				
				ofCircle(0, 0, 15);
				
				glPopMatrix();
				
				popID();
			}
			
			popID();
		}
		
		popID();
	}
	
	void mousePressed(int x, int y, int button)
	{
		const vector<GLuint> &NS = getCurrentNameStack();
		ramActorManager &AM = ramActorManager::instance();
		
		if (NS.size() == 3 && NS[0] == NODE_SELECTOR_PREFIX_ID)
		{
			const ramNodeArray &NA = AM.getNodeArray(NS[1]);
			string name = NA.getName();
			int index = NS[2];
			
			identifer.name = name;
			identifer.index = index;
			
			ofNotifyEvent(selectStateChanged, identifer);
			
			ramEnableInteractiveCamera(false);
		}
		else
		{
			identifer.name = "";
			identifer.index = -1;
			
			ofNotifyEvent(selectStateChanged, identifer);
		}
	}
	
	void mouseReleased(int x, int y, int button)
	{
		ramEnableInteractiveCamera(true);
	}
	
	bool isObjectPickedUp()
	{
		return !getCurrentNameStack().empty();
	}
};

#pragma mark - ramActorManager

ramActorManager* ramActorManager::_instance = NULL;

void ramActorManager::setup()
{
	bFreeze = false;
	
	nodeSelector = new NodeSelector(rootNode);
	ofAddListener(nodeSelector->selectStateChanged, this, &ramActorManager::onSelectStateChanged);
}

void ramActorManager::update()
{
	nodearrays.updateIndexCache();
	
	for (int i = 0; i < nodearrays.size(); i++)
	{
		ramNodeArray &array = nodearrays[i];
		if (array.isOutdated() && !isFreezed())
			nodearrays.remove(array.getName());
	}
	
	rootNode.update();
}

void ramActorManager::draw()
{
	rootNode.draw();
	
	if (nodeSelector->identifer.isValid())
	{
		ramNode node;
		if (ramNodeFinder::findNode(nodeSelector->identifer, node))
		{
			node.beginTransform();
			
			billboard();
			
			ofFill();
			ofSetColor(255, 0, 0, 30);
			ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);
			
			ofNoFill();
			ofCircle(0, 0, 10 + sin(ofGetElapsedTimef() * 10) * 5);

			node.endTransform();
		}
	}
}

void ramActorManager::updateWithOscMessage(const ofxOscMessage &m)
{
	if (isFreezed()) return;
	
	const std::string addr = m.getAddress();
	const std::string name = m.getArgAsString(0);
	
	if (addr == RAM_OSC_ADDR_SKELETON)
	{
		if (!nodearrays.hasKey(name))
		{
			ramActor o;
			o.setType(RAM_NODEARRAY_TYPE_ACTOR);
			o.updateWithOscMessage(m);
			o.setName(name);
			nodearrays.add(name, o);
		}
		else
		{
			ramActor &o = (ramActor &)nodearrays[name];
			o.updateWithOscMessage(m);
		}
	}
	else if (addr == RAM_OSC_ADDR_RIGID_BODY)
	{
		if (!nodearrays.hasKey(name))
		{
			ramRigidBody o;
			o.setType(RAM_NODEARRAY_TYPE_RIGIDBODY);
			o.updateWithOscMessage(m);
			nodearrays.add(name, o);
		}
		else
		{
			ramRigidBody &o = (ramRigidBody &)nodearrays[name];
			o.updateWithOscMessage(m);
		}
	}
}

const ramNodeIdentifer& ramActorManager::getLastSelectedNodeIdentifer()
{
	return nodeSelector->identifer;
}

const ramNode* ramActorManager::getLastSelectedNode()
{
	const ramNodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;
	
	return &getNodeArray(node_id.name).getNode(node_id.index);
}

const ramNodeArray* ramActorManager::getLastSelectedNodeArray()
{
	const ramNodeIdentifer &node_id = getLastSelectedNodeIdentifer();
	if (!node_id.isValid()) return NULL;
	
	return &getNodeArray(node_id.name);
}

void ramActorManager::onSelectStateChanged(ramNodeIdentifer &e)
{
	ofNotifyEvent(selectStateChanged, e);
}
