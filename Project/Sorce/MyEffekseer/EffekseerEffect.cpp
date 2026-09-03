#include "EffekseerForDXLib.h"
#include "EffekseerEffect.h"
#include "../Target/TargetManager.h"

EffekseerEffect::EffekseerEffect()
{
	m_Active = false;
	m_Handle = 0;
	m_Pos = VGet(0.0f, 0.0f, 0.0f);
}

EffekseerEffect::~EffekseerEffect()
{
	Fin();
}

void EffekseerEffect::Step()
{
	// 再生中かどうか
	if (IsEffekseer3DEffectPlaying(m_Handle) != 0)
	{
		m_Active = false;
	}
}

void EffekseerEffect::Update()
{
	if (!m_Active) return;

	// 位置を追跡したい場合
	if (m_IsTracking)
	{
		// 追跡対象が生存していなければ
		if (m_IsTrackTargetActive && !(*m_IsTrackTargetActive))
		{
			Stop();
			return;
		}
		// 追跡対象の座標があれば
		if (m_TrackTarget)
		{
			m_Pos = *m_TrackTarget;
		}	
	}

	// 位置設定
	SetPosPlayingEffekseer3DEffect(m_Handle, m_Pos.x, m_Pos.y, m_Pos.z);
}

void EffekseerEffect::Fin()
{
}

void EffekseerEffect::Play(int handle)
{
	m_Handle = PlayEffekseer3DEffect(handle);
}

void EffekseerEffect::Stop()
{
	StopEffekseer3DEffect(m_Handle);
	m_Active = false;
}

void EffekseerEffect::SetTracking(VECTOR* target,bool* isActive)
{
	m_TrackTarget = target;
	m_IsTrackTargetActive = isActive;
	
	if (target != nullptr)
	{
		m_IsTracking = true;
	}
	else
	{
		m_IsTracking = false;
	}
}

void EffekseerEffect::StopTracking()
{
	m_IsTracking = false;
	m_TrackTarget = nullptr;
}
