#pragma once

#include "MyResource.h"

class ImageResource : public MyResource
{
public:
	ImageResource(MyResourceManager& manager);
	void Delete();
};
