#include "MyResourceManager.h"
#include "ImageResource.h"
#include "SoundResource.h"
#include "ModelResource.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneBase.h"
#include <DxLib.h>
#include <cassert>
#include <list>

MyResourceManager* MyResourceManager::m_Instance = nullptr;



MyResourceManager::MyResourceManager()
{
    m_LodingScene = nullptr;
    m_sceneManager = nullptr;
}

MyResourceManager::~MyResourceManager()
{
    Fin();
}

void MyResourceManager::Init()
{
    m_sceneManager = SceneManager::GetInstance();
}

// 画像ロード関数
std::shared_ptr<MyResource> MyResourceManager::LoadImageResource(std::string path, bool isEternal, bool isAsync)
{
    auto it = m_ResourceTable.find(path);
    if (it == m_ResourceTable.end())
    {
        // もし初回のロードだったらLoadGraphでロードする 
        // もしisAsyncがtrueなら非同期読み込みを行う
        SetUseASyncLoadFlag(isAsync);
        auto handle = LoadGraph(path.c_str());
        assert(handle > 0);
        if (handle <= 0)
        {
            return nullptr;
        }
        m_ResourceTable[path] = std::make_shared<ImageResource>(*this);
        m_ResourceTable[path]->m_Handle = handle;
        m_ResourceTable[path]->m_IsFinishHandle = false;
        m_ResourceTable[path]->m_IsAsync = isAsync;

        if (isAsync)
        {
            m_LodingScene = m_sceneManager->StartLoadingScene();
            return nullptr;
        }

    }
    else
    {
        if (!isAsync || !CheckHandleASyncLoad(m_ResourceTable[path]->m_Handle))
        {
            m_ResourceTable[path]->m_Path = path;
            m_ResourceTable[path]->m_IsEternal = isEternal;
            m_ResourceTable[path]->m_IsFinishHandle = true;

        }
        else
        {
            return nullptr;
        }
    }

    return  std::dynamic_pointer_cast<ImageResource>(m_ResourceTable[path]);
}

// サウンドロード関数
std::shared_ptr<MyResource> MyResourceManager::LoadSoundResource(std::string path, bool isEternal, bool isAsync)
{
    auto it = m_ResourceTable.find(path);
    if (it == m_ResourceTable.end())
    {
        // もし初回のロードだったらLoadGraphでロードする 
        // もしisAsyncがtrueなら非同期読み込みを行う
        SetUseASyncLoadFlag(isAsync);
        auto handle = LoadSoundMem(path.c_str());
        assert(handle > 0);
        if (handle <= 0)
        {
            return nullptr;
        }
        m_ResourceTable[path] = std::make_shared<SoundResource>(*this);
        m_ResourceTable[path]->m_Handle = handle;
        m_ResourceTable[path]->m_IsFinishHandle = false;
        m_ResourceTable[path]->m_IsAsync = isAsync;

        if (isAsync)
        {
            m_LodingScene = m_sceneManager->StartLoadingScene();
            return nullptr;
        }

    }
    else
    {
        if (!isAsync || !CheckHandleASyncLoad(m_ResourceTable[path]->m_Handle))
        {
            m_ResourceTable[path]->m_Path = path;
            m_ResourceTable[path]->m_IsEternal = isEternal;
            m_ResourceTable[path]->m_IsFinishHandle = true;

        }
        else
        {
            return nullptr;
        }
    }

    return  std::dynamic_pointer_cast<SoundResource>(m_ResourceTable[path]);
}

// 3Dモデルロード関数
std::shared_ptr<MyResource> MyResourceManager::LoadModelResource(std::string path, bool isEternal , bool isAsync)
{
    auto it = m_ResourceTable.find(path);
    if (it == m_ResourceTable.end())
    {
        // もし初回のロードだったらLoadGraphでロードする 
        // もしisAsyncがtrueなら非同期読み込みを行う
        SetUseASyncLoadFlag(isAsync);
        auto handle = MV1LoadModel(path.c_str());
        assert(handle > 0);
        if (handle <= 0)
        {
            return nullptr;
        }
        m_ResourceTable[path] = std::make_shared<ModelResource>(*this);
        m_ResourceTable[path]->m_Handle = handle;
        m_ResourceTable[path]->m_IsFinishHandle = false;
        m_ResourceTable[path]->m_IsAsync = isAsync;
     
        if (isAsync)
        {          
             m_LodingScene = m_sceneManager->StartLoadingScene();  
             return nullptr;
        }
      
    }
    else
    {       
        if (!isAsync || !CheckHandleASyncLoad(m_ResourceTable[path]->m_Handle))
        {            
            m_ResourceTable[path]->m_Path = path;
            m_ResourceTable[path]->m_IsEternal = isEternal;
            m_ResourceTable[path]->m_IsFinishHandle = true;
                  
        }
        else
        {
            return nullptr;
        }
    }
    
    return  std::dynamic_pointer_cast<ModelResource>(m_ResourceTable[path]);

}

void MyResourceManager::Delete(const std::string& path)
{
    auto it = m_ResourceTable.find(path);
    if (it == m_ResourceTable.end())
    {
        return;
    }
    auto& myResource = it->second;
    if (myResource->m_IsEternal)
    {
        return;
    }

    if (myResource.use_count() == 0)
    {
        myResource->Delete();
    }

}

bool MyResourceManager::IsFinishLoading()
{
    for (auto& myResource : m_ResourceTable)
    {
        if(myResource.second->m_IsAsync)
        {
            if (!myResource.second->m_IsFinishHandle)
            {
                return false;
            }
        }       
    }

    m_sceneManager->StopLoadingScene(m_LodingScene);
    return true;
}

void MyResourceManager::Fin()
{
    for (auto& myResource : m_ResourceTable)
    {
        myResource.second->Delete();
    }
    m_ResourceTable.clear();
}
