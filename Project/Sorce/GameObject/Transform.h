#pragma once
#include <DxLib.h>
#include "../Quatrenion/Quatrenion.h"

// position_3D,posX_2D,posY_2D,rotation,scaleを持った構造体
struct Transform
{
	VECTOR position_3D;
	Quaternion rotation;
	VECTOR scale;
	int posX_2D;
	int posY_2D;
};
