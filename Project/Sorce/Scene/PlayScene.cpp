#include "PlayScene.h"
#include "../Camera/CameraManager.h"
#include "../Camera/Camera.h"
#include "../Camera/DebugCamera.h"
#include "../Collision/CollisionManager.h"
#include "../MyEffekseer/EffekseerManager.h"
#include "../TestStage/TestStageManager.h"
#include "../Bullet/BulletManager.h"
#include "../Player/PlayerManager.h"
#include "../Enemy/EnemyManager.h"
#include "../StageObject/StageObjectManager.h"
#include "../PullObject/PullObjectManager.h"
#include "../Gimmick/GimmickManager.h"
#include "../Input/Input.h"
#include "../MyMath/MyMath.h"
#include "../GameController/GameController.h"
#include "../Target/TargetManager.h"
#include "../Stage/StageManager.h"
#include "../UI/UIManager.h"
#include "../Score/ScoreManager.h"
#include "../Sound/SoundManager.h"
#include "../Mechanism/IDManager.h"
#include "../ShadowMap/ShadowMap.h"
#include "../Resource/MyResourceManager.h"



PlayScene::PlayScene()
{
	// デバッグフラグ
	m_IsDebug= false;
	m_State = INIT;
}

PlayScene::~PlayScene()
{
	Fin();
}

void PlayScene::Init()
{
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// Collision-----
    // コリジョンマネージャー生成
	CollisionManager::CreateInstance();
	CollisionManager* collisionManager = CollisionManager::GetInstance();
	//---------------

	ScoreManager::CreateInstance();
	ScoreManager* scoreManager = ScoreManager::GetInstance();
	scoreManager->CreateScore();
	scoreManager->Init();

	ShadowMap::CreateInstance();
	ShadowMap::GetInstance()->Init();
	
	//--- TestStage ---
	// テストステージマネージャーを作成
	TestStageManager::CreateInstance();
	// テストステージマネージャーを取得
	TestStageManager* testSatgeManager = TestStageManager::GetInstance();
	// テストステージマネージャーのInit
	testSatgeManager->Init();

	//--- PullObject ---
	// プルオブジェクトマネージャーを作成
	PullObjectManager::CreateInstance();
	// プルオブジェクトマネージャーを取得
	PullObjectManager* pullObjectManager = PullObjectManager::GetInstance();
	// プルオブジェクトマネージャーのInit
	pullObjectManager->Init();


	//--- StageObject ---
	// ステージオブジェクトマネージャーを作成
	StageObjectManager::CreateInstance();
	// ステージオブジェクトマネージャーを取得
	StageObjectManager* stageObjectManager = StageObjectManager::GetInstance();
	// ステージオブジェクトマネージャーのInit
	stageObjectManager->Init();

	//--- Target ---
	// ターゲットマネージャーを作成
	TargetManager::CreateInstance();
	// ターゲットマネージャーを取得
	TargetManager* targetManager = TargetManager::GetInstance();
	// ターゲットを作成
	targetManager->CreateTarget();
	
	//--- Gimmick ---
	// ギミックマネージャーを作成
	GimmickManager::CreateInstance();
	// ギミックマネージャーを取得
	GimmickManager* gimmickManager = GimmickManager::GetInstance();
	// ギミックマネージャーのInit
	gimmickManager->Init();

	//--- Bullet ---
	// バレットマネージャーを作成
	BulletManager::CreateInstance();
	// バレットマネージャーを取得
	BulletManager* bulletManager = BulletManager::GetInstance();
	// バレットマネージャーのInit
	bulletManager->Init();

	//--- Player ---
	// プレイヤーマネージャーを作成
	PlayerManager::CreateInstance();
	// プレイヤーマネージャーを取得
	PlayerManager* playerManager = PlayerManager::GetInstance();
	// プレイヤーマネージャーのInit
	playerManager->Init();

	//--- Enemy ---
	// エネミーマネージャーを作成
	EnemyManager::CreateInstance();
	// エネミーマネージャーを取得
	EnemyManager* enemyManager = EnemyManager::GetInstance();
	// エネミーを初期化
	enemyManager->Init();

	CameraManager::CreateInstance();
	CameraManager* cameraManager = CameraManager::GetInstance();
	// カメラを作成
	cameraManager->CreateCamera();
	// カメラを初期化
	cameraManager->Init();

	// UIマネージャーを作成
	UIManager::CreateInstance();
	UIManager* uiManager = UIManager::GetInstance();
	// UIマネージャーのInit
	uiManager->Init();

	// ステージマネージャーを作成
	StageManager::CreateInstance();

	m_State = LOAD;
}

void PlayScene::Load()
{

	// エフェクシアをロード
	EffekseerManager::GetInstance()->Load();

	// テストステージをロード
	TestStageManager::GetInstance()->Load();

	PullObjectManager::GetInstance()->Load();

	// ステージオブジェクトをロード
	StageObjectManager::GetInstance()->Load();

	// ギミックをロード
	GimmickManager::GetInstance()->Load();

	// バレットをロード
	BulletManager::GetInstance()->Load();

	// プレイヤーをロード
	PlayerManager::GetInstance()->Load();

	// エネミーをロード
	EnemyManager::GetInstance()->Load();

	// UIをロード
	UIManager::GetInstance()->Load();

	// プレイシーンで使用するサウンドをロード
	SoundManager::GetInstance()->Load();

	// ステージマネージャーをロード
	StageManager::GetInstance()->Load("Data/Room/json/PlayScene.json");
	
	if (MyResourceManager::GetInstance()->IsFinishLoading())
	{
		m_State = START;
	}	
}

void PlayScene::Start()
{
	// ステージマネージャースタート
	StageManager::GetInstance()->Start();

	CameraManager::GetInstance()->SetNormal();

	// テストステージをスタート
	TestStageManager::GetInstance()->Start();

	// エフェクシアをスタート
	EffekseerManager::GetInstance()->Start();

	// プルオブジェクトをスタート
	PullObjectManager::GetInstance()->Start();

	// ステージオブジェクトをスタート
	StageObjectManager::GetInstance()->Start();

	// ギミックをスタート
	GimmickManager::GetInstance()->Start();

	// バレットをスタート
	BulletManager::GetInstance()->Start();

	// プレイヤーをスタート
	PlayerManager::GetInstance()->Start();

	// エネミーをスタート
	EnemyManager::GetInstance()->Start();

	//HPを生成
	UIManager::GetInstance()->CreateUI(UI_ID_HP,20,0);

	//スコアを生成
	UIManager::GetInstance()->CreateUI(UI_ID_SCORE, 1100, 30);

	// 操作説明を生成
	UIManager::GetInstance()->CreateUI(UI_ID_MANUAL, 1100, 130);

	UIManager::GetInstance()->Start();

	SoundManager::GetInstance()->Play_BGM(BGM_GAMEPLAY, true);

	m_State = LOOP;
}

void PlayScene::Step()
{

	// デバッグカメラがOFFの時のみそれぞれのオブジェクトを動かす
	if(!m_IsDebug)
	{		
		// エフェクシアステップ
		EffekseerManager::GetInstance()->Step();

		// テストステージステップ
		TestStageManager::GetInstance()->Step();

		// プルオブジェクトステップ
		PullObjectManager::GetInstance()->Step();

		// ステージオブジェクトステップ
		StageObjectManager::GetInstance()->Step();

		// ギミックオブジェクトステップ
		GimmickManager::GetInstance()->Step();

		// バレットステップ
		BulletManager::GetInstance()->Step();

		// プレイヤーステップ
		PlayerManager::GetInstance()->Step();

		// エネミーステップ
		EnemyManager::GetInstance()->Step();

		UIManager::GetInstance()->Step();

		// 当たり判定
		CollisionManager::GetInstance()->CheckCollision();

	}
}

void PlayScene::Update()
{

	CameraManager::GetInstance()->Update();

	// デバッグカメラがOFFの時のみそれぞれのオブジェクトを動かす
	if(!m_IsDebug)
	{	
		// エフェクシアを更新
		EffekseerManager::GetInstance()->Update();

		// テストステージを更新
		TestStageManager::GetInstance()->Update();

		// プルオブジェクトを更新
		PullObjectManager::GetInstance()->Update();

		// ステージオブジェクトを更新
		StageObjectManager::GetInstance()->Update();

		// ギミックを更新
		GimmickManager::GetInstance()->Update();

		// ターゲットを更新
		TargetManager::GetInstance()->Update();

		// バレットを更新
		BulletManager::GetInstance()->Update();

		// プレイヤーを更新
		PlayerManager::GetInstance()->Update();

		// エネミーを更新
		EnemyManager::GetInstance()->Update();

		// コリジョンマネージャーを更新
		CollisionManager::GetInstance()->Update();

		// UIを更新
		UIManager::GetInstance()->Update();	

		// シャドウマップを更新
		ShadowMap::GetInstance()->Update();
	}
}

void PlayScene::Draw()
{
	// オブジェクトの影を描画する
	ShadowMap::GetInstance()->StartDrawShadowMap();
	// ステージオブジェクトを描画
	StageObjectManager::GetInstance()->Draw();

	// ギミックを描画
	GimmickManager::GetInstance()->Draw();

	// バレットを描画
	BulletManager::GetInstance()->Draw();

	// プレイヤー描画
	PlayerManager::GetInstance()->Draw();

	// エネミー描画
	EnemyManager::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndDrawShadowMap();

	// シャドウマップに描画された影を床に移しこむ
	ShadowMap::GetInstance()->StartAppearsShadowMap();
	// テストステージ描画
	TestStageManager::GetInstance()->Draw();
	ShadowMap::GetInstance()->EndAppearsShadowMap();

	// プルオブジェクトを描画
	PullObjectManager::GetInstance()->Draw();

	// ステージオブジェクトを描画
	StageObjectManager::GetInstance()->Draw();

	// ギミックを描画
	GimmickManager::GetInstance()->Draw();

	// バレットを描画
	BulletManager::GetInstance()->Draw();

	// プレイヤー描画
	PlayerManager::GetInstance()->Draw();

	// エネミー描画
	EnemyManager::GetInstance()->Draw();

	// エフェクトの描画
	EffekseerManager::GetInstance()->Draw();

	// UIの描画
	UIManager::GetInstance()->Draw();
}

void PlayScene::Fin()
{
	// テストステージ削除
	TestStageManager::DeleteInstance();

	// プルオブジェクト削除
	PullObjectManager::DeleteInstance();

	// ステージオブジェクト削除
	StageObjectManager::DeleteInstance();

	// ギミックを削除
	GimmickManager::DeleteInstance();

	// バレットマネージャーを削除
	BulletManager::DeleteInstance();

	// ターゲットマネージャーを削除
	TargetManager::DeleteInstance();

	// プレイヤー削除
	PlayerManager::DeleteInstance();

	// エネミー削除
	EnemyManager::DeleteInstance();

	// UI削除
	UIManager::DeleteInstance();

	// コリジョンマネージャー削除
	CollisionManager::DeleteInstance();

	// シャドウマップ削除
	ShadowMap::DeleteInstance();

	// ID削除
	IDManager::GetInstance()->Clear();
}
