//=========================================================
//
// リザルト用スコア管理処理 [ resultscoremanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultscoremanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <ostream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "resultscore.h"
#include "manager.h"
#include "input.h"
#include "ranking.h"
#include "load.h"
#include "template.h"

//=========================================================
// コンストラクタ
//=========================================================
CResultScoreManager::CResultScoreManager() : m_pResultScore{},
m_nEatScore(NULL),
m_nPointScore(NULL),
m_nLastScore(NULL), 
m_nMathScore(NULL)
{
}
//=======================================================
// デストラクタ
//=======================================================
CResultScoreManager::~CResultScoreManager()
{

}
//=========================================================
// インスタンス取得処理
//=========================================================
CResultScoreManager* CResultScoreManager::GetInstancce(void)
{
	static CResultScoreManager Instance;
	return &Instance;
}
//=======================================================
// 初期化処理
//=======================================================
HRESULT CResultScoreManager::Init(void)
{
	// ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	// 構造体宣言
	Config info = {};
	
	// スコアファイル読み込み
	Load();

	// スコアを生成する
	m_pResultScore[info.IDX_FEED]  = CResultScore::Create(D3DXVECTOR3(1250.0f, 230.0f, 0.0f), 140.0f, 50.0f);	// 食べたスコア
	m_pResultScore[info.IDX_QUEEN] = CResultScore::Create(D3DXVECTOR3(1250.0f, 400.0f, 0.0f), 140.0f, 50.0f);	// ポイントスコア
	m_pResultScore[info.IDX_ALL]   = CResultScore::Create(D3DXVECTOR3(1255.0f, 630.0f, 0.0f), 160.0f, 60.0f);	// 最終スコア

	// 食べた数のスコアをセットする
	m_pResultScore[info.IDX_FEED]->SetAnimScore(m_nEatScore);

	// スコアをセットする
	m_pResultScore[info.IDX_QUEEN]->SetAnimScore(m_nPointScore);

	// クリア
	m_nMathScore = 0;

	// 合計を出す
	m_nMathScore = m_nEatScore + m_nPointScore;

	// 最終スコアをセットする
	m_pResultScore[info.IDX_ALL]->SetAnimScore(m_nMathScore);

	return S_OK;
}
//=======================================================
// 終了処理
//=======================================================
void CResultScoreManager::Uninit(void)
{
	// 最終スコアを書き出す
	m_pResultScore[Config::IDX_ALL]->Save();

	// ロードクラスの破棄
	m_pLoad.reset();
}
//=======================================================
// 更新処理
//=======================================================
void CResultScoreManager::Update(void)
{

}
//=======================================================
// バイナリファイル読み込み処理
//=======================================================
void CResultScoreManager::Load(void)
{
	// 読み取った値を格納するメンバ変数
	m_nPointScore = NULL;
	m_nEatScore = NULL;

	// 食べた数のスコアを読み込む
	m_nEatScore = m_pLoad->LoadInt(Config::EATSCORE);

	// 防衛スコアを読み込む
	m_nPointScore = m_pLoad->LoadInt(Config::POINTSCORE);
}
//=======================================================
// 率の値をスコアに変換する関数
//=======================================================
int CResultScoreManager::MathRateScore(void)
{
#if 0
	// 念のためガード
	if (m_nDefenceScore <= NULL)
	{
		return NULL;
	}

	//  割合を計算する
	float rate = static_cast<float>(m_nDefenceScore) / Config::QUEEN_HP;

	// クランプする
	rate = Clump(rate, 0.0f, 1.0f);

	// 最終計算のスコアを出す
	int resultScore = static_cast<int>(rate * Config::MAX_MATHSCORE);
#endif
	return 0;
}