#include "ramGwenControlPanel.h"

#ifdef RAM_GUI_SYSTEM_GWEN

#include "Gwen/Controls/RadioButtonController.h"

ramGwenControlPanel *ramGwenControlPanel::_instance = NULL;

#define NA() { cout << "not impld" << __PRETTY_FUNCTION__ << endl; assert(false); }

using namespace Gwen;
using namespace Gwen::Controls;

#pragma mark - ramGwenControlPanel


//

class ramGwenControlPanel::Separator : public Gwen::Controls::Base
{
public:
	
	GWEN_CONTROL( Separator, Gwen::Controls::Base );
	
	void Render( Skin::Base* skin )
	{
		Gwen::Point p = LocalPosToCanvas();
		
		glPushMatrix();
		glTranslatef(p.x, p.y, 0);
		ofSetColor(0, 60);
		ofLine(0, 0, Width(), 0);
		glPopMatrix();
	}
	
};

ramGwenControlPanel::Separator::Separator( Gwen::Controls::Base* pParent, const Gwen::String& pName ) : BaseClass( pParent, pName ) {}


struct Group
{
	Gwen::Controls::Base *backup;
	Gwen::Controls::Base **ptr;
	
	Group(const string& name, Gwen::Controls::Base*& base, int height = 30)
	{
		backup = base;
		ptr = &base;
		
		(*ptr) = new Gwen::Controls::Base(backup);
		(*ptr)->SetName(name);
		(*ptr)->SetSize(100, height);
		(*ptr)->Dock(Pos::Top);
	}
	
	~Group()
	{
		*ptr = backup;
	}
};

//


ofColor uiThemecb(64, 192), uiThemeco(192, 192), uiThemecoh(128, 192), uiThemecf(240, 255), uiThemecfh(128, 255), uiThemecp(96, 192), uiThemecpo(255, 192);

ramGwenControlPanel& ramGwenControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramGwenControlPanel();
	}
	return *_instance;
}

ramGwenControlPanel::ramGwenControlPanel()
{
	mFloorPattern = ramFloor::FLOOR_NONE;
	mFloorSize = 600.0;
	mGridSize = 50.0;
	enableShadow = true;
	mPause = false;
	
	camera_preset = camera_preset_t = 0;
	
	scenes = NULL;
}

Gwen::Controls::Base* ramGwenControlPanel::getCurrentSection()
{
	return section_stack.top();
}

void ramGwenControlPanel::pushSection(const string& name, int height)
{
	Gwen::Controls::Base *base = new Gwen::Controls::Base(getCurrentSection());
	base->Dock(Pos::Top);
	base->SetName(name);
	base->SetSize(100, height);
	section_stack.push(base);
}

void ramGwenControlPanel::popSection()
{
	section_stack.pop();
}

void ramGwenControlPanel::setup()
{
	gwen.setup();
	
	ofAddListener(ofEvents().keyPressed, this, &ramGwenControlPanel::keyPressed);
	
	Gwen::Controls::Canvas *canvas = gwen.getCanvas();
	dock = new Gwen::Controls::DockBase(canvas);
	
	dock->Dock(Pos::Fill);
	dock->SetSize(canvas->GetSize().x, canvas->GetSize().y);
	
	dock->GetLeft()->GetTabControl()->SetTabStripPosition(Pos::Right);
	dock->GetLeft()->SetWidth(DEFAULT_WIDGET_WIDTH);
	
	
	// interactive camera enable/disable event hook
	{
		struct Handler : public Gwen::Event::Handler
		{
			void handle()
			{
				ramCameraManager::instance().setEnableInteractiveCamera(true);
			}
		};
		static Handler evt;
		dock->onHoverEnter.Add(&evt, &Handler::handle);
	}
	
	{
		struct Handler : public Gwen::Event::Handler
		{
			void handle()
			{
				ramCameraManager::instance().setEnableInteractiveCamera(false);
			}
		};
		static Handler evt;
		dock->onHoverLeave.Add(&evt, &Handler::handle);
	}
	
	//
	
	addPanel("RAMDanceToolkit");
	
	struct FullScreen
	{
		FullScreen() {}
		
		void operator()()
		{
			ofToggleFullscreen();
		}
	};
	
	addButton("Fullscreen", FullScreen());
	addSeparator();
	addToggle("Pause", &mPause);
	
	addColorSelector("Background", &mBGColor);
	
	addRadioGroup("Floor Patterns", ramFloor::getFloorNames(), &mFloorPattern);
	addSlider("Floor Size", 100, 1000, &mFloorSize);
	addSlider("Grid Size", 20, 200, &mGridSize);
	
	addDropdown("Camera Preset", ramCameraManager::instance().getDefaultCameraNames(), &mCameraPreset);
	
	ofAddListener(ofEvents().update, this, &ramGwenControlPanel::update);
}

void ramGwenControlPanel::update(ofEventArgs &e)
{
	ofBackground(mBGColor);
	
	ramActorManager::instance().setFreezed(mPause);
	
	{
		static int t = mCameraPreset;
		if (t != mCameraPreset)
		{
			reloadCameraSetting(mCameraPreset);
		}
		t = mCameraPreset;
	}
}

//

void ramGwenControlPanel::addPanel(ramControllable* control)
{
	ofLog() << "log";
//	section_stack = stack<Gwen::Controls::Base*>();
//	
//	Gwen::Controls::Base *base = new Gwen::Controls::Base(dock);
//	section_stack.push(base);
//	
//	dock->GetLeft()->GetTabControl()->AddPage(name, getCurrentSection());
//	getCurrentSection()->RestrictToParent(true);
//	getCurrentSection()->SetName(name);
//	getCurrentSection()->SetPadding(Padding(4, 4, 4, 4));
//	
//	getCurrentSection()->Dock(Pos::Fill);
}

void ramGwenControlPanel::addPanel(const string& name)
{
	section_stack = stack<Gwen::Controls::Base*>();
	
	Gwen::Controls::Base *base = new Gwen::Controls::Base(dock);
	section_stack.push(base);
	
	dock->GetLeft()->GetTabControl()->AddPage(name, getCurrentSection());
	getCurrentSection()->RestrictToParent(true);
	getCurrentSection()->SetName(name);
	getCurrentSection()->SetPadding(Padding(4, 4, 4, 4));
	
	getCurrentSection()->Dock(Pos::Fill);
}

void ramGwenControlPanel::addSection(const string& name)
{
	Gwen::Controls::Label *obj = new Gwen::Controls::Label(getCurrentSection());
	obj->Dock(Pos::Top);
	obj->SetText(name);
	obj->SetMargin(Margin(5, 5, 5, 5));
	obj->SizeToContents();
	
	obj->SetFont(L"/Library/Fonts/Arial.ttf", 16, true);
	
	addSeparator();
}

void ramGwenControlPanel::addSeparator()
{
	Separator *obj = new Separator(getCurrentSection());
	obj->Dock(Pos::Top);
	obj->SetMargin(Margin(3, 6, 3, 6));
	obj->SetSize(100, 1);
}

void ramGwenControlPanel::addLabel(const string& content)
{
	Gwen::Controls::Label *obj = new Gwen::Controls::Label(getCurrentSection());
	obj->Dock(Pos::Top);
	obj->SetText(content);
	obj->SizeToContents();
	obj->SetMargin(Margin(6, 0, 0, 0));
}

void ramGwenControlPanel::addToggle(const string& name, bool *value)
{
	pushSection(name, 35);
	
	Gwen::Controls::CheckBoxWithLabel* obj = new Gwen::Controls::CheckBoxWithLabel(getCurrentSection());
	
	obj->Label()->SetText(name);
	obj->Dock(Pos::Top);
	obj->SetMargin(Margin(5, 5, 5, 5));
	
	if (value)
	{
		obj->Checkbox()->SetChecked(*value);
		
		typedef Gwen::Controls::CheckBox Control;
		struct Handler : public Gwen::Event::Handler
		{
			void handle(Gwen::Controls::Base *obj)
			{
				if (obj->UserData.Exists("ptr"))
				{
					bool *value = obj->UserData.Get<bool*>("ptr");
					Control *c = gwen_cast<Control>(obj);
					*value = c->IsChecked();
				}
			}
		};
		
		static Handler evt;
		obj->Checkbox()->UserData.Set<bool*>("ptr", value);
		obj->Checkbox()->onCheckChanged.Add(&evt, &Handler::handle);
	}
	
	popSection();
}

void ramGwenControlPanel::addMultiToggle(const string& name, const vector<string>& content, int *value)
{
	pushSection(name, 100);
	
	addLabel(name);
	
	Gwen::Controls::Layout::Tile* obj = new Gwen::Controls::Layout::Tile(getCurrentSection());
	obj->SetTileSize(40, 40);
	
	NA();
	
	popSection();
}

void ramGwenControlPanel::addRadioGroup(const string& name, const vector<string>& content, int *value)
{
	pushSection(name, 24 * content.size() + 10);
	
	addLabel(name);
	
	Gwen::Controls::RadioButtonController* obj = new Gwen::Controls::RadioButtonController(getCurrentSection());
	
	obj->Dock(Pos::Top);
	obj->SetMargin(Margin(5, 2, 5, 5));
	
	Gwen::Point size;
	
	for (int i = 0; i < content.size(); i++)
	{
		const string &s = content[i];
		LabeledRadioButton *btn = obj->AddOption(Utility::StringToUnicode(s));
		size.x = max(size.x, btn->Right());
		size.y += btn->Bottom();
		
		btn->UserData.Set<int>("id", i);
		
		if (value && *value == i)
		{
			btn->Select();
		}
	}
	
	size.y += ELEMENT_MARGIN;
	
	obj->SetSize(100, size.y);
	
	if (value)
	{
		
		typedef Gwen::Controls::RadioButtonController Control;
		struct Handler : public Gwen::Event::Handler
		{
			void handle(Gwen::Controls::Base *obj)
			{
				int *value = obj->UserData.Get<int*>("ptr");
				
				Control *c = gwen_cast<Control>(obj);
				LabeledRadioButton *btn = c->GetSelected();
				
				if (value)
				{
					*value = btn->UserData.Get<int>("id");
				}
			}
		};
		
		static Handler evt;
		obj->UserData.Set<int*>("ptr", value);
		obj->onSelectionChange.Add(&evt, &Handler::handle);
	}
	
	popSection();
}

void ramGwenControlPanel::addDropdown(const string& name, const vector<string>& content, int *value)
{
	pushSection(name, 48);
	
	addLabel(name);
	
	Gwen::Controls::ComboBox* obj = new Gwen::Controls::ComboBox(getCurrentSection());
	obj->SetSize(100, 24);
	obj->Dock(Pos::Top);
	obj->SetMargin(Margin(4, 4, 4, 6));
	
	for (int i = 0; i < content.size(); i++)
	{
		const string &s = content[i];
		Gwen::Controls::MenuItem* item = obj->AddItem(Utility::StringToUnicode(s));
		
		if (value)
		{
			struct Handler : public Gwen::Event::Handler
			{
				void handle(Gwen::Controls::Base *obj)
				{
					int *value = obj->UserData.Get<int*>("ptr");
					*value = obj->UserData.Get<int>("id");
				}
			};
			
			static Handler evt;
			item->UserData.Set<int*>("ptr", value);
			item->UserData.Set<int>("id", i);
			item->onMenuItemSelected.Add(&evt, &Handler::handle);
		}
	}
	
	if (value)
	{
		Controls::Base *d = obj->GetChild(*value);
		if (d) obj->OnItemSelected(d);
	}
	
	popSection();
}

void ramGwenControlPanel::addSlider(const string& name, float min_value, float max_value, float *value)
{
	pushSection(name, 35);
	
	Gwen::Controls::Label *label = new Gwen::Controls::Label(getCurrentSection());
	label->Dock(Pos::Top);
	label->SetText(name);
	label->SizeToContents();
	label->SetMargin(Margin(4, 0, 0, 0));
	
	Gwen::Controls::HorizontalSlider* obj = new Gwen::Controls::HorizontalSlider(getCurrentSection());
	obj->SetRange(min_value, max_value);
	obj->SetSize(100, 15);
	obj->Dock(Pos::Top);
	
	if (value)
	{
		obj->SetFloatValue(*value);
		
		typedef Gwen::Controls::Slider Control;
		struct Handler : public Gwen::Event::Handler
		{
			void handle(Gwen::Controls::Base *obj)
			{
				float *value = obj->UserData.Get<float*>("ptr");
				Control *c = gwen_cast<Control>(obj);
				*value = c->GetFloatValue();
				
				string name = obj->UserData.Get<string>("name");
				Gwen::Controls::Label *label = obj->UserData.Get<Gwen::Controls::Label*>("label");
				label->SetText(name + ": " + ofToString(*value));
			}
		};
		
		static Handler evt;
		obj->UserData.Set<float*>("ptr", value);
		obj->UserData.Set<string>("name", name);
		obj->UserData.Set<Gwen::Controls::Label*>("label", label);
		obj->onValueChanged.Add(&evt, &Handler::handle);
		
		string s = name + ": " + ofToString(*value);
		label->SetText(s);
	}
	
	popSection();
}

void ramGwenControlPanel::addColorSelector(const string& name, ofFloatColor *value)
{
	float height = 60;
	
	pushSection(name, height + 15);
	
	getCurrentSection()->SetMargin(Margin(0, 0, 0, 6));
	
	addLabel(name);
	
	pushSection("", height);
	
	getCurrentSection()->SetPadding(Padding(4, 4, 4, 0));
	
	struct Handler : public Gwen::Event::Handler
	{
		void handle(Gwen::Controls::Base *obj)
		{
			Gwen::Controls::ColorLerpBox *box = obj->UserData.Get<Gwen::Controls::ColorLerpBox*>("box");
			ofColor *value = obj->UserData.Get<ofColor*>("value");
			
			if (obj->UserData.Exists("slider_updated"))
			{
				Gwen::Controls::ColorSlider *slider = (Gwen::Controls::ColorSlider*)obj;
				box->SetColor(slider->GetSelectedColor());
			}
			
			Gwen::Color c = box->GetSelectedColor();
			if (value)
			{
				value->r = c.r * 255;
				value->g = c.g * 255;
				value->b = c.b * 255;
			}
		}
	};
	
	Gwen::Controls::ColorLerpBox *box = new Gwen::Controls::ColorLerpBox(getCurrentSection());
	Gwen::Controls::ColorSlider *slider = new Gwen::Controls::ColorSlider(getCurrentSection());
	
	if (value)
	{
		Gwen::Color c;
		c.r = value->r;
		c.g = value->g;
		c.b = value->b;
		slider->SetColor(c);
		box->SetColor(c);
	}
	
	box->Dock(Pos::Left);
	box->SetSize(190, height);
	box->UserData.Set<Gwen::Controls::ColorSlider*>("slider", slider);
	box->UserData.Set<Gwen::Controls::ColorLerpBox*>("box", box);
	box->UserData.Set<ofFloatColor*>("value", value);
	
	slider->Dock(Pos::Left);
	slider->UserData.Set<Gwen::Controls::ColorLerpBox*>("slider_updated", NULL);
	slider->UserData.Set<Gwen::Controls::ColorSlider*>("slider", slider);
	slider->UserData.Set<Gwen::Controls::ColorLerpBox*>("box", box);
	slider->UserData.Set<ofFloatColor*>("value", value);
	
	static Handler evt;
	box->onSelectionChanged.Add(&evt, &Handler::handle);
	slider->onSelectionChanged.Add(&evt, &Handler::handle);
	
	popSection();
	
	popSection();
}

void ramGwenControlPanel::remove(const string& name)
{
	NA();
}

//

void ramGwenControlPanel::reloadCameraSetting(const int index)
{
	ramCameraManager::instance().rollbackDefaultCameraSetting(index);
}

void ramGwenControlPanel::setupSceneToggles(vector<ramBaseScene*>& scenes)
{
	Gwen::Controls::Base *ram_panel = dock->FindChildByName("RAMDanceToolkit", true);
	
	for (int i = 0; i < scenes.size(); i++)
	{
		ramBaseScene *scene = scenes[i];
		Gwen::Controls::Button *obj = new Gwen::Controls::Button(ram_panel);
		
		obj->Dock(Pos::Top);
		obj->SetText(scene->getName());
		obj->SetIsToggle(true);
		obj->SetMargin(Margin(5, 5, 5, 5));
		obj->SetSize(100, 30);
		
		typedef Gwen::Controls::Button Control;
		struct Handler : public Gwen::Event::Handler
		{
			void handle(Gwen::Controls::Base *obj)
			{
				ramBaseScene *scene = obj->UserData.Get<ramBaseScene*>("ptr");
				Control *c = gwen_cast<Control>(obj);
				scene->setEnabled(c->GetToggleState());
			}
		};
		
		static Handler evt;
		obj->UserData.Set<ramBaseScene*>("ptr", scene);
		obj->onToggle.Add(&evt, &Handler::handle);
	}
}


void ramGwenControlPanel::keyPressed(ofKeyEventArgs &e)
{
//	if (e.key == ' ')
//	{
//		pause = !pause;
//	}
//	
//	if (e.key == '\t')
//	{
//		mTabbedCanvas.toggleVisible();
//	}
}

#endif