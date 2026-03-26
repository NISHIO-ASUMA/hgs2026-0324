//=========================================================
//
// スコア処理 [ score.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "score.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "load.h"

//=========================================================
// コンストラクタ
//=========================================================
CScore::CScore(int nPriority) : CObject(nPriority),
m_nScore(NULL),
m_col(COLOR_WHITE),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fHeight(NULL),
m_fWidth(NULL),
m_apNumber{},
m_pLoad{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CScore::~CScore()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CScore* CScore::Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight)
{
	// スコアインスタンス生成
	CScore* pScore = new CScore;
	if (pScore == nullptr) return nullptr;

	// オブジェクト設定
	pScore->SetPos(pos);
	pScore->SetWidth(fWidth);
	pScore->SetHeight(fHeight);

	// 初期化失敗時
	if (FAILED(pScore->Init())) return nullptr;
	
	// 生成されたポインタを返す
	return pScore;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CScore::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / Config::NUM_SCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < Config::NUM_SCORE; nCnt++)
	{
		// インスタンス生成
		m_apNumber[nCnt] = new CNumber;

		// 初期化処理
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * Config::DIGIT_VALUE * nCnt), m_pos.y,0.0f), fTexPos, m_fHeight);

		// ナンバー変数のサイズ
		m_apNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// 座標設定
		m_apNumber[nCnt]->SetPos(m_pos);

		// テクスチャセット
		m_apNumber[nCnt]->SetTexture(Config::TEX_NAME);
	}

	// 読み込み処理ポインタ生成
	m_pLoad = std::make_unique<CLoad>();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CScore::Uninit(void)
{
	// ナンバークラスの破棄
	for (auto number : m_apNumber)
	{
		if (number != nullptr)
		{
			number->Uninit();
			delete number;
			number = nullptr;
		}
	}

	// ロードクラスのポインタ破棄
	m_pLoad.reset();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CScore::Update(void)
{
	// スコア格納
	int nScore = m_nScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < Config::NUM_SCORE; nCntScore++)
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % Config::NUM_DIGIT;
		nScore /= Config::NUM_DIGIT;

		// ナンバー更新
		m_apNumber[nCntScore]->Update();

		// 桁更新
		m_apNumber[nCntScore]->SetDigit(nDigit);
	}
}
//=========================================================
// 描画処理
//=========================================================
void CScore::Draw(void)
{
	//for (auto number : m_apNumber)
	//{
	//	if (number != nullptr)
	//	{
	//		number->Draw();
	//	}
	//}
}
//=========================================================
// スコア加算処理
//=========================================================
void CScore::AddScore(int nValue)
{
	// スコア加算
	m_nScore += nValue;
}
//=========================================================
// スコア初期化
//=========================================================
void CScore::DeleteScore(void)
{
	m_nScore = NULL;
}
//=========================================================
// スコア書き出し処理
//=========================================================
void CScore::SaveScore(void)
{
	// 一個の数値を書き出す
	m_pLoad->SaveInt(Config::SAVE_NAME, m_nScore);
}