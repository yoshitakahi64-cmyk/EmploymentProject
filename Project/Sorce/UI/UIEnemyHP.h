#pragma once

#include"UIBase3D.h"
#include <functional>

// 敵のHPデータ構造体
struct EnemyHPData
{
	float value;
	float maxValue;
};

// 敵のHPを表示するUIクラス
class UIEnemyHP : public UIBase3D
{
public:
	UIEnemyHP(); // コンストラクタ
	~UIEnemyHP(); // デストラクタ

public:
	void Init()override; // 初期化関数
	void Load()override; // ロード関数
	void Start()override; // スタート関数
	void Update()override; // 更新関数
	void Draw()override; // 描画関数
	void Fin()override; // 終了関数

	// クローン関数
	UniquePtr<UIBase3D> Clone()override;

public:
	// 敵のHPデータを提供する関数を設定する関数
	void SetEnemyHPProvider(std::function<EnemyHPData()> provider) { m_EnemyHPProvider = provider; }
	// 敵の操作状態を提供する関数を設定する関数
	void SetEnemyControlStateProvider(std::function<bool()> provider) { m_EnemyControlStateProvider = provider; }

private:
	// 敵のHPデータを提供する関数
	std::function<EnemyHPData()> m_EnemyHPProvider;
	// 敵の操作状態を提供する関数
	std::function<bool()> m_EnemyControlStateProvider;

	// HPUIの描画に使用するハンドル
	int m_HPCursor_GHandle;
	int m_HPCursor_RHandle;
	int m_HPCursor_YHandle;
	// YボタンとQキーのハンドル（操作状態の表示に使用）
	int m_YButtonHandle;
	int m_QKeyHandle;
	// 敵のHPの現在値と最大値
	float m_Value;
	float m_MaxValue;
};
