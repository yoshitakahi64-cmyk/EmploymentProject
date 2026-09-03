#pragma once

#include "MyResource.h"

class SoundResource : public MyResource
{
public:
	SoundResource(MyResourceManager& manager);
	void Delete();
};
