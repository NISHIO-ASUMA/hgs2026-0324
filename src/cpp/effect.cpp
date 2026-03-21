//=========================================================
//
// エフェクト処理 [ effect.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "effect.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CEffect::CEffect(int nPriority) : CBillboard(nPriority),
m_nLife(NULL),
m_fRadius(NULL),
m_move(VECTOR3_NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CEffect::~CEffect()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CEffect* CEffect::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXCOLOR& col, 
	const D3DXVECTOR3& move, 
	const int& nLife, 
	const float& fRadius
)
{
	// エフェクトポインタ
	CEffect* pEffect = new CEffect;
	if (pEffect == nullptr) return nullptr;

	// 初期化に失敗したら
	if (FAILED(pEffect->Init())) return nullptr;
	
	// 3Dオブジェクトセット
	pEffect->SetSize(fRadius, fRadius);
	pEffect->SetPos(pos);
	pEffect->SetCol(col);
	pEffect->SetTexture(Config::TEXNAME);

	// 半径を代入
	pEffect->SetLife(nLife);
	pEffect->m_fRadius = fRadius;
	pEffect->m_move = move;

	// エフェクトポインタを返す
	return pEffect;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEffect::Init(void)
{
	// 親クラスの初期化
	CBillboard::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEffect::Uninit(void)
{
	// 親クラスの終了
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEffect::Update(void)
{
	// 座標,カラー取得
	D3DXVECTOR3 Effectpos = GetPos();

	//カラーの設定
	D3DXCOLOR col = GetCol();

	// オブジェクト更新
	CBillboard::Update();

	// 移動量の更新
	Effectpos += m_move;

	// 半径をデクリメント
	m_fRadius -= Config::DecPaw;

	// α値を減少
	col.a -= Config::DecPaw;

	// 座標をセットする
	SetPos(Effectpos);

	// カラーをセット
	SetCol(col);

	// 0以下なら
	if (m_fRadius <= 0.0f) m_fRadius = 0.0f;

	// サイズセット
	SetSize(m_fRadius, m_fRadius);

	// 体力を減らす
	m_nLife--;

	// 0以下の時
	if (m_nLife <= NULL)
	{
		// 削除する
		Uninit();
	}
}
//=========================================================
// 描画処理
//=========================================================
void CEffect::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	// ビルボード描画
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