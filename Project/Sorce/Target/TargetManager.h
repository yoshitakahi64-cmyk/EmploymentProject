#pragma once

#include "../Singleton.h"
#include "../Memory.h"
#include "../Container.h"


class Target;

// ターゲット管理クラス
class TargetManager : public Singleton<TargetManager>
{
public:
	TargetManager(); // コンストラクタ
	~TargetManager() = default; // デストラクタ

public:
	void Update(); // 更新関数

public:
	// ターゲット生成関数
	void CreateTarget();
	// ターゲット取得関数
	const UniquePtr<class Target>& GetTarget() const { return m_Target; }

private:
	// ターゲットのインスタンス
	UniquePtr<Target> m_Target;
};