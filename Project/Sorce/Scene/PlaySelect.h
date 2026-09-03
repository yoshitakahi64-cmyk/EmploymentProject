#pragma once
#include "SceneBase.h"

// プレイ選択シーン
class PlaySelectScene : public SceneBase
{
public:
	PlaySelectScene();
	~PlaySelectScene();

public:
	void Init() override;
	void Load() override;
	void Start() override;
	void Step() override;
	void Update() override;
	void Draw() override;
	void Fin() override;

};
