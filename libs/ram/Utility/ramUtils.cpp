#include "ramUtils.h"


string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}
