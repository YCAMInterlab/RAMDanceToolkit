//
//  dpConductionGUI.cpp
//  dpConductor
//
//  Created by Ovis aries on 2016/01/27.
//
//

#include "dpConductionGUI.h"

void dpConductionGUI::setup()
{
	listUp();
	
	sceneList.setup();
	sceneList.setPosition(10, 10);
	currentScene = sceneList.addRadio("Scenes", scenes_for_test);
	sceneList.autoSizeToFitWidgets();
	
	dynamicUI.setup();
	dynamicUI.setPosition(600, 10);
	dynamicUI.clearWidgets();
	
	gui.setFontSize(OFX_UI_FONT_LARGE, 48);
	gui.setup();
	gui.setPosition(300, 10);
	
	gui.addToggle("Exclusive", &bExclusive);
	gui.addButton("Enable", false);
	gui.addButton("Disable", false);
	gui.addButton("requestUI", false);
	gui.addSpacer();
	actorSel = gui.addDropDownList("Actor", ActorList);
	jointSel = gui.addDropDownList("jointList", jointList);
	gui.addButton("PushExt", false);
	gui.addButton("clear", false);
	gui.addSpacer();
	gui.addLabel("Sections");
	for (int i = 0;i < conPtr->sections.size();i++)
	{
		gui.addButton("Que:"+conPtr->sections[i]->sectionName, false);
	}
	gui.autoSizeToFitWidgets();
	
	ofAddListener(gui.newGUIEvent, this, &dpConductionGUI::event);
	ofAddListener(dynamicUI.newGUIEvent, this, &dpConductionGUI::event);
	
	currentScene->activateToggle(SCENE_LINE);
	
	jointSel->setAutoClose(true);
	actorSel->setAutoClose(true);
}

void dpConductionGUI::setDUI(ofxOscMessage m)
{
	if (m.getArgAsString(0) != currentDUI)
	{
		dynamicUI.clearWidgets();
		currentDUI = m.getArgAsString(0);
		dynamicUI.addLabel(currentDUI, OFX_UI_FONT_LARGE);
	}
	
	if (m.getArgAsInt32(1) == OFX_UI_WIDGET_SLIDER_H)
	{
		dynamicUI.addSlider(m.getArgAsString(2),
							m.getArgAsFloat(4),
							m.getArgAsFloat(5),
							m.getArgAsFloat(3));
	}
	
	if (m.getArgAsInt32(1) == OFX_UI_WIDGET_TOGGLE)
	{
		dynamicUI.addToggle(m.getArgAsString(2),m.getArgAsInt32(3));
	}
	
	if (m.getArgAsInt32(1) == OFX_UI_WIDGET_BUTTON)
	{
		dynamicUI.addButton(m.getArgAsString(2), false);
	}
	
	if (m.getArgAsInt32(1) == OFX_UI_WIDGET_SPACER)
	{
		dynamicUI.addSpacer();
	}
	
	dynamicUI.autoSizeToFitWidgets();
}

void dpConductionGUI::event(ofxUIEventArgs &e)
{
	ofxUIWidget* w = e.widget;
	
	
	if ((jointSel->getSelectedNames().size() > 0) && (w == jointSel))
		jointSel->setLabelText(jointSel->getSelectedNames()[0]);
	
	if ((actorSel->getSelectedNames().size() > 0) && (w == actorSel))
		actorSel->setLabelText(actorSel->getSelectedNames()[0]);
	
	if (w->getName() == "Enable"  && w->getState() == OFX_UI_STATE_DOWN)
	{
		if (bExclusive) conPtr->sceneCon->clearScene();
		conPtr->sceneCon->setScene(currentScene->getActiveName(), true, true, true);
		conPtr->sceneCon->setScene(currentScene->getActiveName(), false, true, true);
	}
	
	if (w->getName() == "Disable" && w->getState() == OFX_UI_STATE_DOWN)
	{
		if (bExclusive) conPtr->sceneCon->clearScene();
		conPtr->sceneCon->disableScene(currentScene->getActiveName(), true);
		conPtr->sceneCon->disableScene(currentScene->getActiveName(), false);
	}
	
	if ((w->getName() == "requestUI") && (w->getState() == OFX_UI_STATE_DOWN))
	{
		requestCurrentUI();
	}
	
	if (w->getParent() == &dynamicUI)
	{
		if (w->getKind() == OFX_UI_WIDGET_SLIDER_H)
		{
			ofxUISlider* s = ((ofxUISlider*)(w));
			conPtr->sceneCon->setFloatTune(currentDUI, w->getName(), s->getValue());
		}
		
		if (w->getKind() == OFX_UI_WIDGET_TOGGLE)
		{
			ofxUIToggle* t = ((ofxUIToggle*)(w));
			conPtr->sceneCon->setToggleTune(currentDUI, w->getName(), t->getValue());
		}
		
		if (w->getKind() == OFX_UI_WIDGET_BUTTON && w->getState() == OFX_UI_STATE_DOWN)
		{
			conPtr->sceneCon->setButtonTune(currentDUI, w->getName());
		}
	}
	
	if (w->getName().substr(0,3) == "Que" && w->getState() == OFX_UI_STATE_DOWN)
	{
		string name = ofSplitString(w->getName(), ":")[1];
		conPtr->callSection(name);
	}
	
	if ((w->getName() == "PushExt" && w->getState() == OFX_UI_STATE_DOWN) &&
		(actorSel->getSelectedNames().size() > 0) && (jointSel->getSelectedNames().size() > 0))
	{
		conPtr->sceneCon->setExtractor(currentScene->getActiveName(),
									   actorSel->getSelectedNames()[0],
									   (Joint)jointSel->getSelectedIndeces()[0]);
	}
}

void dpConductionGUI::setScene(ofxOscMessage m)
{
	if (m.getArgAsInt32(1) == 1)
	{
		currentScene->activateToggle(m.getArgAsString(0));
		requestCurrentUI();
	}
	
}

void dpConductionGUI::requestCurrentUI()
{
	dynamicUI.clearWidgets();
	ofxOscSender sender;
	ofxOscMessage m;
	m.setAddress("/ram/tuneList/"+currentScene->getActiveName());
	
	sender.setup(conPtr->sceneCon->addr_rdtk2, 10000);
	sender.sendMessage(m);
}

void dpConductionGUI::listUp()
{
	scenes_for_test.push_back(SCENE_CAMERA);
	scenes_for_test.push_back(SCENE_ACTOR);
	scenes_for_test.push_back(SCENE_LINE);
	scenes_for_test.push_back(SCENE_BIGBOX);
	scenes_for_test.push_back(SCENE_DONUTS);
	scenes_for_test.push_back(SCENE_MONSTER);
	scenes_for_test.push_back(SCENE_METABALL);
	scenes_for_test.push_back(SCENE_FOURPT);
	scenes_for_test.push_back(SCENE_THREEPT);
	
	ActorList.push_back("kojiri");
	ActorList.push_back("miyashita");
	ActorList.push_back("shimaji");
	ActorList.push_back("ando");
	
	jointList.push_back("HIPS");
	jointList.push_back("ABDOMEN");
	jointList.push_back("CHEST");
	jointList.push_back("NECK");
	jointList.push_back("HEAD");
	jointList.push_back("L_HIP");
	jointList.push_back("L_KNEE");
	jointList.push_back("L_ANKLE");
	jointList.push_back("L_TOE");
	
	jointList.push_back("R_HIP");
	jointList.push_back("R_KNEE");
	jointList.push_back("R_ANKLE");
	jointList.push_back("R_TOE");

	jointList.push_back("L_COLLAR");
	jointList.push_back("L_SHOULDER");
	jointList.push_back("L_ELBOW");
	jointList.push_back("L_WRIST");
	jointList.push_back("L_HAND");;

	jointList.push_back("R_COLLAR");
	jointList.push_back("R_SHOULDER");
	jointList.push_back("R_ELBOW");
	jointList.push_back("R_WRIST");
	jointList.push_back("R_HAND");;
}