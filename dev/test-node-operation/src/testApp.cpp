#include "testApp.h"

#include "ofxGoogleTest.h"

void makeTestActor(ramActor &actor)
{
	ofxOscMessage m;
	m.setAddress("/ram/skeleton");
	
	m.addStringArg("Richi_2012-09-01_16-55-24");
	m.addFloatArg(23);
	m.addStringArg("Hips");
	m.addFloatArg(245.531998);
	m.addFloatArg(81.952698);
	m.addFloatArg(110.873001);
	m.addFloatArg(77.887604);
	m.addFloatArg(0.511670);
	m.addFloatArg(-0.463257);
	m.addFloatArg(-0.723593);
	m.addStringArg("Adbomen");
	m.addFloatArg(247.250000);
	m.addFloatArg(92.078102);
	m.addFloatArg(116.371002);
	m.addFloatArg(133.207001);
	m.addFloatArg(-0.245773);
	m.addFloatArg(0.966723);
	m.addFloatArg(-0.071011);
	m.addStringArg("Chest");
	m.addFloatArg(245.266998);
	m.addFloatArg(95.547997);
	m.addFloatArg(114.771004);
	m.addFloatArg(137.274002);
	m.addFloatArg(-0.233063);
	m.addFloatArg(0.963149);
	m.addFloatArg(-0.134261);
	m.addStringArg("Neck");
	m.addFloatArg(236.759995);
	m.addFloatArg(127.790001);
	m.addFloatArg(103.317001);
	m.addFloatArg(151.917007);
	m.addFloatArg(-0.139804);
	m.addFloatArg(0.982142);
	m.addFloatArg(-0.125909);
	m.addStringArg("Head");
	m.addFloatArg(235.309998);
	m.addFloatArg(133.819000);
	m.addFloatArg(101.378998);
	m.addFloatArg(169.626999);
	m.addFloatArg(0.018763);
	m.addFloatArg(-0.990580);
	m.addFloatArg(0.135641);
	m.addStringArg("LeftHip");
	m.addFloatArg(245.666000);
	m.addFloatArg(75.216003);
	m.addFloatArg(120.654999);
	m.addFloatArg(161.451004);
	m.addFloatArg(0.408582);
	m.addFloatArg(-0.135829);
	m.addFloatArg(-0.902558);
	m.addStringArg("LeftKnee");
	m.addFloatArg(253.108002);
	m.addFloatArg(38.409901);
	m.addFloatArg(135.796997);
	m.addFloatArg(152.246002);
	m.addFloatArg(0.401419);
	m.addFloatArg(-0.174520);
	m.addFloatArg(-0.899114);
	m.addStringArg("LeftAnkle");
	m.addFloatArg(262.636993);
	m.addFloatArg(10.898100);
	m.addFloatArg(151.845001);
	m.addFloatArg(153.141998);
	m.addFloatArg(0.400977);
	m.addFloatArg(-0.172086);
	m.addFloatArg(-0.899780);
	m.addStringArg("LeftToe");
	m.addFloatArg(255.608002);
	m.addFloatArg(2.845880);
	m.addFloatArg(148.485001);
	m.addFloatArg(131.498001);
	m.addFloatArg(-0.913002);
	m.addFloatArg(0.314403);
	m.addFloatArg(0.259959);
	m.addStringArg("RightHip");
	m.addFloatArg(256.976013);
	m.addFloatArg(78.787201);
	m.addFloatArg(111.195999);
	m.addFloatArg(169.923004);
	m.addFloatArg(-0.474420);
	m.addFloatArg(-0.106468);
	m.addFloatArg(0.873837);
	m.addStringArg("RightKnee");
	m.addFloatArg(254.621002);
	m.addFloatArg(36.624401);
	m.addFloatArg(99.391403);
	m.addFloatArg(163.516006);
	m.addFloatArg(0.488914);
	m.addFloatArg(-0.021718);
	m.addFloatArg(-0.872062);
	m.addStringArg("RightAnkle");
	m.addFloatArg(262.174011);
	m.addFloatArg(4.701830);
	m.addFloatArg(105.250999);
	m.addFloatArg(174.145004);
	m.addFloatArg(0.499286);
	m.addFloatArg(0.046724);
	m.addFloatArg(-0.865177);
	m.addStringArg("RightToe");
	m.addFloatArg(258.763000);
	m.addFloatArg(1.084880);
	m.addFloatArg(95.192497);
	m.addFloatArg(92.044998);
	m.addFloatArg(-0.936265);
	m.addFloatArg(0.123226);
	m.addFloatArg(0.328974);
	m.addStringArg("LeftCollar");
	m.addFloatArg(234.720001);
	m.addFloatArg(120.119003);
	m.addFloatArg(105.606003);
	m.addFloatArg(155.574005);
	m.addFloatArg(-0.405851);
	m.addFloatArg(0.811648);
	m.addFloatArg(0.420135);
	m.addStringArg("LeftShoulder");
	m.addFloatArg(224.257004);
	m.addFloatArg(127.244003);
	m.addFloatArg(112.219002);
	m.addFloatArg(156.610992);
	m.addFloatArg(-0.394913);
	m.addFloatArg(0.729760);
	m.addFloatArg(0.558117);
	m.addStringArg("LeftElbow");
	m.addFloatArg(224.542999);
	m.addFloatArg(124.559998);
	m.addFloatArg(114.515999);
	m.addFloatArg(116.910004);
	m.addFloatArg(-0.380580);
	m.addFloatArg(0.873085);
	m.addFloatArg(-0.304764);
	m.addStringArg("LeftWrist");
	m.addFloatArg(228.990005);
	m.addFloatArg(104.417000);
	m.addFloatArg(136.498993);
	m.addFloatArg(170.459000);
	m.addFloatArg(0.623000);
	m.addFloatArg(-0.079013);
	m.addFloatArg(-0.778221);
	m.addStringArg("LeftHand");
	m.addFloatArg(226.916000);
	m.addFloatArg(82.846199);
	m.addFloatArg(140.419998);
	m.addFloatArg(169.029999);
	m.addFloatArg(0.978735);
	m.addFloatArg(0.161237);
	m.addFloatArg(-0.126810);
	m.addStringArg("RightCollar");
	m.addFloatArg(238.464005);
	m.addFloatArg(119.768997);
	m.addFloatArg(101.885002);
	m.addFloatArg(94.846397);
	m.addFloatArg(-0.104862);
	m.addFloatArg(0.595807);
	m.addFloatArg(-0.796252);
	m.addStringArg("RightShoulder");
	m.addFloatArg(247.453003);
	m.addFloatArg(125.990997);
	m.addFloatArg(92.695099);
	m.addFloatArg(131.738998);
	m.addFloatArg(0.196661);
	m.addFloatArg(0.675267);
	m.addFloatArg(-0.710872);
	m.addStringArg("RightElbow");
	m.addFloatArg(249.602997);
	m.addFloatArg(123.250000);
	m.addFloatArg(93.344704);
	m.addFloatArg(149.238998);
	m.addFloatArg(-0.350252);
	m.addFloatArg(0.935557);
	m.addFloatArg(-0.045363);
	m.addStringArg("RightWrist");
	m.addFloatArg(267.182007);
	m.addFloatArg(100.130997);
	m.addFloatArg(99.022697);
	m.addFloatArg(165.927994);
	m.addFloatArg(-0.205011);
	m.addFloatArg(-0.075559);
	m.addFloatArg(0.975839);
	m.addStringArg("RightHand");
	m.addFloatArg(262.341003);
	m.addFloatArg(80.217003);
	m.addFloatArg(92.044296);
	m.addFloatArg(156.623001);
	m.addFloatArg(0.569904);
	m.addFloatArg(-0.026441);
	m.addFloatArg(0.821286);
	m.addFloatArg(942.367004);
	
	actor.updateWithOscMessage(m);
}

//TEST(ramNodeOpTest, Eq)
//{
//	ramActor actor0;
//	ramActor actor1;
//	
//	makeTestActor(actor0);
//	makeTestActor(actor1);
//	
//	ASSERT_EQ(actor0, actor1);
//}
//
//TEST(ramNodeOpTest, NotEq)
//{
//	ramActor actor0;
//	ramActor actor1;
//	
//	makeTestActor(actor0);
//	makeTestActor(actor1);
//	
//	actor0.getNode(0).move(100, 0, 0);
//	
//	ASSERT_NE(actor0, actor1);
//}

TEST(ramNodeOpTest, AddSub)
{
	ramActor actor0;
	ramActor actor1;
	
	makeTestActor(actor0);
	makeTestActor(actor1);
	
	ramActor d = actor0 + actor0;
	ramActor dd = d - actor0;
	
	ASSERT_EQ(actor0, dd);
}

//TEST(ramNodeOpTest, AddScale)
//{
//	ramActor actor0;
//	ramActor actor1;
//	
//	makeTestActor(actor0);
//	makeTestActor(actor1);
//	
//	ramActor d = actor0 + actor0;
//	ramActor dd = actor0 * 2;
//	
//	ASSERT_EQ(d, dd);
//}
//
//TEST(ramNodeOpTest, BigAddScale)
//{
//	ramActor actor0;
//	ramActor actor1;
//	
//	makeTestActor(actor0);
//	makeTestActor(actor1);
//	
//	ramActor d = actor0;
//	for (int i = 0; i < 99; i++)
//	{
//		d += actor0;
//	}
//	
//	ramActor dd = actor0 * 100;
//	
//	ASSERT_EQ(d, dd);
//}
//
//
//TEST(ramNodeOpTest, MulAdd)
//{
//	ramActor actor0;
//	ramActor actor1;
//	
//	makeTestActor(actor0);
//	makeTestActor(actor1);
//	
////	ramActor a =
//	
////	ASSERT_NE(d, dd);
//}


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofExit(ofxGoogleTest::run());
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
	/// ram setup
	// ------------------
	ramInitialize();
	oscReceiver.setup(10000);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}



#pragma mark - ram methods

ramNodeArray arr;

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
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

