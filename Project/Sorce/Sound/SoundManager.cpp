#include "DxLib.h"
#include "SoundManager.h"
#include "../Resource/MyResourceManager.h"
#include "../Resource/MyResource.h"
#include "../Scene/SceneManager.h"


SoundManager::SoundManager()
{
	m_BGMVolume = 0;
	m_SEVolume = 0;
	for (int i = 0; i < BGM_MAX; i++)
	{
		m_BGM[i] = -1;
	}
	for (int i = 0; i < SE_ID_MAX; i++)
	{
		m_SE[i] = -1;
	}
}

SoundManager::~SoundManager()
{

}

void SoundManager::Init()
{
	m_BGMVolume = 10;
	m_SEVolume = 30;	
}

void SoundManager::Load()
{
	if (SceneManager::GetInstance()->GetCurrentScene() == TITLE)
	{
		// タイトルBGMのロード
		SharedPtr<MyResource> titleBGM = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/TitleBGM.mp3", true, false);
		if (titleBGM)
		{
			m_BGM[BGM_TITLE] = titleBGM->GetHandle();
		}
		// スタートジングルのロード
		SharedPtr<MyResource> startJingle = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/StartJingl.mp3", true, false);
		if (startJingle)
		{
			m_SE[SE_START_JINGLE] = startJingle->GetHandle();
		}
	}
	else if (SceneManager::GetInstance()->GetCurrentScene() == PLAY)
	{
		// ゲームプレイBGMのロード
		SharedPtr<MyResource> gameplayBGM = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/PlayBGM.mp3", true, true);
		if (gameplayBGM)
		{
			m_BGM[BGM_GAMEPLAY] = gameplayBGM->GetHandle();
		}
		// クリアBGMのロード
		SharedPtr<MyResource> clearBGM = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/ClearBGM.mp3", true, true);
		if (clearBGM)
		{
			m_BGM[BGM_CLEAR] = clearBGM->GetHandle();
		}
		// ゲームオーバーBGMのロード
		SharedPtr<MyResource> gameOverBGM = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/GameOverBGM.mp3", true, true);
		if (gameOverBGM)
		{
			m_BGM[BGM_GAMEOVER] = gameOverBGM->GetHandle();
		}
		// ジャンプSEのロード
		SharedPtr<MyResource> jumpSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/Jump.mp3", true, true);
		if (jumpSE)
		{
			int copyHandle = DuplicateSoundMem(jumpSE->GetHandle());
			m_SE[SE_JUMP] = copyHandle;
		}
		// グラビティオーブの発射SEのロード
		SharedPtr<MyResource> gravityOrbShootSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/ShotSE_2.mp3", true, true);
		if (gravityOrbShootSE)
		{
			int copyHandle = DuplicateSoundMem(gravityOrbShootSE->GetHandle());
			m_SE[SE_GRAVITYORB_SHOT] = copyHandle;
		}
		// グラビティオーブのヒットSEのロード
		SharedPtr<MyResource> gravityOrbHitSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/HitSE_1.mp3", true, true);
		if (gravityOrbHitSE)
		{
			int copyHandle = DuplicateSoundMem(gravityOrbHitSE->GetHandle());
			m_SE[SE_GRAVITYORB_HIT] = copyHandle;
		}
		// コントロールオブジェクトのヒットSEのロード
		SharedPtr<MyResource> controlObjectHitSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/HitSE_4.mp3", true, true);
		if (controlObjectHitSE)
		{
			int copyHandle = DuplicateSoundMem(controlObjectHitSE->GetHandle());
			m_SE[SE_CONTROLOBJECT_HIT] = copyHandle;
		}
		// エネミー攻撃SEのロード
		SharedPtr<MyResource> enemyAttackSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/HitSE_6.mp3", true, true);
		if (enemyAttackSE)
		{
			int copyHandle = DuplicateSoundMem(enemyAttackSE->GetHandle());
			m_SE[SE_ENEMY_ATTACK] = copyHandle;
		}
		// エネミーバンプSEのロード
		SharedPtr<MyResource> enemyBumpSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/HitSE_2.mp3", true, true);
		if (enemyBumpSE)
		{
			int copyHandle = DuplicateSoundMem(enemyBumpSE->GetHandle());
			m_SE[SE_ENEMY_BUMP] = copyHandle;
		}		 
		// エネミーチャージSEのロード
		SharedPtr<MyResource> enemyChargeSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/EnemyCharge.mp3", true, true);
		if (enemyChargeSE)
		{
			int copyHandle = DuplicateSoundMem(enemyChargeSE->GetHandle());
			m_SE[SE_ENEMY_CHARGE] = copyHandle;
		}
		// エネミー射撃SEのロード
		SharedPtr<MyResource> enemyShotSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/ShotSE_1.mp3", true, true);
		if (enemyShotSE)
		{
			int copyHandle = DuplicateSoundMem(enemyShotSE->GetHandle());
			m_SE[SE_ENEMY_SHOT] = copyHandle;
		}
		// エネミー射撃ヒットSEのロード
		SharedPtr<MyResource> enemyShotHitSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/HitSE_5.mp3", true, true);
		if (enemyShotHitSE)
		{
			int copyHandle = DuplicateSoundMem(enemyShotHitSE->GetHandle());
			m_SE[SE_ENEMY_SHOT_HIT] = copyHandle;
		}
		// エネミーダウンSEのロード
		SharedPtr<MyResource> enemyDownSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/EnemyDie.mp3", true, true);
		if (enemyDownSE)
		{
			int copyHandle = DuplicateSoundMem(enemyDownSE->GetHandle());
			m_SE[SE_ENEMY_DOWN] = copyHandle;
		}
		// ボタンプッシュSEのロード
		SharedPtr<MyResource> buttonPushSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/ButtonPush.mp3", true, true);
		if (buttonPushSE)
		{
			int copyHandle = DuplicateSoundMem(buttonPushSE->GetHandle());
			m_SE[SE_BUTTON_PUSH] = copyHandle;
		}
		// ターゲット中SEのロード
		SharedPtr<MyResource> targetSE = MyResourceManager::GetInstance()->LoadSoundResource("Data/Sound/TargetSE.mp3", true, true);
		if (targetSE)
		{
			int copyHandle = DuplicateSoundMem(targetSE->GetHandle());
			m_SE[SE_TARGET] = copyHandle;
		}
	}
}

void SoundManager::Play_BGM(BGM_ID id, bool loop)
{
	if (m_BGM[id] != -1)
	{
		PlaySoundMem(m_BGM[id], loop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
	}
}

void SoundManager::Play_SE(SE_ID id, bool loop)
{
	if (m_SE[id] != -1)
	{
		PlaySoundMem(m_SE[id], DX_PLAYTYPE_BACK);
	}
}

void SoundManager::ChangeBGMVolume(int volume)
{
	m_BGMVolume = volume;
	SetBGMVolume();
}

void SoundManager::ChangeSEVolume(int volume)
{
	m_SEVolume = volume;
	SetSEVolume();
}

void SoundManager::Stop_BGM(BGM_ID id)
{
	StopSoundMem(m_BGM[id]);
}

void SoundManager::SetBGMVolume()
{
	for (int i = 0; i < BGM_MAX; i++)
	{
		if (m_BGM[i] != -1)
		{
			SetVolumeSoundMem(m_BGMVolume, m_BGM[i]);
		}
	}
}

void SoundManager::SetSEVolume()
{
	for (int i = 0; i < SE_ID_MAX; i++)
	{
		if (m_SE[i] != -1)
		{
			SetVolumeSoundMem(m_SEVolume, m_SE[i]);
		}
	}
}