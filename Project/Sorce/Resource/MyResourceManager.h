#pragma once

#include<memory>
#include<string>
#include<unordered_map>

class MyResource;
class SceneManager;
class SceneBase;

class MyResourceManager
{
public:
	MyResourceManager(); // コンストラクタ
	~MyResourceManager(); // デストラクタ
	void Init();
	void Fin();

public:
	static void CreateInstance() { if (!m_Instance) m_Instance = new MyResourceManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static MyResourceManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) MyResourceManager* temp = m_Instance; }
	
public:
	//　画像ファイルを読み込む関数
    std::shared_ptr<MyResource> LoadImageResource(std::string path,bool isEternal = false, bool isAsync = false);
	// サウンドファイルを読み込む関数
    std::shared_ptr<MyResource> LoadSoundResource(std::string path, bool isEternal = false, bool isAsync = false);
	// モデルファイルを読み込む関数
	std::shared_ptr<MyResource> LoadModelResource(std::string path, bool isEternal = false, bool isAsync = false);
	// リソースマネージャーの削除関数
	void Delete(const std::string& path);	
	// 
	bool IsFinishLoading();

	// 3Dモデルのハンドルを格納したリストを取得できる関数
	//std::list<int> Get3DHandleList() { return m_Handle3D; }


private:
	std::unordered_map < std::string, std::shared_ptr<MyResource>> m_ResourceTable;
	std::list<int> m_Handle3D;
	static MyResourceManager* m_Instance;
	SceneManager* m_sceneManager;
	SceneBase* m_LodingScene;

};
