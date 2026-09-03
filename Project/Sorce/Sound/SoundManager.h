#pragma once

#include "../Singleton.h"
#include "../Memory.h"

// BGMのID
enum BGM_ID
{
	BGM_TITLE,
	BGM_GAMEPLAY,
	BGM_CLEAR,
	BGM_GAMEOVER,
	BGM_MAX,
	BGM_ID_NONE = -1,
};

// SEのID
enum SE_ID
{
	SE_START_JINGLE,
	SE_JUMP,
	SE_GRAVITYORB_SHOT,
	SE_GRAVITYORB_HIT,
	SE_CONTROLOBJECT_HIT,
	SE_ENEMY_ATTACK,
	SE_ENEMY_BUMP,
	SE_ENEMY_CHARGE,
	SE_ENEMY_SHOT,
	SE_ENEMY_SHOT_HIT,
	SE_ENEMY_DOWN,
	SE_BUTTON_PUSH,
	SE_TARGET,
	SE_ID_MAX,
	SE_ID_NONE = -1,
};

// サウンド管理クラス
class SoundManager : public Singleton<SoundManager>
{
public:
	SoundManager(); // コンストラクタ
	~SoundManager(); // デストラクタ

public:
	void Init(); // 初期化関数
	void Load(); // ロード関数

public:
	// BGM再生関数
	void Play_BGM(BGM_ID id, bool loop);
	// SE再生関数
	void Play_SE(SE_ID id, bool loop);
	// BGM停止関数
	void Stop_BGM(BGM_ID id);
	// BGM音量設定関数
	void SetBGMVolume();
	// SE音量設定関数
	void SetSEVolume();
	// BGM音量変更関数
	void ChangeBGMVolume(int volume);
	// SE音量変更関数
	void ChangeSEVolume(int volume);

private:
	// BGMとSEのハンドルを格納する配列
	int m_BGM[BGM_MAX];
	int m_SE[SE_ID_MAX];

	// BGMとSEの音量を格納する変数
	int m_BGMVolume;
	int m_SEVolume;
};
