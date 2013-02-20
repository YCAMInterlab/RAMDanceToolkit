#pragma once

#include "ofMain.h"

class ramControlPanelBase
{
public:
	
	virtual ~ramControlPanelBase() {}
	
	virtual void setup() {}
	
	// add & remove element
	
	virtual void addPanel(const string& name) {}
	virtual void addSection(const string& name) {}
	
	virtual void addSeparator() {}
	
	virtual void addLabel(const string& content) {}
	
	template <typename Functor>
	void addButton(const string& name, const Functor &functor) {}
	
	virtual void addToggle(const string& name, bool *value) {}
	
	virtual void addMultiToggle(const string& name, const vector<string>& content, int *value) {}
	
	virtual void addRadioGroup(const string& name, const vector<string>& content, int *value) {}
	virtual void addDropdown(const string& name, const vector<string>& content, int *value) {}
	
	virtual void addSlider(const string& name, float min_value, float max_value, float *value) {}
	virtual void addColorSelector(const string& name, ofColor *value) {}
	
	virtual void remove(const string& name) {}
	
	// accessor
	
	template <typename T>
	bool get(const string &name, T& value) { assert(false); }
	
	template <typename T>
	bool set(const string &name, const T& value) { assert(false); }
	
	// visible
	
	virtual void show() {}
	virtual void hide() {}
	virtual void toggle() {}
	virtual bool isVisible() { return false; }
	
	// load & save
	
	virtual void save(const string& path) {}
	virtual void load(const string& path) {}
	
protected:
	
	struct ICallable
    {
        virtual ~ICallable() {}
        virtual void call() {}
    };
	
    template <typename T>
    struct Callback : public ICallable
    {
        T func;
        Callback(const T& func) : func(func) {}
		
        void call() { func(); }
    };
	
};
