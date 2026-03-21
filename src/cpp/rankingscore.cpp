//=========================================================
//
// ランキングスコア処理 [ rankingscore.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankingscore.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "number.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CRankingScore::CRankingScore(int nPriority) : CObject(nPriority),
m_fHeight(NULL),
m_fWidth(NULL),
m_pos(VECTOR3_NULL)
{
	for (int nCntData = 0; nCntData < Config::RANKING_MAX; nCntData++)
	{
		// スコア初期化
		m_aRankData[nCntData] = NULL;

		for (int nCnt = 0; nCnt < Config::RANKSCOREDIGIT; nCnt++)
		{
			// ナンバーポインタ
			m_apNumber[nCntData][nCnt] = nullptr;
		}
	}
}
//=========================================================
// デストラクタ
//=========================================================
CRankingScore::~CRankingScore()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CRankingScore* CRankingScore::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	// インスタンス生成
	CRankingScore* pRankScore = new CRankingScore;
	if (pRankScore == nullptr) return nullptr;

	// オブジェクト設定
	pRankScore->SetPos(pos);
	pRankScore->SetWidth(fWidth);
	pRankScore->SetHeight(fHeight);

	// 初期化失敗時
	if (FAILED(pRankScore->Init())) return nullptr;

	// 生成されたポインタを返す
	return pRankScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingScore::Init(void)
{
	// スコアをロード
	Load();

	// 横幅計算
	float fTexPos = m_fWidth / Config::RANKSCOREDIGIT;

	for (int nRank = 0; nRank < Config::RANKING_MAX; nRank++)
	{
		// 桁数分
		for (int nDigit = 0; nDigit < Config::RANKSCOREDIGIT; nDigit++)
		{
			// インスタンス生成
			m_apNumber[nRank][nDigit] = new CNumber;

			// Y座標をずらす
			float yOffset = m_pos.y + (m_fHeight + Config::POS_Y_VALUE) * nRank;

			// 初期化処理
			m_apNumber[nRank][nDigit]->Init
			(
				D3DXVECTOR3(m_pos.x - (fTexPos * Config::POS_WIDTH_VALUE * nDigit), yOffset, 0.0f),
				fTexPos,
				m_fHeight
			);

			// サイズ設定
			m_apNumber[nRank][nDigit]->SetSize(fTexPos, m_fHeight);

			// カラー設定
			m_apNumber[nRank][nDigit]->SetCol(SCORECOLOR);

			// テクスチャ設定
			m_apNumber[nRank][nDigit]->SetTexture(Config::TEXNAME);
		}
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingScore::Uninit(void)
{
	// 使っている桁数分の破棄
	for (int nRankData = 0; nRankData < Config::RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt < Config::RANKSCOREDIGIT; nCnt++)
		{
			if (m_apNumber[nRankData][nCnt] != nullptr)
			{
				// ナンバークラスの終了処理
				m_apNumber[nRankData][nCnt]->Uninit();
				delete m_apNumber[nRankData][nCnt];
				m_apNumber[nRankData][nCnt] = nullptr;
			}
		}
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CRankingScore::Update(void)
{
	// スコアの桁数更新
	for (int nRank = 0; nRank < Config::RANKING_MAX; nRank++)
	{
		// 現在のスコアを格納
		int nScore = m_aRankData[nRank];

		for (int nDigit = 0; nDigit < Config::RANKSCOREDIGIT; nDigit++)
		{
			// 1桁ずつ取り出す
			int nNum = nScore % Config::DIGITNUM;
			nScore /= Config::DIGITNUM;

			// 桁更新
			m_apNumber[nRank][nDigit]->SetDigit(nNum);
		}
	}
}
//=========================================================
// 描画処理
//=========================================================
void CRankingScore::Draw(void)
{
	// 使っている桁数分の描画
	for (int nRankData = 0; nRankData < Config::RANKING_MAX; nRankData++)
	{
		for (int nCnt = 0; nCnt <Config::RANKSCOREDIGIT; nCnt++)
		{
			// ナンバー描画
			m_apNumber[nRankData][nCnt]->Draw();
		}
	}
}
//=========================================================
// ランキングスコア読み込み
//=========================================================
void CRankingScore::Load(void)
{
#if 0
	// Networkクラス取得
	CNetWork* pNetWork = CManager::GetInstance()->GetNetWork();
	if (!pNetWork) return;

	// サーバー接続チェック
	if (!pNetWork->GetIsConnect()) return;

	// 受信用配列
	int recvData[Config::RANKING_MAX] = {};

	// ランキングデータを受信出来なかったら
	if (!pNetWork->RecvInt(recvData)) return;

	// メンバ変数へコピー
	for (int nRecvScore = 0; nRecvScore < Config::RANKING_MAX; nRecvScore++)
	{
		m_aRankData[nRecvScore] = recvData[nRecvScore];
	}

#endif
}