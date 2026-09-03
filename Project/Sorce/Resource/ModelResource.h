#pragma once

#include "MyResource.h"

class ModelResource : public MyResource
{
public:
	ModelResource(MyResourceManager& manager);
	void Delete();
};
