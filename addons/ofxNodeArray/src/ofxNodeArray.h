#pragma once

#include "ofMain.h"

namespace ofxNodeArray
{
	template <typename SuperClass> class Node;
	template <typename SuperClass, typename NodeType> class NodeArray;
}

#pragma mark - Node

template <typename SuperClass>
class ofxNodeArray::Node : public ofNode
{
public:
	
	Node() : node_id(-1), ofNode() {}
	Node(int node_id) : node_id(node_id), ofNode() {}
	
	Node(const Node& copy) { *this = copy; }
	Node(const SuperClass& copy) { *this = copy; }
	
	Node& operator=(const Node& copy);
	
	inline int getID() const { return node_id; }
	inline void setID(int id) { node_id = id; }
	
	inline void setParent(SuperClass &parent) { this->parent = &parent; }
	inline SuperClass* getParent() const { return (SuperClass*)parent; }
	inline bool hasParent() const { return parent != NULL; }
	
	// operators
	inline operator ofVec3f() const { return getGlobalPosition(); }
	
	inline bool operator==(const SuperClass &node) const;
	inline bool operator!=(const SuperClass &node) const;
	
	inline SuperClass operator+(const SuperClass &node) const;
	inline SuperClass& operator+=(const SuperClass &node);
	
	inline SuperClass operator-(const SuperClass &node) const;
	inline SuperClass& operator-=(const SuperClass &node);
	
	inline SuperClass& lerp(const SuperClass &base, float t);
	inline SuperClass getLerpd(const SuperClass &base, float t) const;
	
	inline SuperClass& normalize(const SuperClass &base, float length);
	inline SuperClass getNormalized(const SuperClass &base, float length) const;
	
	inline SuperClass& limit(const SuperClass &base, float t);
	inline SuperClass getLimited(const SuperClass &base, float length) const;

protected:
	
	int node_id;
};

#pragma mark - NodeArray

template <typename SuperClass, typename NodeType>
class ofxNodeArray::NodeArray
{
public:
	
	NodeArray() : name("") {}
	NodeArray(const string& name) : name(name) {}
	virtual ~NodeArray() {}
	
	NodeArray(const NodeArray& copy) { *this = copy; }
	NodeArray(const SuperClass& copy) { *this = copy; }
	
	inline NodeArray& operator=(const NodeArray& copy);
	
	inline void setName(const string& name) { this->name = name; }
	inline const string& getName() const { return name; }
	
	inline int getNumNode() const { return nodes.size(); }
	
	inline NodeType& getNode(int node_id) { return nodes[node_id]; }
	inline const NodeType& getNode(int node_id) const { return nodes[node_id]; }
	
	// operators
	inline bool operator==(const SuperClass &arr) const;
	inline bool operator!=(const SuperClass &arr) const;
	
	inline SuperClass operator+(const SuperClass &arr) const;
	inline SuperClass& operator+=(const SuperClass &arr);
	
	inline SuperClass operator-(const SuperClass &arr) const;
	inline SuperClass& operator-=(const SuperClass &arr);
	
	inline SuperClass& lerp(const SuperClass &base, float t);
	inline SuperClass getLerpd(const SuperClass &base, float t) const;
	
	inline SuperClass& normalize(const SuperClass &base, float length);
	inline SuperClass getNormalized(const SuperClass &base, float length) const;
	
	inline SuperClass& limit(const SuperClass &base, float length);
	inline SuperClass getLimited(const SuperClass &base, float length) const;
	
protected:
	
	string name;
	vector<NodeType> nodes;
	
	inline void rebuildHierarchy(const NodeArray& ref);
	inline void clearHierarchy();
	inline void rebuildLocalPosition();
};



// ofxNodeArray::Node impl

template <typename SuperClass>
inline ofxNodeArray::Node<SuperClass>& ofxNodeArray::Node<SuperClass>::operator=(const ofxNodeArray::Node<SuperClass>& copy)
{
	ofNode::operator=(copy);
	
	node_id = copy.node_id;
	parent = copy.parent;
	
	return *this;
}

template <typename SuperClass>
inline bool ofxNodeArray::Node<SuperClass>::operator==(const SuperClass &node) const
{
	const float *m = getLocalTransformMatrix().getPtr();
	const float *mm = node.getLocalTransformMatrix().getPtr();
	
	float r = 0;
	r += fabs(m[0] - mm[0]);
	r += fabs(m[1] - mm[1]);
	r += fabs(m[2] - mm[2]);
	
	r += fabs(m[4] - mm[4]);
	r += fabs(m[5] - mm[5]);
	r += fabs(m[6] - mm[6]);
	
	r += fabs(m[8] - mm[8]);
	r += fabs(m[9] - mm[9]);
	r += fabs(m[10] - mm[10]);
	
	if (r > 0.01)
	{
		return false;
	}
	
	float d = ofVec3f(m[12], m[13], m[14]).distance(ofVec3f(mm[12], mm[13], mm[14]));
	if (d > 0.2)
	{
		return false;
	}
	
	return true;
}

template <typename SuperClass>
inline bool ofxNodeArray::Node<SuperClass>::operator!=(const SuperClass &node) const
{
	return !(*this == node);
}

template <typename SuperClass>
inline SuperClass ofxNodeArray::Node<SuperClass>::operator+(const SuperClass &node) const
{
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	result.setPosition(result.getPosition() + node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat());
	
	return result;
}

template <typename SuperClass>
inline SuperClass& ofxNodeArray::Node<SuperClass>::operator+=(const SuperClass &node)
{
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	result.setPosition(result.getPosition() + node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat());
	
	return result;
}

template <typename SuperClass>
inline SuperClass ofxNodeArray::Node<SuperClass>::operator-(const SuperClass &node) const
{
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	result.setPosition(result.getPosition() - node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat().inverse());
	
	return result;
}

template <typename SuperClass>
inline SuperClass& ofxNodeArray::Node<SuperClass>::operator-=(const SuperClass &node)
{
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	result.setPosition(result.getPosition() - node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat().inverse());
	
	return result;
}

template <typename SuperClass>
inline SuperClass& ofxNodeArray::Node<SuperClass>::lerp(const SuperClass &base, float t)
{
	const ofMatrix4x4& a = this->getGlobalTransformMatrix();
	const ofMatrix4x4& b = base.getGlobalTransformMatrix();
	
	ofQuaternion trot;
	trot.slerp(t, a.getRotate(), b.getRotate());
	setGlobalOrientation(trot);
	
	ofVec3f apos = a.getTranslation(), bpos = b.getTranslation();
	setGlobalPosition(apos.interpolate(bpos, t));
	
	return *this;
}

template <typename SuperClass>
inline SuperClass ofxNodeArray::Node<SuperClass>::getLerpd(const SuperClass &base, float t) const
{
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;

	result.lerp(base, t);
	return result;
}

template <typename SuperClass>
inline SuperClass& ofxNodeArray::Node<SuperClass>::normalize(const SuperClass &base, float length)
{
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.normalize();
	d *= length;
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename SuperClass>
inline SuperClass ofxNodeArray::Node<SuperClass>::getNormalized(const SuperClass &base, float length) const
{
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.normalize();
	d *= length;
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename SuperClass>
inline SuperClass& ofxNodeArray::Node<SuperClass>::limit(const SuperClass &base, float length)
{
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.limit(length);
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename SuperClass>
inline SuperClass ofxNodeArray::Node<SuperClass>::getLimited(const SuperClass &base, float length) const
{
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.limit(length);
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

// ofxNodeArray::NodeArray impl

template <typename SuperClass, typename NodeType>
inline void ofxNodeArray::NodeArray<SuperClass, NodeType>::rebuildHierarchy(const ofxNodeArray::NodeArray<SuperClass, NodeType>& ref)
{
	// rebuild hierarchy
	for (int i = 0; i < ref.nodes.size(); i++)
	{
		const NodeType &src = ref.nodes[i];
		NodeType &dst = nodes[i];
		
		NodeType *p = src.getParent();
		if (!p) continue;
		
		int idx = p->getID();
		if (idx < 0) continue;
		
		dst.setParent(nodes[idx]);
	}
}

template <typename SuperClass, typename NodeType>
inline void ofxNodeArray::NodeArray<SuperClass, NodeType>::clearHierarchy()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		NodeType &node = nodes[i];
		ofMatrix4x4 m = node.getGlobalTransformMatrix();
		node.clearParent();
		node.setTransformMatrix(m);
	}
}

template <typename SuperClass, typename NodeType>
inline void ofxNodeArray::NodeArray<SuperClass, NodeType>::rebuildLocalPosition()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		NodeType &node = nodes[i];
		ofMatrix4x4 m = node.getTransformMatrix();
		node.setGlobalOrientation(m.getRotate());
		node.setGlobalPosition(m.getTranslation());
	}
}

template <typename SuperClass, typename NodeType>
inline ofxNodeArray::NodeArray<SuperClass, NodeType>& ofxNodeArray::NodeArray<SuperClass, NodeType>::operator=(const ofxNodeArray::NodeArray<SuperClass, NodeType>& copy)
{
	this->name = copy.name;
	
	this->nodes = copy.nodes;
	rebuildHierarchy(copy);
	
	return *this;
}

template <typename SuperClass, typename NodeType>
inline bool ofxNodeArray::NodeArray<SuperClass, NodeType>::operator==(const SuperClass &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	for (int i = 0; i < getNumNode(); i++)
		if (getNode(i) != arr.getNode(i)) return false;
	
	return true;
}

template <typename SuperClass, typename NodeType>
inline bool ofxNodeArray::NodeArray<SuperClass, NodeType>::operator!=(const SuperClass &arr) const
{
	return !(*this == arr);
}

template <typename SuperClass, typename NodeType>
inline SuperClass ofxNodeArray::NodeArray<SuperClass, NodeType>::operator+(const SuperClass &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) += arr.getNode(i);
	
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass& ofxNodeArray::NodeArray<SuperClass, NodeType>::operator+=(const SuperClass &arr)
{
	assert(getNumNode() == arr.getNumNode());
	
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) += arr.getNode(i);
	
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass ofxNodeArray::NodeArray<SuperClass, NodeType>::operator-(const SuperClass &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) -= arr.getNode(i);
	
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass& ofxNodeArray::NodeArray<SuperClass, NodeType>::operator-=(const SuperClass &arr)
{
	assert(getNumNode() == arr.getNumNode());
	
	SuperClass &result = static_cast<SuperClass&>(*this);
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) -= arr.getNode(i);
	
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass& ofxNodeArray::NodeArray<SuperClass, NodeType>::lerp(const SuperClass &base, float t)
{
	assert(getNumNode() == base.getNumNode());
	
	SuperClass &result = static_cast<SuperClass&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i).lerp(base.getNode(i), t);
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass ofxNodeArray::NodeArray<SuperClass, NodeType>::getLerpd(const SuperClass &base, float t) const
{
	assert(getNumNode() == base.getNumNode());
	
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;
	
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).lerp(base.getNode(i), t);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass& ofxNodeArray::NodeArray<SuperClass, NodeType>::normalize(const SuperClass &base, float length)
{
	assert(getNumNode() == base.getNumNode());
	
	SuperClass &result = static_cast<SuperClass&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).normalize(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass ofxNodeArray::NodeArray<SuperClass, NodeType>::getNormalized(const SuperClass &base, float length) const
{
	assert(getNumNode() == base.getNumNode());
	
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;

	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).normalize(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass& ofxNodeArray::NodeArray<SuperClass, NodeType>::limit(const SuperClass &base, float length)
{
	assert(getNumNode() == base.getNumNode());
	
	SuperClass &result = static_cast<SuperClass&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).limit(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename SuperClass, typename NodeType>
inline SuperClass ofxNodeArray::NodeArray<SuperClass, NodeType>::getLimited(const SuperClass &base, float length) const
{
	assert(getNumNode() == base.getNumNode());
	
	const SuperClass &self = static_cast<const SuperClass&>(*this);
	SuperClass result = self;

	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).limit(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}
