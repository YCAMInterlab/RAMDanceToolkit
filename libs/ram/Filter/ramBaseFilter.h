#pragma once

#include "ofMain.h"

#include "ramControllable.h"
#include "ramControlPanel.h"

#include "ramActor.h"

/** sample code...

   #pragma once

   class ramXXXFilter : public ramBaseFilter
   {
   public:

    const string getName() { return "ramXXXFilter"; }

    ramXXXFilter() {}

    void setupControlPanel()
    {
        ramControlPanel &gui = ramGetGUI();

        gui.addSection(getName());
    }

    void setup()
    {

    }

    const ramNodeArray& update(ramNodeArray& src)
    {
        return src;
    }
   };

 */

class ramBaseFilter : public ramControllable, public ramGlobalShortcut
{
public:

	virtual ~ramBaseFilter() {}

	virtual void setupControlPanel() {}
	virtual void setup() {}

	inline const ramNodeArray& operator()(const ramNodeArray& src)
	{
		return update(src);
	}
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		if (cache.getNumNode() != src.getNumNode())
			cache = src;

		const int f = ofGetFrameNum();
		if (last_update_frame != f)
		{
			last_update_frame = f;
			cache = filter(src);
		}

		return cache;
	}

	virtual const ramNodeArray& get(size_t index = 0) const { return cache; }
	virtual size_t getSize() const { return 1; }

protected:

	ramNodeArray cache;

	virtual const ramNodeArray& filter(const ramNodeArray& src) = 0;

private:

	int last_update_frame;
};

//

class ramBusSend : public ramBaseFilter
{
public:

	const string getName() { return "ramBusSend"; }

	ramBusSend() {}
	ramBusSend(const string &bus_name) { setBusName(bus_name); }

	void setBusName(const string &bus_name) { this->bus_name = bus_name; }
	const string& getBusName() const { return bus_name; }

	const ramNodeArray& get(size_t index = 0) const { static ramNodeArray arr; return arr; }
	size_t getSize() const { return 1; }

protected:

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		ramActorManager::instance().setBus(bus_name, src);
		return src;
	}

private:

	string bus_name;
};

//

template <typename FilterClass>
class ramFilterEach
{
public:

	void clear()
	{
		cache.clear();
		filters.clear();
	}

	const vector<ramNodeArray>& update(const vector<ramNodeArray> &src)
	{
		if (ofGetFrameNum() == last_updated_time) return cache;

		last_updated_time = ofGetFrameNum();

		cache.clear();

		for (int i = 0; i < src.size(); i++)
		{
			const ramNodeArray &NA = src[i];
			const string &name = NA.getName();

			if (!filters.hasKey(name))
				filters.set(name, FilterClass());

			FilterClass &filter = filters[name];
			cache.push_back(filter.update(NA));
		}

		return cache;
	}
	
	inline const vector<ramNodeArray>& get(size_t index = 0) { return cache[index]; }
	inline const size_t getNumFilters() { return filters.size(); }
	
	FilterClass& getFilter(int index) { return filters[index]; }
	FilterClass& getFilter(const string& name) { return filters[name]; }

protected:

	ramCompoundContainer<FilterClass> filters;
	vector<ramNodeArray> cache;

	unsigned int last_updated_time;

};