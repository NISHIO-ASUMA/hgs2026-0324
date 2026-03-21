//===================================================================
//
// アニメーションする2Dオブジェクト処理 [ animationobject2d.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "animationobject2d.h"

//*******************************************************************
// システムインクルードファイル
//*******************************************************************
#include <string>

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "template.h"

//===================================================================
// コンストラクタ
//===================================================================
CAnimationObject2D::CAnimationObject2D(int nPriority) : CObject(nPriority),
m_pTexture(nullptr),
m_pVtxBuff(nullptr),
m_rot(VECTOR3_NULL),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_fHeight(NULL),
m_fWidth(NULL),
m_nColorCount(NULL),
m_nIdxTexture(-1),
m_fTexU(1.0f),
m_fTexV(1.0f),
m_isLoop(false)
{

}
//===================================================================
// デストラクタ
//===================================================================
CAnimationObject2D::~CAnimationObject2D()
{

}
//===================================================================
// 生成処理
//===================================================================
CAnimationObject2D* CAnimationObject2D::Create
(
	const D3DXVECTOR3& pos, 
	const float fWidth, 
	const float fHeight, 
	const char* pTexName
)
{
	// インスタンス生成
	CAnimationObject2D* pAnim2D = new CAnimationObject2D;
	if (pAnim2D == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pAnim2D->Init())) return nullptr;

	// オブジェクト設定
	pAnim2D->SetPos(pos);
	pAnim2D->SetSize(fWidth, fHeight);
	pAnim2D->SetTexture(pTexName);

	return pAnim2D;
}
//===================================================================
// 初期化処理
//===================================================================
HRESULT CAnimationObject2D::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 2D頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos =
	pVtx[1].pos =
	pVtx[2].pos =
	pVtx[3].pos = VECTOR3_NULL;

	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = COLOR_WHITE;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//===================================================================
// 終了処理
//===================================================================
void CAnimationObject2D::Uninit(void)
{
	// テクスチャの破棄
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//===================================================================
// 更新処理
//===================================================================
void CAnimationObject2D::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = D3DXCOLOR(m_col);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_fTexV);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU, m_fTexV);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===================================================================
// 描画処理
//===================================================================
void CAnimationObject2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// nullなら
	if (m_nIdxTexture == -1)
	{
		// テクスチャを戻す
		pDevice->SetTexture(0, nullptr);
	}
	else
	{
		// テクスチャ取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		if (pTexture == nullptr) return;

		// テクスチャセット
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));
	}

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャを戻す
	pDevice->SetTexture(0, nullptr);
}
//===================================================================
// 点滅処理
//===================================================================
void CAnimationObject2D::SetFlash(const int nFirstcount, const int nEndcount, const D3DXCOLOR& col)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックする
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// カラーカウントを加算
	m_nColorCount++;

	// 点滅する周期を計算する
	int nCycle = nEndcount - nFirstcount;
	if (nCycle <= 0) nCycle = 1;

	// 進行度を設定
	float fProgress = static_cast<float>((m_nColorCount - nFirstcount) % nCycle) / static_cast<float>(nCycle);

	// 透明度を格納する
	float alpha = NULL;

	if (fProgress < HALF)
	{
		// 線形補間
		alpha = Lerp(HALF, Config::LERP_MIN, fProgress * Config::LERP_MAX);
	}
	else
	{
		// 線形補間
		alpha = Lerp(Config::LERP_MIN, HALF, (fProgress - HALF) * Config::LERP_MAX);
	}

	// カラー設定
	D3DXCOLOR ChangeCol(col.r, col.g, col.b, alpha);

	// 現在カラーに適用
	SetCol(ChangeCol);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===================================================================
// テクスチャ割り当て処理
//===================================================================
void CAnimationObject2D::SetTexture(const char* pRegisterName)
{
	// テクスチャクラス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パスを短縮する
	std::string TexName = "data/TEXTURE/";
	TexName += pRegisterName;

	// 割り当てる
	m_nIdxTexture = pTexture->Register(TexName.c_str());
}