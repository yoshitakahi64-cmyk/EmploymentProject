#include "GameObject.h"

GameObject::GameObject():
	m_Transform(),
	m_Handle(-1),
	m_IsFall(false)
{
}

void GameObject::Init()
{
	m_Transform.position_3D = {};
	m_Transform.rotation = {};
	m_Transform.scale = {};
	m_Transform.posX_2D = 0;
	m_Transform.posY_2D = 0;
	m_Handle = -1;
}
