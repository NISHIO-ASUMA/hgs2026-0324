//=========================================================
//
// ビルボード処理 [ billboard.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "billboard.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CBillboard::CBillboard(int nPriority): CObject(nPriority),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_fWidth(0.0f),
m_fHeight(0.0f),
m_FlashCount(NULL),
m_nCountAnim(NULL),
m_nPatterAnim(NULL),
m_nIdxTexture(-1),
m_col(COLOR_WHITE),
m_isTests(true),
m_pVtxBuff(nullptr),
m_mtxWorld{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CBillboard::~CBillboard()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CBillboard* CBillboard::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot,float fWidth, float fHeight,const char * pTexName)
{
	// インスタンス生成
	CBillboard* pBillboard = new CBillboard;
	if (pBillboard == nullptr) return nullptr;

	// オブジェクトセット
	pBillboard->SetPos(pos);
	pBillboard->SetSize(fWidth, fHeight);
	pBillboard->SetRot(rot);
	pBillboard->SetTexture(pTexName);

	// 初期化失敗時
	if (FAILED(pBillboard->Init())) return nullptr;

	// ビルボードのポインタを返す
	return pBillboard;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBillboard::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	// 法線情報の設定
	pVtx[0].nor = 
	pVtx[1].nor = 
	pVtx[2].nor = 
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f,-1.0f);	

	// 頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = COLOR_WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CBillboard::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, -m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, -m_fHeight, 0.0f);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor = 
	pVtx[1].nor = 
	pVtx[2].nor = 
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = m_col;

	// アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 描画処理
//=========================================================
void CBillboard::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (m_isTests)
	{
		// Zテストを適用
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを宣言
	D3DXMATRIX mtxView;

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (m_nIdxTexture == -1)
	{
		// テクスチャがない場合はnullセット
		pDevice->SetTexture(0, nullptr);
	}
	else
	{
		// テクスチャを設定
		pDevice->SetTexture(0, CManager::GetInstance()->GetTexture()->GetAddress(m_nIdxTexture));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャを戻す
	pDevice->SetTexture(0, nullptr);

	if (m_isTests)
	{
		// Zテストを戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
//=========================================================
// テクスチャセット
//=========================================================
void CBillboard::SetTexture(const char * pTexName)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// パスを連結
	std::string TexName = "data/TEXTURE/";
	TexName += pTexName;

	// 割り当て
	m_nIdxTexture = pTexture->Register(TexName.c_str());
}
//=========================================================
// UV設定処理
//=========================================================
void CBillboard::SetUV(float fTexU, float fTexU1, float fTexV)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + fTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + fTexU1, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fTexU, fTexV);
	pVtx[3].tex = D3DXVECTOR2(0.0f + fTexU1, fTexV);

	//アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// アニメーション処理
//=========================================================
void CBillboard::SetAnim(const int nMaxPattern,const int nMaxAnimCount,float fTexU, float fTexV)
{
	// アニメーションカウンターを加算
	m_nCountAnim++;

	// カウントが上限より大きくなった時
	if (m_nCountAnim >= nMaxAnimCount)
	{
		m_nCountAnim = NULL;	// カウンターを初期値に戻す

		m_nPatterAnim++;		// パターンナンバーを更新

		// テクスチャ座標更新
		SetUV(m_nPatterAnim * fTexU, fTexU + m_nPatterAnim * fTexU, fTexV); // U,U1,V1座標
	}

	// パターンナンバーが最大値より大きくなった時
	if (m_nPatterAnim > nMaxPattern)
	{
		m_nPatterAnim = NULL;		// パターンナンバーを初期値に戻す
	}
}
//=========================================================
// 点滅処理
//=========================================================
void CBillboard::Flash(const int nMaxFlashTime, const int Digittime)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 点滅カウントを加算
	m_FlashCount++;

	// カラー変数
	D3DXCOLOR col = COLOR_WHITE;

	// 点滅カウントと一致したとき
	if (m_FlashCount == Digittime)		
	{
		// 頂点カラーの設定
		col = COLOR_GLAY;

		// カラーセット
		SetCol(col);
	}
	else if (m_FlashCount == nMaxFlashTime)	// 最大継続時間と一致したとき
	{
		// 頂点カラーの設定
		col = COLOR_WHITE;

		// カラーセット
		SetCol(col);

		// 初期値に戻す
		m_FlashCount = NULL;
	}

	//アンロック
	m_pVtxBuff->Unlock();
}