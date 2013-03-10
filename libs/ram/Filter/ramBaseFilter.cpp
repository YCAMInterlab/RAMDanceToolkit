#include "ramBaseFilter.h"

const ramNodeArray& ramBaseFilter::operator()(const ramNodeArray& src)
{
	return update(src);
}

const ramNodeArray& ramBaseFilter::update(const ramNodeArray& src)
{
	if (!isEnabled()) return src;
	
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

const ramNodeArray& ramBaseFilter::get(size_t index) const { return cache; }
size_t ramBaseFilter::getSize() const { return 1; }

//
string ramBusSend::getName() const { return "ramBusSend"; }

ramBusSend::ramBusSend() {}
ramBusSend::ramBusSend(const string &bus_name) { setBusName(bus_name); }

void ramBusSend::setBusName(const string &bus_name) { this->bus_name = bus_name; }
const string& ramBusSend::getBusName() const { return bus_name; }

const ramNodeArray& ramBusSend::get(size_t index) const { static ramNodeArray arr; return arr; }
size_t ramBusSend::getSize() const { return 1; }

const ramNodeArray& ramBusSend::filter(const ramNodeArray& src)
{
	ramActorManager::instance().setBus(bus_name, src);
	return src;
}

