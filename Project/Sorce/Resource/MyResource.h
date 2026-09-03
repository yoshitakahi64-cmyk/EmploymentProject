#pragma once

#include<string>

class MyResourceManager;

// リソース管理システムの基底クラス
class MyResource
{
	friend MyResourceManager;

public:
	MyResource(MyResourceManager& manager);
	~MyResource();
	virtual void Delete() = 0;
	int GetHandle()const;

protected:
	MyResourceManager& m_Manager;
	int m_Handle = 0;
	int m_Count = 0;
	std::string m_Path = "";
	bool m_IsEternal = false; // 常駐フラグ
	bool m_IsAsync = false;
	bool m_IsFinishHandle = false;
	bool m_IsDeleted = false;

};
