#include "CameraManager.h"
#include "Camera.h"

CameraManager::CameraManager():
	m_UseNormalCamera(false),
	m_UseShotCamera(false),
	m_UseTargetCamera(false)
{
}

CameraManager::~CameraManager()
{

}

// 通常カメラに切り替える関数
void CameraManager::SetNormal()
{
	m_Camera->ChangeState(&m_NormalCamera);
	ResetUseCameraFlags();
	m_UseNormalCamera = true;
}

// ショットカメラに切り替える関数
void CameraManager::SetShot()
{
	m_Camera->ChangeState(&m_ShotCamera);
	ResetUseCameraFlags();
	m_UseShotCamera = true;
}

// ターゲットカメラに切り替える関数
void CameraManager::SetTarget()
{
	m_Camera->ChangeState(&m_TargetCamera);
	ResetUseCameraFlags();
	m_UseTargetCamera = true;
}

// デバッグカメラに切り替える関数
void CameraManager::SetDebug()
{
	m_Camera->ChangeState(&m_DebugCamera);
}

void CameraManager::Init()
{
	if (m_Camera)
	{
		m_Camera->Init();
	}
}

void CameraManager::Update()
{
	m_Camera->Update();
}

void CameraManager::Draw()
{
	m_Camera->Draw();
}

void CameraManager::CreateCamera()
{
	m_Camera = MakeUnique<Camera>();
}
