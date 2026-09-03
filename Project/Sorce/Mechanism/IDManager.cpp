#include "IDManager.h"
#include "../GameObject/3DGameObject.h"
#include <format>

void IDManager::Register(GameObject3D* mech)
{
	// オブジェクトのタイプ名を取得してIDを生成
	std::string typeName = mech->GetTypeName();
	// IDをオブジェクトにセットして管理コンテナに登録
	std::string id = GenetrateID(typeName);
	// オブジェクトにIDをセット
	mech->SetID(id);
	// IDとオブジェクトのペアを管理コンテナに登録
	m_3DGameObject[id] = mech;
}


GameObject3D* IDManager::FindByID(const std::string& id)
{
	// IDをキーにして管理コンテナからオブジェクトを検索
	auto it = m_3DGameObject.find(id);
	// 見つかった場合はオブジェクトを返し、見つからなかった場合はnullptrを返す
	return (it!= m_3DGameObject.end()) ? it->second : nullptr;
}


std::string IDManager::GenetrateID(const std::string& typeName)
{
	// タイプ名をキーにしてIDカウンターを管理コンテナから取得
	int& counter = m_IDCounters[typeName];
	counter++;
	// タイプ名とカウンターを組み合わせてIDを生成
	return std::format("{}_{:03}",typeName,counter);
}

void IDManager::Clear()
{
	m_3DGameObject.clear();
	m_IDCounters.clear();
}
