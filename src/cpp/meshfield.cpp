//=========================================================
//
// メッシュフィールド処理 [ meshfield.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "meshfield.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//=========================================================
// コンストラクタ
//=========================================================
CMeshField::CMeshField(int nPriority) : CObject(nPriority),
m_pIdx(nullptr),
m_pVtx(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_MeshFiled{},
m_mtxWorld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CMeshField::~CMeshField()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CMeshField* CMeshField::Create
(
	const D3DXVECTOR3& pos, 
	const float& fRadiusX, 
	const float& fRadiusZ, 
	const int& nNumX, 
	const int& nNumZ
)
{
	// インスタンス生成
	CMeshField* pMeshField = new CMeshField;
	if (pMeshField == nullptr) return nullptr;

	// オブジェクト設定
	pMeshField->m_pos = pos;
	pMeshField->m_MeshFiled.fRadius = fRadiusX;
	pMeshField->m_MeshFiled.fRadiusZ = fRadiusZ;
	pMeshField->m_MeshFiled.nNumX = nNumX;
	pMeshField->m_MeshFiled.nNumZ = nNumZ;

	// テクスチャ設定
	pMeshField->SetTexture();

	// 初期化失敗時
	if (FAILED(pMeshField->Init())) return nullptr;

	return pMeshField;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMeshField::Init(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice =CManager::GetInstance()->GetInstance()->GetRenderer()->GetDevice();

	// 頂点計算
	m_MeshFiled.nNumAllVtx = ((m_MeshFiled.nNumX + 1) * (m_MeshFiled.nNumZ + 1)); // 頂点数
	m_MeshFiled.nNumPrimitive = (((m_MeshFiled.nNumX * m_MeshFiled.nNumZ) * 2)) + (4 * (m_MeshFiled.nNumZ - 1)); // ポリゴン数
	m_MeshFiled.nNumIdx = m_MeshFiled.nNumPrimitive + 2; // インデックス数

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshFiled.nNumAllVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtx,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshFiled.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdx,
		NULL);

	// 変数の初期化
	m_rot = VECTOR3_NULL;

	// 頂点情報のポインタを宣言
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標を計算する変数
	float fTexX = 1.0f / m_MeshFiled.nNumX;
	float fTexY = 1.0f / m_MeshFiled.nNumZ;
	int nCnt = 0;

	D3DXVECTOR3 MathPos = m_pos;

	// 縦軸の頂点
	for (int nCntZ = 0; nCntZ <= m_MeshFiled.nNumZ; nCntZ++)
	{
		// 横軸の頂点
		for (int nCntX = 0; nCntX <= m_MeshFiled.nNumX; nCntX++)
		{
			// 頂点座標を計算
			MathPos.x = ((m_MeshFiled.fRadius / m_MeshFiled.nNumX) * nCntX) - (m_MeshFiled.fRadius * 0.5f);
			MathPos.y = m_pos.y;
			MathPos.z = m_MeshFiled.fRadiusZ - ((m_MeshFiled.fRadiusZ / m_MeshFiled.nNumZ) * nCntZ) - (m_MeshFiled.fRadiusZ * 0.5f);

			// 頂点座標の設定
			pVtx[nCnt].pos = MathPos;

			// 法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nCnt].col = COLOR_WHITE;

			// テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX, nCntZ * fTexY);

			// 加算
			nCnt++;
		}
	}

	// アンロック
	m_pVtx->Unlock();

	// インデックスバッファのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdx->Lock(0, 0, (void**)&pIdx, 0);

	WORD IndxNum = m_MeshFiled.nNumX + 1;// X
	WORD IdxCnt = 0;// 配列
	WORD Num = 0;

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_MeshFiled.nNumZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_MeshFiled.nNumX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// 最後の行じゃなかったら
		if (IndxCount1 < m_MeshFiled.nNumZ - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdx->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの解放
	if (m_pVtx != nullptr)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}

	// インデックスバッファの解放
	if (m_pIdx != nullptr)
	{
		m_pIdx->Release();
		m_pIdx = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMeshField::Update(void)
{
	// 頂点情報のポインタを宣言
	VERTEX_3D* pVtx = nullptr;

	//頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標を計算する変数
	float fTexX = 1.0f / m_MeshFiled.nNumX;
	float fTexY = 1.0f / m_MeshFiled.nNumZ;
	int nCnt = 0;

	D3DXVECTOR3 MathPos = m_pos;

	//縦
	for (int nCntZ = 0; nCntZ <= m_MeshFiled.nNumZ; nCntZ++)
	{
		//横
		for (int nCntX = 0; nCntX <= m_MeshFiled.nNumX; nCntX++)
		{
			// 頂点座標を計算
			MathPos.x = ((m_MeshFiled.fRadius / m_MeshFiled.nNumX) * nCntX) - (m_MeshFiled.fRadius * 0.5f);
			MathPos.y = m_pos.y;
			MathPos.z = m_MeshFiled.fRadiusZ - ((m_MeshFiled.fRadiusZ / m_MeshFiled.nNumZ) * nCntZ) - (m_MeshFiled.fRadiusZ * 0.5f);

			// 頂点座標の設定
			pVtx[nCnt].pos = MathPos;

			// 法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nCnt].col = COLOR_WHITE;

			// テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX, nCntZ * fTexY);

			// 加算
			nCnt++;
		}
	}

	// アンロック
	m_pVtx->Unlock();

	// インデックスバッファのポインタ
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdx->Lock(0, 0, (void**)&pIdx, 0);

	WORD IndxNum = m_MeshFiled.nNumX + 1;// X

	WORD IdxCnt = 0;// 配列

	WORD Num = 0;

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < m_MeshFiled.nNumZ; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= m_MeshFiled.nNumX; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// 最後の行じゃなかったら
		if (IndxCount1 < m_MeshFiled.nNumZ - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdx->Unlock();
}
//=========================================================
// 描画処理
//=========================================================
void CMeshField::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスの行列計算
	m_mtxWorld = mtxRot * mtxTrans;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdx);

	// テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ割り当て
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	pDevice->SetTexture(0, pTexture->GetAddress(m_MeshFiled.nTexIdx));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_MeshFiled.nNumAllVtx, 0, m_MeshFiled.nNumPrimitive);

	// テクスチャを戻す
	pDevice->SetTexture(0, nullptr);
}
//=========================================================
// テクスチャ設定
//=========================================================
void CMeshField::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();

	// 割り当て
	m_MeshFiled.nTexIdx = pTexture->Register("data/TEXTURE/field.jpg");
}