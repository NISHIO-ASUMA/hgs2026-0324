//=========================================================
//
// パーティクル処理 [ particle.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "particle.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "effect.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace PARTICLEINFO
{
	constexpr int RAND_MAXVALUE		= 629;		// 最大ランダム数
	constexpr int RAND_MINVALUE		= 314;		// 最小ランダム数
	constexpr int DIGIT_LIFE		= 10;		// 寿命を割る値
	constexpr float DIGIT_RAND		= 100.0f;	// ランダム値を割る値
	constexpr float DIGIT_LENGTH	= 10.0f;	// 飛散距離の割る値
};

//=========================================================
// コンストラクタ
//=========================================================
CParticle::CParticle(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_nLength(NULL),
m_nMaxParticle(NULL),
m_nLife(NULL),
m_nRadius(NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CParticle::~CParticle()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CParticle* CParticle::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXCOLOR& col, 
	const int& nMaxParticle, 
	const int& nRadius, 
	const int& nLength, 
	const int& nLife
)
{
	// インスタンス生成
	CParticle* pParticle = new CParticle;
	if (pParticle == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pParticle->Init())) return nullptr;

	// オブジェクト設定
	pParticle->m_pos = pos;
	pParticle->m_col = col;
	pParticle->m_nMaxParticle = nMaxParticle;
	pParticle->m_nLength = nLength;
	pParticle->m_nLife = nLife;
	pParticle->m_nRadius = nRadius;

	return pParticle;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CParticle::Init(void)
{
	// 配列クリア
	m_pEffect.clear();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CParticle::Uninit(void)
{
	// 配列クリア
	m_pEffect.clear();

	// 親クラスの破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CParticle::Update(void)
{
	// カラーの設定
	D3DXCOLOR col = m_col;

	// 座標を取得
	D3DXVECTOR3 pos = m_pos;

	//パーティクルの生成
	for (int nCntApper = 0; nCntApper < m_nMaxParticle; nCntApper++)
	{
		// 移動量の設定
		float fAngelX = static_cast<float>(rand() % PARTICLEINFO::RAND_MAXVALUE - PARTICLEINFO::RAND_MINVALUE) / PARTICLEINFO::DIGIT_RAND;
		float fAngelY = static_cast<float>(rand() % PARTICLEINFO::RAND_MAXVALUE - PARTICLEINFO::RAND_MINVALUE) / PARTICLEINFO::DIGIT_RAND;

		// 長さ設定
		float fLength = (static_cast<float>(rand() % m_nLength) / PARTICLEINFO::DIGIT_LENGTH);

		// 移動量変数
		D3DXVECTOR3 Move = VECTOR3_NULL;

		// 移動距離設定
		Move.x = sinf(fAngelX) * sinf(fAngelY) * fLength;
		Move.y = cosf(fAngelX) * fLength;
		Move.z = sinf(fAngelX) * cosf(fAngelY) * fLength;

		//半径の設定
		float fRadius = (static_cast<float>(rand() % m_nRadius) / PARTICLEINFO::DIGIT_LENGTH + 0.7f);

		// 寿命の設定
		int nLife = (static_cast<float>(rand() % m_nLife) / PARTICLEINFO::DIGIT_LIFE);

		// エフェクト生成
		CEffect* pEffect = CEffect::Create(pos, col, Move, nLife, fRadius);

		if (pEffect) 
			m_pEffect.push_back(pEffect);
		else
			break;
	}

	// 破棄
	Uninit();
}
//=========================================================
// 描画処理
//=========================================================
void CParticle::Draw(void)
{
	
}