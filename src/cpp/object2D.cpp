//=========================================================
//
// 2Dオブジェクト処理 [ object2D.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//**********************************************************
// クラス定義ヘッダーファイル
//**********************************************************
#include "object2D.h"

//**********************************************************
// システムインクルードファイル
//**********************************************************
#include <string>

//**********************************************************
// インクルードファイル
//**********************************************************
#include "manager.h"
#include "renderer.h"
#include "template.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority),
m_pTexture(nullptr),
m_pVtxBuff(nullptr),
m_rot(VECTOR3_NULL),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_fHeight(NULL),
m_fWidth(NULL),
m_nColorCount(NULL),
m_nDrawType(NULL),
m_nIdxTexture(-1),
m_nAnchorType(ANCHORTYPE_NONE)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CObject2D::~CObject2D()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CObject2D* CObject2D::Create(void)
{
	// インスタンス生成
	CObject2D* pObject2D;

	// インスタンス生成
	pObject2D = new CObject2D;

	// 初期化処理
	pObject2D->Init();

	return pObject2D;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CObject2D::Init(void)
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

	// 頂点情報のポインタ
	VERTEX_2D* pVtx;

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
//=========================================================
// 終了処理
//=========================================================
void CObject2D::Uninit(void)
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
//=========================================================
// 更新処理
//=========================================================
void CObject2D::Update(void)
{
	// アンカーの種類を取得
	switch (m_nAnchorType)
	{
	case ANCHORTYPE_NONE:		// 何もない
		SetCenter();
		break;

	case ANCHORTYPE_CENTER:		// 中心点
		SetCenter();
		break;

	case ANCHORTYPE_LEFTSIDE:	// 左基準点
		SetLeft();
		break;

	case ANCHORTYPE_RIGHTSIDE:	// 右基準点
		SetRight();
		break;

	default:
		break;
	}
}
//=========================================================
// 描画処理
//=========================================================
void CObject2D::Draw(void)
{
	// ポーズオブジェクトなら
	if (CObject::GetObjType() == TYPE_PAUSE)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// テクスチャを戻す
		pDevice->SetTexture(0, nullptr);
	}
	else
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
}
//===========================================================
// UV座標設定関数
//===========================================================
void CObject2D::SetUV(float TexU, float TexV)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TexV);
	pVtx[3].tex = D3DXVECTOR2(TexU, TexV);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===========================================================
// 点滅関数
//===========================================================
void CObject2D::SetFlash(const int nFirstcount, const int nEndcount, const D3DXCOLOR col)
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

	if (fProgress < 0.5f)
	{
		// 線形補間
		alpha = Lerp(0.5f, 1.0f, fProgress * 2.0f);
	}
	else
	{
		// 線形補間
		alpha = Lerp(1.0f, 0.5f, (fProgress - 0.5f) * 2.0f);
	}

	// カラー設定
	D3DXCOLOR ChangeCol(col.r, col.g, col.b, alpha);

	// 現在カラーに適用
	SetCol(ChangeCol);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
//===========================================================
// 中心が基準点のポリゴン
//===========================================================
void CObject2D::SetCenter(void)
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===========================================================
// 左が基準点のポリゴン
//===========================================================
void CObject2D::SetLeft(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
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
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===========================================================
// 右が基準点のポリゴン
//===========================================================
void CObject2D::SetRight(void)
{
	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}
//===========================================================
// テクスチャ割り当て
//===========================================================
void CObject2D::SetTexture(const char* pRegisterName)
{
	// テクスチャクラス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// 無かったら
	if (pRegisterName == nullptr) return;

	// パスを短縮する
	std::string TexName = "data/TEXTURE/";
	TexName += pRegisterName;

	// 割り当てる
	m_nIdxTexture = pTexture->Register(TexName.c_str());
}