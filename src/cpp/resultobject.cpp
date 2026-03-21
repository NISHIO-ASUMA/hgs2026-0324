//=========================================================
//
// リザルトのオブジェクト管理 [ resultobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "resultscoremanager.h"
#include "blockmanager.h"
#include "jsonmanager.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CResultObject* CResultObject::m_pInstance = nullptr;				// シングルトン変数

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace RESULTOBJECT
{
	constexpr const char* LoadName = "data/JSON/Resultobject.json"; // 読み込むjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CResultObject::CResultObject()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CResultObject::~CResultObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultObject::Init(void)
{
	// マップ読み込み
	m_pBlock = std::make_unique<CBlockManager>();
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlock.get());
	m_pBlock->Init();

	// リザルトシーンオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(RESULTOBJECT::LoadName);

	// リザルトのスコア生成
	CResultScoreManager::GetInstancce()->Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultObject::Uninit(void)
{
	// リザルトのスコア破棄
	CResultScoreManager::GetInstancce()->Uninit();

	// ステージマップの破棄
	m_pBlock.reset();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// インスタンス取得処理
//=========================================================
CResultObject* CResultObject::GetInstance(void)
{
	// nullなら生成
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CResultObject;
	}

	// インスタンスを返す
	return m_pInstance;
}