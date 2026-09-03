#pragma once

#include <unordered_map>
#include <string>
#include "../Singleton.h"

class GameObject3D;

// ID管理クラス
class IDManager : public Singleton<IDManager>
{
public:
	// ID登録
	void Register(GameObject3D*);
	// ID検索
	GameObject3D* FindByID(const std::string&);
	// 自動ID生成
	std::string GenetrateID(const std::string& typeName);
	// ID削除
	void Clear();

private:
	// IDとGameObject3Dのコンテナ
	std::unordered_map<std::string, GameObject3D*> m_3DGameObject;
	std::unordered_map<std::string, int>m_IDCounters;
};
