#pragma once

#include "ofMain.h"

namespace ofxNodeArray
{
	template <typename BaseType> class Node;
	template <typename BaseType, typename NodeType> class NodeArray;
}

#pragma mark - Node

template <typename BaseType>
class ofxNodeArray::Node : public ofNode
{
public:
	
	Node() : node_id(-1), ofNode() {}
	Node(int node_id) : node_id(node_id), ofNode() {}
	
	Node(const Node& copy) { *this = copy; }
	Node(const BaseType& copy) { *this = copy; }
	
	Node& operator=(const Node& copy);
	
	inline int getID() { return node_id; }
	
	inline void setParent(BaseType &parent) { this->parent = &parent; }
	inline BaseType* getParent() const { return (BaseType*)parent; }
	inline bool hasParent() const { return parent != NULL; }
	
	// operators
	inline operator ofVec3f() const { return getGlobalPosition(); }
	
	inline bool operator==(const BaseType &node) const;
	inline bool operator!=(const BaseType &node) const;
	
	inline BaseType operator+(const BaseType &node) const;
	inline BaseType& operator+=(const BaseType &node);
	
	inline BaseType operator-(const BaseType &node) const;
	inline BaseType& operator-=(const BaseType &node);
	
	inline BaseType& lerp(const BaseType &base, float t);
	inline BaseType getLerpd(const BaseType &base, float t) const;
	
	inline BaseType& normalize(const BaseType &base, float length);
	inline BaseType getNormalized(const BaseType &base, float length) const;
	
	inline BaseType& limit(const BaseType &base, float t);
	inline BaseType getLimited(const BaseType &base, float length) const;

	// utils
	inline void beginTransform() const { transformGL(); }
	inline void endTransform() const { restoreTransformGL(); }

protected:
	
	int node_id;
};

#pragma mark - NodeArray

template <typename BaseType, typename NodeType>
class ofxNodeArray::NodeArray
{
public:
	
	NodeArray() : name("") {}
	NodeArray(const string& name) : name(name) {}
	virtual ~NodeArray() {}
	
	NodeArray(const NodeArray& copy) { *this = copy; }
	NodeArray(const BaseType& copy) { *this = copy; }
	
	inline NodeArray& operator=(const NodeArray& copy);
	
	inline void setName(const string& name) { this->name = name; }
	inline const string& getName() const { return name; }
	
	inline int getNumNode() const { return nodes.size(); }
	
	inline NodeType& getNode(int node_id) { return nodes[node_id]; }
	inline const NodeType& getNode(int node_id) const { return nodes[node_id]; }
	
	// operators
	inline bool operator==(const BaseType &arr) const;
	inline bool operator!=(const BaseType &arr) const;
	
	inline BaseType operator+(const BaseType &arr) const;
	inline BaseType& operator+=(const BaseType &arr);
	
	inline BaseType operator-(const BaseType &arr) const;
	inline BaseType& operator-=(const BaseType &arr);
	
	inline BaseType& lerp(const BaseType &base, float t);
	inline BaseType getLerpd(const BaseType &base, float t) const;
	
	inline BaseType& normalize(const BaseType &base, float length);
	inline BaseType getNormalized(const BaseType &base, float length) const;
	
	inline BaseType& limit(const BaseType &base, float length);
	inline BaseType getLimited(const BaseType &base, float length) const;
	
protected:
	
	string name;
	vector<NodeType> nodes;
	
	inline void rebuildHierarchy(const NodeArray& ref);
	inline void clearHierarchy();
	inline void rebuildLocalPosition();
};



// ofxNodeArray::Node impl

template <typename BaseType>
inline ofxNodeArray::Node<BaseType>& ofxNodeArray::Node<BaseType>::operator=(const ofxNodeArray::Node<BaseType>& copy)
{
	ofNode::operator=(copy);
	
	node_id = copy.node_id;
	parent = copy.parent;
	
	return *this;
}

template <typename BaseType>
inline bool ofxNodeArray::Node<BaseType>::operator==(const BaseType &node) const
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

template <typename BaseType>
inline bool ofxNodeArray::Node<BaseType>::operator!=(const BaseType &node) const
{
	return !(*this == node);
}

template <typename BaseType>
inline BaseType ofxNodeArray::Node<BaseType>::operator+(const BaseType &node) const
{
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	result.setPosition(result.getPosition() + node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat());
	
	return result;
}

template <typename BaseType>
inline BaseType& ofxNodeArray::Node<BaseType>::operator+=(const BaseType &node)
{
	BaseType &result = static_cast<BaseType&>(*this);
	
	result.setPosition(result.getPosition() + node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat());
	
	return result;
}

template <typename BaseType>
inline BaseType ofxNodeArray::Node<BaseType>::operator-(const BaseType &node) const
{
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	result.setPosition(result.getPosition() - node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat().inverse());
	
	return result;
}

template <typename BaseType>
inline BaseType& ofxNodeArray::Node<BaseType>::operator-=(const BaseType &node)
{
	BaseType &result = static_cast<BaseType&>(*this);
	
	result.setPosition(result.getPosition() - node.getPosition());
	result.setOrientation(result.getOrientationQuat() * node.getOrientationQuat().inverse());
	
	return result;
}

template <typename BaseType>
inline BaseType& ofxNodeArray::Node<BaseType>::lerp(const BaseType &base, float t)
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

template <typename BaseType>
inline BaseType ofxNodeArray::Node<BaseType>::getLerpd(const BaseType &base, float t) const
{
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;

	result.lerp(base, t);
	return result;
}

template <typename BaseType>
inline BaseType& ofxNodeArray::Node<BaseType>::normalize(const BaseType &base, float length)
{
	BaseType &result = static_cast<BaseType&>(*this);
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.normalize();
	d *= length;
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename BaseType>
inline BaseType ofxNodeArray::Node<BaseType>::getNormalized(const BaseType &base, float length) const
{
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.normalize();
	d *= length;
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename BaseType>
inline BaseType& ofxNodeArray::Node<BaseType>::limit(const BaseType &base, float length)
{
	BaseType &result = static_cast<BaseType&>(*this);
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.limit(length);
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

template <typename BaseType>
inline BaseType ofxNodeArray::Node<BaseType>::getLimited(const BaseType &base, float length) const
{
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	const ofVec3f &p0 = result.getGlobalPosition();
	const ofVec3f &p1 = base.getGlobalPosition();
	
	ofVec3f d = (p0 - p1);
	d.limit(length);
	
	result.setGlobalPosition(p1 + d);
	
	return result;
}

// ofxNodeArray::NodeArray impl

template <typename BaseType, typename NodeType>
inline void ofxNodeArray::NodeArray<BaseType, NodeType>::rebuildHierarchy(const ofxNodeArray::NodeArray<BaseType, NodeType>& ref)
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

template <typename BaseType, typename NodeType>
inline void ofxNodeArray::NodeArray<BaseType, NodeType>::clearHierarchy()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		NodeType &node = nodes[i];
		ofMatrix4x4 m = node.getGlobalTransformMatrix();
		node.clearParent();
		node.setTransformMatrix(m);
	}
}

template <typename BaseType, typename NodeType>
inline void ofxNodeArray::NodeArray<BaseType, NodeType>::rebuildLocalPosition()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		NodeType &node = nodes[i];
		ofMatrix4x4 m = node.getTransformMatrix();
		node.setGlobalOrientation(m.getRotate());
		node.setGlobalPosition(m.getTranslation());
	}
}

template <typename BaseType, typename NodeType>
inline ofxNodeArray::NodeArray<BaseType, NodeType>& ofxNodeArray::NodeArray<BaseType, NodeType>::operator=(const ofxNodeArray::NodeArray<BaseType, NodeType>& copy)
{
	this->name = copy.name;
	
	this->nodes = copy.nodes;
	rebuildHierarchy(copy);
	
	return *this;
}

template <typename BaseType, typename NodeType>
inline bool ofxNodeArray::NodeArray<BaseType, NodeType>::operator==(const BaseType &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	for (int i = 0; i < getNumNode(); i++)
		if (getNode(i) != arr.getNode(i)) return false;
	
	return true;
}

template <typename BaseType, typename NodeType>
inline bool ofxNodeArray::NodeArray<BaseType, NodeType>::operator!=(const BaseType &arr) const
{
	return !(*this == arr);
}

template <typename BaseType, typename NodeType>
inline BaseType ofxNodeArray::NodeArray<BaseType, NodeType>::operator+(const BaseType &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) += arr.getNode(i);
	
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType& ofxNodeArray::NodeArray<BaseType, NodeType>::operator+=(const BaseType &arr)
{
	assert(getNumNode() == arr.getNumNode());
	
	BaseType &result = static_cast<BaseType&>(*this);
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) += arr.getNode(i);
	
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType ofxNodeArray::NodeArray<BaseType, NodeType>::operator-(const BaseType &arr) const
{
	assert(getNumNode() == arr.getNumNode());
	
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) -= arr.getNode(i);
	
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType& ofxNodeArray::NodeArray<BaseType, NodeType>::operator-=(const BaseType &arr)
{
	assert(getNumNode() == arr.getNumNode());
	
	BaseType &result = static_cast<BaseType&>(*this);
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i) -= arr.getNode(i);
	
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType& ofxNodeArray::NodeArray<BaseType, NodeType>::lerp(const BaseType &base, float t)
{
	assert(getNumNode() == base.getNumNode());
	
	BaseType &result = static_cast<BaseType&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
		result.getNode(i).lerp(base.getNode(i), t);
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType ofxNodeArray::NodeArray<BaseType, NodeType>::getLerpd(const BaseType &base, float t) const
{
	assert(getNumNode() == base.getNumNode());
	
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;
	
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).lerp(base.getNode(i), t);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType& ofxNodeArray::NodeArray<BaseType, NodeType>::normalize(const BaseType &base, float length)
{
	assert(getNumNode() == base.getNumNode());
	
	BaseType &result = static_cast<BaseType&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).normalize(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType ofxNodeArray::NodeArray<BaseType, NodeType>::getNormalized(const BaseType &base, float length) const
{
	assert(getNumNode() == base.getNumNode());
	
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;

	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).normalize(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType& ofxNodeArray::NodeArray<BaseType, NodeType>::limit(const BaseType &base, float length)
{
	assert(getNumNode() == base.getNumNode());
	
	BaseType &result = static_cast<BaseType&>(*this);
	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).limit(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}

template <typename BaseType, typename NodeType>
inline BaseType ofxNodeArray::NodeArray<BaseType, NodeType>::getLimited(const BaseType &base, float length) const
{
	assert(getNumNode() == base.getNumNode());
	
	const BaseType &self = static_cast<const BaseType&>(*this);
	BaseType result = self;

	result.clearHierarchy();
	
	for (int i = 0; i < result.getNumNode(); i++)
	{
		result.getNode(i).limit(base.getNode(i), length);
	}
	
	result.rebuildHierarchy(base);
	result.rebuildLocalPosition();
	return result;
}
