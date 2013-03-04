#pragma once

#include "ofMain.h"

#include "ramUnit.h"

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
        ramControlPanel &gui = gui();

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

class ramBaseFilter : public ramUnit
{
public:
	const ramNodeArray& operator()(const ramNodeArray& src);
	const ramNodeArray& update(const ramNodeArray& src);
	virtual const ramNodeArray& get(size_t index = 0) const;
	virtual size_t getSize() const;

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

	const string getName();
	ramBusSend();
	ramBusSend(const string &bus_name);

	void setBusName(const string &bus_name);
	const string& getBusName() const;

	const ramNodeArray& get(size_t index = 0) const;
	size_t getSize() const;

protected:

	const ramNodeArray& filter(const ramNodeArray& src);

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
	
	const vector<ramNodeArray>& get(size_t index = 0) { return cache[index]; }
	const size_t getNumFilters() { return filters.size(); }
	
	FilterClass& getFilter(int index) { return filters[index]; }
	FilterClass& getFilter(const string& name) { return filters[name]; }

protected:

	ramCompoundContainer<FilterClass> filters;
	vector<ramNodeArray> cache;

	unsigned int last_updated_time;

};