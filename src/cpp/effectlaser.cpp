//=========================================================
//
// レーザーエフェクト処理 [ effectlaser.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "effectlaser.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CEffectLaser::CEffectLaser(int nPriority) : CBillboard(nPriority),
m_nLife(NULL),
m_fRadius(NULL),
m_fLength(NULL),
m_move(VECTOR3_NULL),
m_EndPos(VECTOR3_NULL),
m_OldPos(VECTOR3_NULL)
{
}
//=========================================================
// デストラクタ
//=========================================================
CEffectLaser::~CEffectLaser()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CEffectLaser* CEffectLaser::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Endpos, D3DXCOLOR col, D3DXVECTOR3 move, int nLife, float fRadius)
{
	// インスタンス生成
	CEffectLaser* pLaser = new CEffectLaser;
	if (pLaser == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pLaser->Init()))
	{
		return nullptr;
	}

	// オブジェクト設定
	pLaser->SetPos(pos);
	pLaser->SetSize(fRadius, fRadius);
	pLaser->SetCol(col);
	pLaser->SetTexture("effect000.jpg");

	pLaser->m_fRadius = fRadius;
	pLaser->m_move = move;
	pLaser->m_nLife = nLife;
	pLaser->m_EndPos = Endpos;

	// 生成されたポインタを返す
	return pLaser;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEffectLaser::Init(void)
{
	// 親クラスの初期化
	CBillboard::Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEffectLaser::Uninit(void)
{
	// 親クラスの終了
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEffectLaser::Update(void)
{
	// 座標,カラー取得
	D3DXVECTOR3 Effectpos = GetPos();

	// オブジェクト更新
	CBillboard::Update();

	// 移動量の更新
	Effectpos += m_move;

	// 半径をデクリメント
	m_fRadius -= 0.05f;

	// 終点判定
	D3DXVECTOR3 toEnd = m_EndPos - Effectpos;
	float fDist = D3DXVec3Length(&toEnd);

	// 一定以下なら
	if (fDist < EndFloatValue)
	{
		Effectpos = m_EndPos;
		m_nLife = 0;
	}

	// 座標をセットする
	SetPos(Effectpos);

	if (m_fRadius <= 0.0f)
	{
		m_fRadius = 0.0f;
	}

	// サイズセット
	SetSize(m_fRadius, m_fRadius);

	// 体力を減らす
	m_nLife--;

	// 0以下の時
	if (m_nLife <= 0)
	{
		// 削除する
		Uninit();
		return;
	}

	// オブジェクト更新
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CEffectLaser::Draw(void)
{
	// デバイスの取得
	const auto& pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// αブレンディングの加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// αテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zテストを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// 親クラス描画
	CBillboard::Draw();

	// αテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
