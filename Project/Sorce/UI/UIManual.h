#pragma once

#include "UIBase.h"

class Player;

// 操作説明UIクラス
class UIManual : public UIBase
{
public:
	UIManual(); // コンストラクタ	
	~UIManual(); // デストラクタ

public:
	void Init()override; // 初期化関数
	void Load()override; // ロード関数
	void Start()override; // スタート関数
	void Step()override; //	ステップ関数
	void Update()override; // 更新関数
	void Draw()override; // 描画関数
	void Fin()override; // 終了関数

	// クローン関数
	UniquePtr<UIBase> Clone()override;

private:
	// プレイヤーのインスタンス
	Player* m_Player;

	// 各操作キー・ボタンのハンドル
	int m_Handle_AButton;
	int m_Handle_BButton;
	int m_Handle_XButton;
	int m_Handle_YButton;
	int m_Handle_R2Button;
	int m_Handle_L2Button;
	int m_Handle_StickButton_R;
	int m_Handle_StickButton_L;
	int m_Handle_ArrowKey;
	int m_Handle_SpaceKey;
	int m_Handle_QKey;
	int m_Handle_EKey;
	int m_Handle_FKey;
	int m_Handle_VKey;
	int m_Handle_CKey;
	int m_Handle_XKey;
	int m_Handle_ZKey;
};
