#include "ramControlPanel.h"

ramControlPanel* ramControlPanel::_instance = NULL;

ramControlPanel& ramControlPanel::instance()
{
	if (_instance == NULL)
	{
		_instance = new ramControlPanel;
	}
	return *_instance;
}