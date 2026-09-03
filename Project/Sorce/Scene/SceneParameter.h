#pragma once

// シーンの状態
enum SceneState
{
	INIT,
	LOAD,
	START,
	LOOP,
	FIN,
	SCENE_STATE_MAX,
	SCENE_STATE_NONE = -1
};

// シーンのタイプ
enum SceneType
{
	TITLE,
	PLAY,
	CLEAR,
	GAMEOVER,
    PLAYSELECT,
    LOADING,
	SCENE_TYPE_NONE = -1
};
