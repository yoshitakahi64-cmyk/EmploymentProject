#include "ImageResource.h"
#include <DxLib.h>

ImageResource::ImageResource(MyResourceManager& manager) : MyResource(manager)
{
}

void ImageResource::Delete()
{
	DeleteGraph(m_Handle);
}
