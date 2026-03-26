//=========================================================
//
// リザルトに表示するスコアの処理 [ resultscore.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultscore.h"

//*********************************************************
// システムインクルード
//*********************************************************
#include <fstream>
#include <algorithm>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "load.h"
#include "number.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CResultScore::CResultScore(int nPriority) : CObject(nPriority),
m_pLoad(nullptr),
m_pNumber{},
m_pos(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_nCurrentScore(NULL),
m_nDuration(NULL),
m_nLoadScore(NULL),
m_nStartScore(NULL),
m_nTargetScore(NULL),
m_nTimer(NULL),
m_isUse(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CResultScore::~CResultScore()
{

}
//=========================================================
// 生成処理
//=========================================================
CResultScore* CResultScore::Create(const D3DXVECTOR3& pos, const float fWidth, const float fHeight)
{
	// インスタンス生成
	CResultScore* pScore = new CResultScore;
	if (pScore == nullptr) return nullptr;

	// オブジェクト設定
	pScore->SetPos(pos);
	pScore->SetWidth(fWidth);
	pScore->SetHeight(fHeight);

	// 初期化失敗時
	if (FAILED(pScore->Init())) return nullptr;

	return pScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultScore::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / Config::NUM_SCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < Config::NUM_SCORE; nCnt++)
	{
		// インスタンス生成
		m_pNumber[nCnt] = new CNumber;

		// 初期化処理
		m_pNumber[nCnt]->Init
		(
			D3DXVECTOR3(m_pos.x - (fTexPos * Config::POSX_VALUE * nCnt), m_pos.y, 0.0f), 
			fTexPos, 
			m_fHeight
		);

		// ナンバー変数のサイズ
		m_pNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// 座標設定
		m_pNumber[nCnt]->SetPos(m_pos);

		// テクスチャセット
		m_pNumber[nCnt]->SetTexture(Config::TEXNAME);
		m_pNumber[nCnt]->SetCol(SCORECOLOR);

	}

	// ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultScore::Uninit(void)
{
	// ポインタの破棄
	for (auto number : m_pNumber)
	{
		// nullチェック
		if (number != nullptr)
		{
			number->Uninit();
			delete number;
			number = nullptr;
		}
	}

	// ユニークポインタの破棄
	m_pLoad.reset();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CResultScore::Update(void)
{
	// アニメーション更新
	UpdateAnimScore();
}
//=========================================================
// 描画処理
//=========================================================
void CResultScore::Draw(void)
{
	// ナンバーの描画
	for (auto number : m_pNumber)
	{
		number->Draw();
	}
}
//=========================================================
// データ書き出し処理
//=========================================================
void CResultScore::Save(void)
{
	// nullだったら
	if (!m_pLoad) return;

	// バイナリ数値データを保存
	m_pLoad->SaveInt(Config::SAVEFILE, m_nLoadScore);
}
//=========================================================
// アニメーション更新関数
//=========================================================
void CResultScore::UpdateAnimScore(void)
{
	// まだアニメーション中
	if (m_nTimer < m_nDuration)
	{
		// イージング適用
		float fEasetime = CEasing::SetEase(m_nTimer, m_nDuration);
		float fRate = CEasing::EaseOutCubic(fEasetime);

		// 現在スコアをイージングさせる
		m_nCurrentScore = m_nStartScore + static_cast<int>(((m_nLoadScore - m_nStartScore) * fRate));

		// 加算
		m_nTimer++;
	}
	else
	{
		// 現在スコアに設定
		m_nCurrentScore = m_nLoadScore;
	}

	// 桁数更新
	int nScore = m_nCurrentScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < Config::NUM_SCORE; nCntScore++)
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % Config::NUM_DIGIT;
		nScore /= Config::NUM_DIGIT;

		// ナンバーの更新
		m_pNumber[nCntScore]->Update();

		// 桁更新
		m_pNumber[nCntScore]->SetDigit(nDigit);
	}
}
//=========================================================
// アニメーションするスコアをセットする関数
//=========================================================
void CResultScore::SetAnimScore(const int& nDestScore)
{
	m_nStartScore = 0;			// 初期化
	m_nLoadScore = nDestScore;  // 目的のスコアを設定
	m_nCurrentScore = 0;		// 現在の値

	m_nTimer = 0;				// カウントタイマー
	m_nDuration = Config::MAX_ANIMTIME; // 3秒掛けて目的の値に
}