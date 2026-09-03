#include "LoadingScene.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"

LoadingScene::LoadingScene():
	m_IconHeight(0),
	m_IconWidth(0),
	m_IconRot(0.0),
	m_LodingIconHandle(0),
	m_lodingImageHandle(false)
{
	m_State = INIT;
}

LoadingScene::~LoadingScene()
{

}

void LoadingScene::Init()
{
	// ステートをロードに設定
	m_State = LOAD;
	m_IconRot = 0.0;
}

void LoadingScene::Load()
{
	// ロード画面の画像をロード
	m_lodingImageHandle = MyResourceManager::GetInstance()->LoadImageResource("Data/Loading/LoadingImage.png",true,false)->GetHandle();

	// ローディングアイコンの画像をロード
	m_LodingIconHandle = MyResourceManager::GetInstance()->LoadImageResource("Data/Loading/LoadingIcon.png", true, false)->GetHandle();

	// ローディングアイコンのサイズを取得
	GetGraphSize(m_LodingIconHandle, &m_IconWidth, &m_IconHeight);
	// ステートをスタートに設定
	m_State = START;
}

void LoadingScene::Start()
{
	// ステートをループに設定
	m_State = LOOP;
}

void LoadingScene::Step()
{
	// ローディングアイコンの回転角度を更新
	m_IconRot += DX_PI / 90;
}

void LoadingScene::Update()
{
}

void LoadingScene::Draw()
{
	// ローディングアイコンを回転させて描画
	DrawRotaGraph(700, 820, 0.5, m_IconRot, m_LodingIconHandle, true);

	// ロード画面を描画
	DrawGraph(0, 0, m_lodingImageHandle, true);

}

void LoadingScene::Fin()
{
}
