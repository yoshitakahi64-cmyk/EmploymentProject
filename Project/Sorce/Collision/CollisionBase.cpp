#include "CollisionBase.h"

CollisionBase::CollisionBase() :
	m_TargetPos(nullptr),
	m_LocalPos(VGet(0, 0, 0)),
	m_IsActive(true)
{
}
