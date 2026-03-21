//=========================================================
//
// タイトルで出現するオブジェクト処理 [ titleobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "titleobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "jsonmanager.h"
//#include "worldwallmanager.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CTitleObject* CTitleObject::m_pInstance = nullptr;					// シングルトン変数

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace TITLEOBJECT
{
	constexpr const char* LoadName = "data/JSON/Titleobject.json";		// 読み込むjsonファイル
	constexpr const char* WallName = "data/JSON/TitleWorldWall.json";	// 壁のjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CTitleObject::CTitleObject()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTitleObject::~CTitleObject()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleObject::Init(void)
{
	// タイトルで使うオブジェクトの読み込み
	auto JsonManager = CManager::GetInstance()->GetJsonManager();
	JsonManager->Load(TITLEOBJECT::LoadName);

	//// 見えない壁生成
	//m_pWallManager = std::make_unique<CWorldWallManager>();
	//JsonManager->SetWorldWallManager(m_pWallManager.get());
	//m_pWallManager->Init(TITLEOBJECT::WallName);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleObject::Uninit(void)
{
	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CTitleObject::Update(void)
{
}
//=========================================================
// シングルトン取得処理
//=========================================================
CTitleObject* CTitleObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CTitleObject;

	// シングルトンを返す
	return m_pInstance;
}