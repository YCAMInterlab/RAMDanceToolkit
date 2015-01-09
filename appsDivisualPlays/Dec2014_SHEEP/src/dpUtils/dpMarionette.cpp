//
//  dpMarionette.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/08.
//
//

#include "dpMarionette.h"

dpMarionette* dpMarionette::__instance = NULL;

dpMarionette::dpMarionette(){

	mMarioA_parts[0].loadImage("dpMarionetteA/head_neck.png");
	mMarioA_parts[1].loadImage("dpMarionetteA/neck_shoulder.png");
	mMarioA_parts[2].loadImage("dpMarionetteA/6_l_uarm.png");
	mMarioA_parts[3].loadImage("dpMarionetteA/7_l_farm.png");
	mMarioA_parts[4].loadImage("dpMarionetteA/6_r_uarm.png");
	mMarioA_parts[5].loadImage("dpMarionetteA/7_r_farm.png");
	mMarioA_parts[6].loadImage("dpMarionetteA/l_wrist_hand.png");
	mMarioA_parts[7].loadImage("dpMarionetteA/r_wrist_hand.png");

	mMarioA_parts[8].loadImage("dpMarionetteA/4_chest.png");
	mMarioA_parts[9].loadImage("dpMarionetteA/5_lower.png");

	mMarioA_parts[10].loadImage("dpMarionetteA/10_l_crotch.png");
	mMarioA_parts[11].loadImage("dpMarionetteA/10_r_crotch.png");
	mMarioA_parts[12].loadImage("dpMarionetteA/11_l_shin.png");
	mMarioA_parts[13].loadImage("dpMarionetteA/11_r_shin.png");
	mMarioA_parts[14].loadImage("dpMarionetteA/12_l_foot.png");
	mMarioA_parts[15].loadImage("dpMarionetteA/12_r_foot.png");

}

void dpMarionette::drawActor(const ramActor &actor){

	ofSetColor(255);

	glAlphaFunc(GL_GREATER, 0);
	glEnable(GL_ALPHA_TEST);

	drawBasicRibbon(mMarioA_parts[3].getTextureReference(),
					actor.getNode(ramActor::JOINT_LEFT_ELBOW),
					actor.getNode(ramActor::JOINT_LEFT_WRIST), 8, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawElbowPoint(mMarioA_parts[4].getTextureReference(),
				   actor.getNode(ramActor::JOINT_RIGHT_ELBOW),
				   actor.getNode(ramActor::JOINT_LEFT_COLLAR),
				   actor.getNode(ramActor::JOINT_RIGHT_COLLAR), 8, 10);

	drawElbowPoint(mMarioA_parts[2].getTextureReference(),
				   actor.getNode(ramActor::JOINT_LEFT_ELBOW),
				   actor.getNode(ramActor::JOINT_RIGHT_SHOULDER),
				   actor.getNode(ramActor::JOINT_LEFT_SHOULDER), 8, 10,true);


	drawBasicRibbon(mMarioA_parts[5].getTextureReference(),
					actor.getNode(ramActor::JOINT_RIGHT_ELBOW),
					actor.getNode(ramActor::JOINT_RIGHT_WRIST), 8, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[6].getTextureReference(),
					actor.getNode(ramActor::JOINT_LEFT_WRIST),
					actor.getNode(ramActor::JOINT_LEFT_HAND), 5, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, -10);

	drawBasicRibbon(mMarioA_parts[7].getTextureReference(),
					actor.getNode(ramActor::JOINT_RIGHT_WRIST),
					actor.getNode(ramActor::JOINT_RIGHT_HAND), 5, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, -10);

	drawShoulderPoint(mMarioA_parts[1].getTextureReference(),
					  actor.getNode(ramActor::JOINT_NECK),
					  actor.getNode(ramActor::JOINT_LEFT_COLLAR),
					  actor.getNode(ramActor::JOINT_RIGHT_COLLAR), 50, 10);

	drawChestPoint(mMarioA_parts[8].getTextureReference(),
				   actor.getNode(ramActor::JOINT_ABDOMEN),
				   actor.getNode(ramActor::JOINT_LEFT_COLLAR),
				   actor.getNode(ramActor::JOINT_RIGHT_COLLAR), 50, 10);

	drawBasicRibbon(mMarioA_parts[9].getTextureReference(),
					actor.getNode(ramActor::JOINT_ABDOMEN),
					actor.getNode(ramActor::JOINT_HIPS), 34,10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[10].getTextureReference(),
					actor.getNode(ramActor::JOINT_LEFT_HIP),
					actor.getNode(ramActor::JOINT_LEFT_KNEE), 15, 15,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[11].getTextureReference(),
					actor.getNode(ramActor::JOINT_RIGHT_HIP),
					actor.getNode(ramActor::JOINT_RIGHT_KNEE), 15, 15,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[12].getTextureReference(),
					actor.getNode(ramActor::JOINT_LEFT_KNEE),
					actor.getNode(ramActor::JOINT_LEFT_ANKLE), 10, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[13].getTextureReference(),
					actor.getNode(ramActor::JOINT_RIGHT_KNEE),
					actor.getNode(ramActor::JOINT_RIGHT_ANKLE), 10, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, 0);

	drawBasicRibbon(mMarioA_parts[14].getTextureReference(),
					actor.getNode(ramActor::JOINT_LEFT_ANKLE),
					actor.getNode(ramActor::JOINT_LEFT_TOE), 8, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, -3);

	drawBasicRibbon(mMarioA_parts[15].getTextureReference(),
					actor.getNode(ramActor::JOINT_RIGHT_ANKLE),
					actor.getNode(ramActor::JOINT_RIGHT_TOE), 8, 10,
					DPM_AXIS_X, DPM_AXIS_X, true, 0, -3);

	drawBasicRibbon(mMarioA_parts[0].getTextureReference(),
					actor.getNode(ramActor::JOINT_HEAD),
					actor.getNode(ramActor::JOINT_NECK), 15, 10,
					DPM_AXIS_X, DPM_AXIS_X, true,10,0);



	glDisable(GL_ALPHA_TEST);
}

void dpMarionette::drawBasicRibbon(ofTexture &tex,
								   const ramNode &nodeA,const ramNode &nodeB,
								   float width, int resolution,
								   int A_Axis, int B_Axis,
								   bool wired,int beginOffset, int endOffset){

	ofNode parent[2];
	parent[0] = nodeA;
	parent[1] = nodeB;

	ofNode child[4];
	child[0].setParent(parent[0]);
	child[1].setParent(parent[0]);
	child[2].setParent(parent[1]);
	child[3].setParent(parent[1]);

	ofVec3f aPos,bPos;
	if (A_Axis == DPM_AXIS_X) aPos.set(width/2.0, beginOffset, 0.0);
	if (A_Axis == DPM_AXIS_Y) aPos.set(0.0, width/2.0, 0.0);
	if (A_Axis == DPM_AXIS_Z) aPos.set(0.0, beginOffset, width/2.0);

	if (B_Axis == DPM_AXIS_X) bPos.set(width/2.0, endOffset, 0.0);
	if (B_Axis == DPM_AXIS_Y) bPos.set(0.0, width/2.0, 0.0);
	if (B_Axis == DPM_AXIS_Z) bPos.set(0.0, endOffset, width/2.0);

	child[1].setPosition(aPos);
	child[0].setPosition(aPos.x * -1, aPos.y, aPos.z * -1);
	child[3].setPosition(bPos);
	child[2].setPosition(bPos.x * -1, bPos.y, bPos.z * -1);

	ofVec2f texSize = ofVec2f(tex.getWidth(),tex.getHeight());
	ofVec3f targA,targB;
	float sliceP;

	tex.bind();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0;i < resolution + 1;i++){
		sliceP = i/float(resolution);
		targA = child[0].getGlobalPosition().interpolated(child[2].getGlobalPosition(),
														  sliceP);
		targB = child[1].getGlobalPosition().interpolated(child[3].getGlobalPosition(),
														  sliceP);
		glTexCoord2d(0.0, texSize.y * sliceP);
		glVertex3d(targA.x, targA.y, targA.z);
		glTexCoord2d(texSize.x, texSize.y * sliceP);
		glVertex3d(targB.x, targB.y, targB.z);
	}
	glEnd();
	tex.unbind();

}

void dpMarionette::drawElbowPoint(ofTexture &tex, const ramNode &nodeA, const ramNode &nodeB, const ramNode &nodeC, int width, int resolution, bool isLEFT){

	ofNode parent[3];
	parent[0] = nodeA;
	parent[1] = nodeB;
	parent[2] = nodeC;

	ofNode child[4];
	child[0].setParent(parent[0]);
	child[1].setParent(parent[0]);

	if (isLEFT){
		child[2].setGlobalPosition(parent[2].getGlobalPosition().
								   interpolate(parent[1].getGlobalPosition(),-0.2));
		child[3].setGlobalPosition(parent[2].getGlobalPosition().
								   interpolate(parent[1].getGlobalPosition(), 0.1));
	}else{
		child[2].setGlobalPosition(parent[2].getGlobalPosition().
								   interpolate(parent[1].getGlobalPosition(),-0.2));
		child[3].setGlobalPosition(parent[2].getGlobalPosition().
								   interpolate(parent[1].getGlobalPosition(),-0.4));
	}

	child[0].setPosition( width/2.0, 0.0, 0.0);
	child[1].setPosition(-width/2.0, 0.0, 0.0);

	ofVec2f texSize = ofVec2f(tex.getWidth(),tex.getHeight());
	ofVec3f targA,targB;
	float sliceP;

	tex.bind();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0;i < resolution + 1;i++){
		sliceP = i/float(resolution);
		targA = child[0].getGlobalPosition().interpolated(child[2].getGlobalPosition(),
														  sliceP);
		targB = child[1].getGlobalPosition().interpolated(child[3].getGlobalPosition(),
														  sliceP);

		glTexCoord2d(0.0, texSize.y * (1.0 - sliceP));
		glVertex3d(targA.x, targA.y, targA.z);
		glTexCoord2d(texSize.x, texSize.y * (1.0 - sliceP));
		glVertex3d(targB.x, targB.y, targB.z);

	}
	glEnd();
	tex.unbind();

}

void dpMarionette::drawChestPoint(ofTexture &tex, const ramNode &nodeA, const ramNode &nodeB, const ramNode &nodeC, int width, int resolution){

	ofNode parent[3];
	parent[0] = nodeA;
	parent[1] = nodeB;
	parent[2] = nodeC;

	ofNode child[4];
	child[0].setParent(parent[0]);
	child[1].setParent(parent[0]);
	child[2].setParent(parent[1]);
	child[3].setParent(parent[2]);

	child[0].setPosition( width/2.0, 0.0, 0.0);
	child[1].setPosition(-width/2.0, 0.0, 0.0);

	child[2].setPosition( 12.0, 0.0, 0.0);
	child[3].setPosition(-12.0, 0.0, 0.0);

	ofVec2f texSize = ofVec2f(tex.getWidth(),tex.getHeight());
	ofVec3f targA,targB;
	float sliceP;

	tex.bind();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0;i < resolution + 1;i++){
		sliceP = i/float(resolution);
		targA = child[0].getGlobalPosition().interpolated(child[2].getGlobalPosition(),
														  sliceP);
		targB = child[1].getGlobalPosition().interpolated(child[3].getGlobalPosition(),
														  sliceP);

		glTexCoord2d(0.0, texSize.y * (1.0 - sliceP));
		glVertex3d(targA.x, targA.y, targA.z);
		glTexCoord2d(texSize.x, texSize.y * (1.0 - sliceP));
		glVertex3d(targB.x, targB.y, targB.z);

	}
	glEnd();
	tex.unbind();


}

void dpMarionette::drawShoulderPoint(ofTexture &tex,
									 const ramNode &nodeA,
									 const ramNode &nodeB,
									 const ramNode &nodeC,int width, int resolution){
	//A is NECK!
	ofNode parent[3];
	parent[0] = nodeA;
	parent[1] = nodeB;
	parent[2] = nodeC;

	ofNode child[4];
	child[0].setParent(parent[0]);
	child[1].setParent(parent[0]);
	child[2].setParent(parent[1]);
	child[3].setParent(parent[2]);

	child[0].setPosition( width/2.0, 0.0, 0.0);
	child[1].setPosition(-width/2.0, 0.0, 0.0);

	child[2].setPosition( 12.0, 0.0, 0.0);
	child[3].setPosition(-12.0, 0.0, 0.0);

	ofVec2f texSize = ofVec2f(tex.getWidth(),tex.getHeight());
	ofVec3f targA,targB;
	float sliceP;

	tex.bind();
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0;i < resolution + 1;i++){
		sliceP = i/float(resolution);
		targA = child[0].getGlobalPosition().interpolated(child[2].getGlobalPosition(),
														  sliceP);
		targB = child[1].getGlobalPosition().interpolated(child[3].getGlobalPosition(),
														  sliceP);

		glTexCoord2d(0.0, texSize.y * sliceP);
		glVertex3d(targA.x, targA.y, targA.z);
		glTexCoord2d(texSize.x, texSize.y * sliceP);
		glVertex3d(targB.x, targB.y, targB.z);

	}
	glEnd();
	tex.unbind();

}