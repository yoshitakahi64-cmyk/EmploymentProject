#pragma once
#include "SceneBase.h"

// ロードシーンクラス
class LoadingScene : public SceneBase
{
public:
	LoadingScene(); // コンストラクタ	
	~LoadingScene(); // デストラクタ	

public:
	void Init() override; // 初期化関数
	void Load() override; // ロード関数
	void Start() override; // スタート関数
	void Step() override; // ステップ関数
	void Update() override; // 更新関数
	void Draw() override; // 描画関数
	void Fin() override; // 終了関数

private:
	// 画像ハンドル
	int m_lodingImageHandle;
	// ローディングアイコンのハンドル
	int m_LodingIconHandle;

	// ローディングアイコンの回転角度
	double m_IconRot;
	// ローディングアイコンのサイズ
	int m_IconHeight;
	int m_IconWidth;
};
