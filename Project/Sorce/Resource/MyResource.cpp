#include "MyResource.h"
#include "MyResourceManager.h"

MyResource::MyResource(MyResourceManager& manager):m_Manager(manager)
{

}

MyResource::~MyResource()
{

}

int MyResource::GetHandle()const
{
	return m_Handle;
}
