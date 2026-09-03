#include "DxLib.h"
#include "ShadowMap.h"
#include "../Player/PlayerManager.h"
#include "../Player/Player.h"
#include "../MyMath/MyMath.h"

const constexpr int SHADOW_MAP_SIZE_X = 4096;
const constexpr int SHADOW_MAP_SIZE_Y = 4096;

ShadowMap* ShadowMap::m_Instance = nullptr;

ShadowMap::ShadowMap()
{
	m_Handle = 0;
}

ShadowMap::~ShadowMap()
{
	Fin();
}

void ShadowMap::Init()
{
	// シャドウマップを作成
	m_Handle = MakeShadowMap(SHADOW_MAP_SIZE_X, SHADOW_MAP_SIZE_X);

	// ライトの向きを取得
	VECTOR lightDirection = GetLightDirection();

	// 影が出るライトの向きを設定
	SetShadowMapLightDirection(m_Handle, lightDirection);
}

void ShadowMap::Update()
{
	Player* player = PlayerManager::GetInstance()->GetPlayer().get();

	VECTOR min = MyMath::VecAdd(player->GetPos3D(), VGet(-40.0f, 0.0f, -40.0f));
	VECTOR max = MyMath::VecAdd(player->GetPos3D(), VGet(40.0f, 0.0f, 40.0f));

	min.y = 0.0f;
	max.y = 1.0f;

	SetShadowMapDrawArea(m_Handle, min, max);
}

void ShadowMap::Fin()
{
	DeleteShadowMap(m_Handle);
}

void ShadowMap::StartDrawShadowMap()
{
	// シャドウマップへの描画を開始する
	ShadowMap_DrawSetup(m_Handle);
}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::StartAppearsShadowMap()
{
	// シャドウマップの映しこみを開始する
	SetUseShadowMap(0, m_Handle);
}

void ShadowMap::EndAppearsShadowMap()
{
	// これ以上映すモデルがない場合は-1を渡して終了する
	SetUseShadowMap(0, -1);
}
