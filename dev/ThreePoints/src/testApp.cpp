#include "testApp.h"


static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

// NO texture coords or normals
// use ofGetIcoSphere(radius, 0) // 0 iterations will return Icosahedron //
//----------------------------------------------------------
ofMesh ofGetIcosahedronMesh(float radius) {
    ofMesh mesh;
    
    const float sqrt5 = sqrt(5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    
    /// Step 1 : Generate icosahedron
    float invnorm = 1/sqrt(phi*phi+1);
    
    mesh.addVertex(invnorm*ofVec3f(-1,  phi, 0));//0
    mesh.addVertex(invnorm*ofVec3f( 1,  phi, 0));//1
    mesh.addVertex(invnorm*ofVec3f(0,   1,  -phi));//2
    mesh.addVertex(invnorm*ofVec3f(0,   1,   phi));//3
    mesh.addVertex(invnorm*ofVec3f(-phi,0,  -1));//4
    mesh.addVertex(invnorm*ofVec3f(-phi,0,   1));//5
    mesh.addVertex(invnorm*ofVec3f( phi,0,  -1));//6
    mesh.addVertex(invnorm*ofVec3f( phi,0,   1));//7
    mesh.addVertex(invnorm*ofVec3f(0,   -1, -phi));//8
    mesh.addVertex(invnorm*ofVec3f(0,   -1,  phi));//9
    mesh.addVertex(invnorm*ofVec3f(-1,  -phi,0));//10
    mesh.addVertex(invnorm*ofVec3f( 1,  -phi,0));//11
    
    int firstFaces[] = {
        0,1,2,
        0,3,1,
        0,4,5,
        1,7,6,
        1,6,2,
        1,3,7,
        0,2,4,
        0,5,3,
        2,6,8,
        2,8,4,
        3,5,9,
        3,9,7,
        11,6,7,
        10,5,4,
        10,4,8,
        10,9,5,
        11,8,6,
        11,7,9,
        10,8,11,
        10,11,9
    };
    
    for(int i = 0; i < mesh.getNumVertices(); i++) {
        mesh.setVertex(i, mesh.getVertex(i) * radius);
    }
    
    for(int i = 0; i < 60; i+=3) {
        mesh.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2]);
    }
    
    return mesh;
}

// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
//----------------------------------------------------------
ofMesh ofGetIcoSphereMesh(float radius, int iterations) {
    
    ofMesh icosahedron = ofGetIcosahedronMesh( 1.f );
    vector<ofVec3f> vertices = icosahedron.getVertices();
    vector<ofIndexType> faces = icosahedron.getIndices();
    
    int size = faces.size();
    
    /// Step 2 : tessellate
    for (unsigned short iteration = 0; iteration < iterations; iteration++)
    {
        size*=4;
        vector<ofIndexType> newFaces;
        newFaces.clear();
        //newFaces.resize(size);
        for (int i=0; i<size/12; i++)
        {
            int i1 = faces[i*3];
            int i2 = faces[i*3+1];
            int i3 = faces[i*3+2];
            int i12 = vertices.size();
            int i23 = i12+1;
            int i13 = i12+2;
            ofVec3f v1 = vertices[i1];
            ofVec3f v2 = vertices[i2];
            ofVec3f v3 = vertices[i3];
            //make 1 vertice at the center of each edge and project it onto the sphere
            vertices.push_back((v1+v2).normalized());
            vertices.push_back((v2+v3).normalized());
            vertices.push_back((v1+v3).normalized());
            //now recreate indices
            newFaces.push_back(i1);
            newFaces.push_back(i12);
            newFaces.push_back(i13);
            newFaces.push_back(i2);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i3);
            newFaces.push_back(i13);
            newFaces.push_back(i23);
            newFaces.push_back(i12);
            newFaces.push_back(i23);
            newFaces.push_back(i13);
        }
        faces.swap(newFaces);
    }
    
    /// Step 3 : generate texcoords
    vector<ofVec2f> texCoords;
    for (unsigned short i=0;i<vertices.size();i++)
    {
        const ofVec3f& vec = vertices[i];
        float u, v;
        float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
        float alpha;
        alpha = atan2f(vec.z,vec.x);
        u = alpha/TWO_PI+.5f;
        v = atan2f(vec.y, r0)/PI + .5f;
        // reverse the u coord, so the default is texture mapped left to
        // right on the outside of a sphere //
        texCoords.push_back(ofVec2f(1.0-u,v));
    }
    
    /// Step 4 : fix texcoords
    // find vertices to split
    std::vector<int> indexToSplit;
    
    for (unsigned int i=0;i<faces.size()/3;i++)
    {
        ofVec2f& t0 = texCoords[faces[i*3+0]];
        ofVec2f& t1 = texCoords[faces[i*3+1]];
        ofVec2f& t2 = texCoords[faces[i*3+2]];
        
        if (abs(t2.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
        if (abs(t1.x-t0.x)>0.5)
        {
            if (t0.x<0.5)
                indexToSplit.push_back(faces[i*3]);
            else
                indexToSplit.push_back(faces[i*3+1]);
        }
        if (abs(t2.x-t1.x)>0.5)
        {
            if (t1.x<0.5)
                indexToSplit.push_back(faces[i*3+1]);
            else
                indexToSplit.push_back(faces[i*3+2]);
        }
    }
    
    //split vertices
    for (unsigned short i=0;i<indexToSplit.size();i++)
    {
        int index = indexToSplit[i];
        //duplicate vertex
        ofVec3f v = vertices[index];
        ofVec2f t = texCoords[index] + ofVec2f(1.f, 0.f);
        vertices.push_back(v);
        texCoords.push_back(t);
        int newIndex = vertices.size()-1;
        //reassign indices
        for (unsigned short j=0;j<faces.size();j++)
        {
            if (faces[j]==index)
            {
                int index1 = faces[(j+1)%3+(j/3)*3];
                int index2 = faces[(j+2)%3+(j/3)*3];
                if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
                {
                    faces[j] = newIndex;
                }
            }
        }
    }
    
    
    ofMesh sphere;
    
    sphere.addIndices( faces );
    //sphere.addNormals( vertices );
    //sphere.addTexCoords( texCoords );
    
    for(int i = 0; i < vertices.size(); i++ ) {
        vertices[i] *= radius;
    }
    sphere.addVertices( vertices );
    
    return  sphere;
}

ofMesh icosphere;
void ofIcoSphere(ofVec3f center, float radius) {
    ofPushMatrix();
    ofTranslate(center);
    ofScale(radius, radius, radius);
    icosphere.drawVertices();
    icosphere.drawWireframe();
    ofPopMatrix();
}

//--------------------------------------------------------------
void rotateToNormal(ofVec3f normal) {
	normal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 1);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

// from the bourkster.
// https://gist.github.com/2724338
//--------------------------------------------------------------
bool lineLineIntersectSegment(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4, ofVec3f &pa, ofVec3f &pb) {
	float mua = 0;
	float mub = 0;
	
	ofVec3f p13,p43,p21;
	
	float d1343,d4321,d1321,d4343,d2121;
	float numer,denom;
	p13 = p1 - p3;
	p43 = p4 - p3;
	
	
	if (ABS(p43.x) < FLT_EPSILON && ABS(p43.y) < FLT_EPSILON && ABS(p43.z) < FLT_EPSILON)
		return false;
	
	p21 = p2 - p1;
	
	if (ABS(p21.x) < FLT_EPSILON && ABS(p21.y) < FLT_EPSILON && ABS(p21.z) < FLT_EPSILON)
		return false;
	
	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
	
	denom = d2121 * d4343 - d4321 * d4321;
	if (ABS(denom) < FLT_EPSILON)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;
	
	mua = numer / denom;
	mub = (d1343 + d4321 * mua) / d4343;
	
	pa = p1 + p21 * mua;
	pb = p3 + p43 * mub;
	
	return true;
}

// https://gist.github.com/2724338
//--------------------------------------------------------------
ofVec3f lineLineIntersection(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4) {
	ofVec3f pa, pb;
	lineLineIntersectSegment(p1,p2,p3,p4,pa,pb);
	return (pa+pb)/2.f;
}

//--------------------------------------------------------------
void findCircle(const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, ofVec3f& center, ofVec3f& normal, float& radius) {
	ofVec3f ab = a - b, cb = c - b;
	normal = ab.getCrossed(cb);
	ofVec3f abm = ab / 2, cbm = cb / 2;
	ofVec3f abo = abm.getRotated(90, normal), cbo = cbm.getRotated(90, normal);
	abm += b, cbm += b;
	center = lineLineIntersection(abm, abm + abo, cbm, cbm + cbo);
	radius = center.distance(b);
	normal.normalize();
}

//--------------------------------------------------------------
void findSphere(const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, const ofVec3f& d, ofVec3f& center, float& radius) {
    ofVec3f c1, c2, n1, n2;
    float r1, r2;
    findCircle(a, b, c, c1, n1, r1);
    findCircle(b, c, d, c2, n2, r2);
    center = lineLineIntersection(c1, c1 + n1, c2, c2 + n2);
    radius = center.distance(a);
}

//--------------------------------------------------------------
int getChoice(ofxUIToggleMatrix* matrix) {
    vector<ofxUIToggle*> toggles = matrix->getToggles();
    for(int i = 0; i < toggles.size(); i++) {
        if(toggles[i]->getValue()) {
            return i;
        }
    }
    return 0;
}

//--------------------------------------------------------------
void testApp::setupUI() {
	float dim = 20;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 320 - xInit;
	gui = new ofxUICanvas(0, 0, length + xInit * 2, 2560);
	gui->setFont("/System/Library/Fonts/HelveticaLight.ttf");
	ofColor cb(64, 192),
	co(192, 192),
	coh(128, 192),
	cf(240, 255),
	cfh(128, 255),
	cp(96, 192),
	cpo(255, 192);
	gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
	
	gui->addLabel("Joint Selection", OFX_UI_FONT_LARGE);
	gui->addLabelToggle("Show spheres", &showSpheres, length, dim);
	gui->addLabelToggle("Show rects", &showRects, length, dim);
	gui->addLabelToggle("Inverted spheres", &invertSpheres, length, dim);
	gui->addLabelToggle("Show circle bisector", &showCircleBisector, length, dim);
	gui->addLabelToggle("Show center circles", &showCenterCircles, length, dim);
	gui->addSlider("Point size", 1, 10, &pointSize, length, dim);
	gui->addSlider("Cross length", 1, 1000, &crossLength, length, dim);
	gui->addSlider("Rect radius", 1, 1000, &rectRadius, length, dim);
	gui->addSlider("Max invert radius", 1, 10000, &maxInvertRadius, length, dim);
	gui->addSlider("Circle resolution", 3, 30, &circleResolution, length, dim);
	int matrixSize = 10;
    
	gui->addLabelToggle("Show three points", &showThreePoints, length, dim);
	m3p = (ofxUIToggleMatrix*) gui->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "3p"));
    
	gui->addLabelToggle("Show four point twist", &showFourPointTwist, length, dim);
	gui->addSlider("Twist resolution", 2, 100, &twistResolution, length, dim);
	gui->addSlider("Twist extension", 1, 40, &twistExtension, length, dim);
	gui->addLabelToggle("Show four point sphere", &showFourPointSphere, length, dim);
	m4p1 = (ofxUIToggleMatrix*) gui->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.1"));
	m4p2 = (ofxUIToggleMatrix*) gui->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.2"));
	m4p3 = (ofxUIToggleMatrix*) gui->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.3"));
	m4p4 = (ofxUIToggleMatrix*) gui->addWidgetRight(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.4"));
    m4p1->setAllowMultiple(false);
    m4p2->setAllowMultiple(false);
    m4p3->setAllowMultiple(false);
    m4p4->setAllowMultiple(false);
    m4p1->getToggles()[ramActor::JOINT_LEFT_HAND]->setValue(true);
    m4p2->getToggles()[ramActor::JOINT_RIGHT_HAND]->setValue(true);
    m4p3->getToggles()[ramActor::JOINT_LEFT_TOE]->setValue(true);
    m4p4->getToggles()[ramActor::JOINT_RIGHT_TOE]->setValue(true);
}

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(120);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetLineWidth(2);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);
    
    icosphere = ofGetIcoSphereMesh(1, 3);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	showRects = false;
	showSpheres = false;
	invertSpheres = false;
	showCircleBisector = false;
	showCenterCircles = false;
    showFourPointSphere = false;
    showFourPointTwist = false;
    showThreePoints = false;
	crossLength = 0;
	rectRadius = 0;
	pointSize = 3;
	maxInvertRadius = 2000;
	circleResolution = 30;
    twistResolution = 20;
    twistExtension = 1;
	
	setupUI();
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	glPointSize(pointSize);
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramDrawBasicFloor(ramFloor::FLOOR_PLANE, 600., 50., ofColor(255, 64), ofColor(255, 96));
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(0);
	
    ofVec3f j1 = actor.getNode(getChoice(m4p1));
    ofVec3f j2 = actor.getNode(getChoice(m4p2));
    ofVec3f j3 = actor.getNode(getChoice(m4p3));
    ofVec3f j4 = actor.getNode(getChoice(m4p4));
    if(j1 != j2 && j1 != j3 && j1 != j4 && j2 != j3 && j2 != j4 && j3 != j4) {
        if(showFourPointSphere) {
            ofVec3f sphereCenter;
            float sphereRadius;
            findSphere(j1, j2, j3, j4, sphereCenter, sphereRadius);
            ofSetColor(255, 32);
            ofIcoSphere(sphereCenter, sphereRadius);
        }
        if(showFourPointTwist) {
            ofVec3f ap = (j1 + j2) / 2, bp = (j3 + j4) / 2;
            ofVec3f an = j2 - j1, bn = j4 - j3;
            float al = an.length(), bl = bn.length();
            ofQuaternion aq, bq;
            aq.makeRotate(ofVec3f(1, 0, 0), an.getNormalized());
            bq.makeRotate(ofVec3f(1, 0, 0), bn.getNormalized());
            ofSetColor(255, 192);
            for(int i = 0; i < twistResolution; i++) {
                float t = ofMap(i, 0, twistResolution - 1, 0, 1);
                ofPushMatrix();
                ofTranslate(ap.getInterpolated(bp, t));
                ofQuaternion q;
                q.slerp(t, aq, bq);
                ofMultMatrix(ofMatrix4x4(q));
                float radius = twistExtension * ofLerp(al, bl, t) / 2;
                ofLine(-radius, 0, radius, 0);
                ofPopMatrix();
            }
        }
    }
    
	for (int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		
		ofPushMatrix();
		node.beginTransform();
		ofSetColor(yellowPrint);
		ofBox((i==ramActor::JOINT_HEAD) ? 6 : 3);
		if(showRects) {
			ofPushStyle();
			ofFill();
			ofSetColor(255, 128);
			ofRect(0, 0, 100, 30);
			ofPopStyle();
		}
		
		ofSetColor(255);
		node.endTransform();
		ofPopMatrix();
		
		if (node.hasParent())
		{
			ramNode* parent = node.getParent();
			ofLine(node, *parent);
            
			if(parent->hasParent() && m3p->getToggles()[i]->getValue())
			{
				ramNode* grandparent = parent->getParent();
				ofVec3f a = node, b = *parent, c = *grandparent;
				ofVec3f normal = (a - b).cross(c - b);
				normal.normalize();
				ofSetColor(magentaPrint);
				ofLine(b - normal * crossLength, b + normal * crossLength);
				
				ofVec3f center;
				float radius;
				findCircle(a, b, c, center, normal, radius);
				ofPushMatrix();
				ofTranslate(center);
				rotateToNormal(normal);
				if(showSpheres) {
					ofSetColor(255, 32);
					ofIcoSphere(ofVec3f(), radius);
				}
				ofSetColor(yellowPrint);
				if(showThreePoints) {
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
				if(showCircleBisector) {
					ofLine(b, b + (center - b) * 2);
				}
				
				ofPushMatrix();
				ofTranslate(b);
				rotateToNormal(normal);
				ofSetColor(cyanPrint);
				ofRect(-rectRadius, -rectRadius, 2*rectRadius, 2*rectRadius);
				if(invertSpheres) {
					ofSetColor(255, 32);
					ofIcoSphere(ofVec3f(), maxInvertRadius / radius);
				}
				if(showCenterCircles) {
					ofSetColor(yellowPrint);
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
			}
		}
	}
	
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}

