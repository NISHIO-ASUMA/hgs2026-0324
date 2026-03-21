//=========================================================
//
// ランキングのオブジェクト管理 [ rankingobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankingobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "rankingscore.h"
#include "jsonmanager.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CRankingObject* CRankingObject::m_pInstance = nullptr; // シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace RANKINGOBJECT
{
	const D3DXVECTOR3 ScorePos		= { 870.0f, 160.0f, 0.0f };	// ランキングスコアの座標
	constexpr float ScoreWidth		= 250.0f;					// ポリゴンの横幅
	constexpr float ScoreHeight		= 40.0f;					// ポリゴンの高さ
	constexpr const char* LoadName	= "data/JSON/Rankingobject.json"; // 読み込むjsonファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CRankingObject::CRankingObject()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CRankingObject::~CRankingObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingObject::Init(void)
{
	// ランキングで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(RANKINGOBJECT::LoadName);

	// ランキングスコア生成
	CRankingScore::Create(RANKINGOBJECT::ScorePos, RANKINGOBJECT::ScoreWidth, RANKINGOBJECT::ScoreHeight);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingObject::Uninit(void)
{
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
CRankingObject* CRankingObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CRankingObject;

	// 生成されたインスタンスを返す
	return m_pInstance;
}