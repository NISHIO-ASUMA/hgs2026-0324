//=========================================================
//
// 移動可能な3Dオブジェクト処理 [ move3dobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "move3dobject.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>

//*********************************************************
// インクルードガード
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CMove3DObject::CMove3DObject(int nPriority) : CObject(nPriority),
m_col(COLOR_WHITE),
m_fHeight(NULL),
m_fWidth(NULL),
m_move(VECTOR3_NULL),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_pVtxBuff(nullptr),
m_nIdxTexture(-1),
m_mtxWorld{}
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CMove3DObject::~CMove3DObject()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMove3DObject::Init(void)
{
	// デバイスポインタ
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
	pVtx[0].pos =
	pVtx[1].pos =
	pVtx[2].pos =
	pVtx[3].pos = VECTOR3_NULL;

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
void CMove3DObject::Uninit(void)
{
	// 頂点バッファ解放
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMove3DObject::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, m_fWidth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fWidth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, m_fHeight, -m_fWidth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, -m_fWidth);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスの行列計算
	m_mtxWorld = mtxRot * mtxTrans;
}
//=========================================================
// 描画処理
//=========================================================
void CMove3DObject::Draw(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ割り当て
	if (m_nIdxTexture == -1)
	{
		pDevice->SetTexture(0, nullptr);
	}
	else
	{
		// テクスチャクラス取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		if (pTexture == nullptr) return;

		// テクスチャセット
		pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャを消す
	pDevice->SetTexture(0, nullptr);
}
//=========================================================
// テクスチャ割り当て処理
//=========================================================
void CMove3DObject::SetTexture(const char* pTexName)
{
	// テクスチャクラスポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャパス省略
	std::string TexName = "data/TEXTURE/";
	TexName += pTexName;

	// テクスチャクラスに割り当て
	m_nIdxTexture = pTexture->Register(TexName.c_str());
}
//=========================================================
// 座標更新処理
//=========================================================
void CMove3DObject::UpdatePosition(void)
{
	m_posOld = m_pos;
	m_pos += m_move;
}
//=========================================================
// 移動量の減衰処理
//=========================================================
void CMove3DObject::DecayMove(const float fValue)
{
	m_move.x += (0.0f - m_move.x) * fValue;
	m_move.z += (0.0f - m_move.z) * fValue;
}