#pragma once

#include "DxLib.h"
#include <vector>

class EffekseerEffect;

class EffekseerManager
{
public:
	EffekseerManager();
	~EffekseerManager();

public:
	static void CreateInstence() { if (!m_Instance) m_Instance = new EffekseerManager; }
	static EffekseerManager* GetInstance() { return m_Instance; }
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	// Effekseerを使うのに必要なセットアップ処理
	void Setup();

	// Effekseerを使う前に必要な初期化処理
	bool Init();

	// Effekseerのエフェクトデータをロードする
	void Load();

	// Effekseer使用開始処理
	void Start();

	// エフェクト再生関数
	EffekseerEffect* PlayEffect(int type, VECTOR pos);
	// 追跡するエフェクト再生
	EffekseerEffect* PlayEffectTracking(int type, VECTOR* posPtr,bool* activeFlag);

	// エフェクトの生成取得関数
	EffekseerEffect* GetOrCreateEffect();

	// ステップ処理
	void Step();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// Effekseer終了処理
	void Fin();

private:
	static EffekseerManager* m_Instance;	// シングルトン用インスタンス

	std::vector<int> m_EffectHandles;		// リソースハンドル配列
	std::vector<EffekseerEffect*> m_Effects;	// エフェクト配列
};
