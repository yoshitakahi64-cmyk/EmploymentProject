#pragma once
#include "UIBase.h"

class Player;

// HPゲージクラス
class UIHP : public UIBase
{
public:
	UIHP(); // コンストラクタ
	~UIHP(); // デストラクタ

public:
	void Init()override; // 初期化関数
	void Load()override; // ロード関数
	void Start()override; // スタート関数
	void Update()override; // 更新関数
	void Draw()override; // 描画関数
	void Fin()override; // 終了関数

	// クローン関数
	UniquePtr<UIBase> Clone()override;

private:
	// HPゲージの幅と高さ
	int m_GaugeWidth;
	int m_GaugeHeight;
	// HPの現在値と最大値
	int m_Value;
	int m_MaxValue;

	// HPゲージの画像ハンドル
	Player* m_Player;
};
