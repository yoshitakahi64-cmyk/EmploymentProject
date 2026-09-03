#pragma once

#include "../Gimmick/Gimmick.h"

// ドアクラス
class Door : public Gimmick
{
public:
	Door(); // コンストラクタ
	~Door() = default; // デストラクタ

public :
	void OnStart()override; // 継承先スタート関数
	void Load()override; // ロード関数

	// クローン関数
	UniquePtr<Gimmick> Clone()override;
	// ターゲットIDのセッター関数
	void SetTargetID(std::string id) { m_TargetID = id; }

	// ゲッター・セッター関数
	std::string GetTypeName()const override { return "Door"; }

public:
	// ドアを開ける関数と閉める関数
	void Open();
	void Close();

private:
	// 空いているドアのハンドル
	int m_OpenDoorHandle;
	// 対象のID
	std::string m_TargetID;
	// ドアが空いているかどうかのフラグ
	bool m_IsOpen;
};
