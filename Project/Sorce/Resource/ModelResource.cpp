#include "ModelResource.h"
#include <DxLib.h>

ModelResource::ModelResource(MyResourceManager& manager):MyResource(manager)
{
}

void ModelResource::Delete()
{
	MV1DeleteModel(m_Handle);
}
