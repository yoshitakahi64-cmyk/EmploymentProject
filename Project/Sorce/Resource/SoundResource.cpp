#include "SoundResource.h"
#include <DxLib.h>

SoundResource::SoundResource(MyResourceManager& manager):MyResource(manager)
{
}

void SoundResource::Delete()
{
	DeleteSoundMem(m_Handle);
}
