//=========================================================
//
// シンプルメッシュシリンダー処理 [ simplemeshcylinder.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "simplemeshcylinder.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//=========================================================
// コンストラクタ
//=========================================================
CSimpleMeshCylinder::CSimpleMeshCylinder(int nPriority) : CObject(nPriority),
m_pIdx(nullptr),
m_pVtx(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_Cylinder{},
m_mtxWorld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CSimpleMeshCylinder::~CSimpleMeshCylinder()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CSimpleMeshCylinder* CSimpleMeshCylinder::Create(D3DXVECTOR3 pos, float fRadius)
{
	// インスタンス生成
	CSimpleMeshCylinder* pMeshcylinder = new CSimpleMeshCylinder;
	if (pMeshcylinder == nullptr) return nullptr;

	// オブジェクト設定
	pMeshcylinder->SetTexture();
	pMeshcylinder->m_pos = pos;
	pMeshcylinder->m_Cylinder.fRadius = fRadius;

	// 初期化失敗時
	if (FAILED(pMeshcylinder->Init())) return nullptr;

	return pMeshcylinder;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSimpleMeshCylinder::Init(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点数,インデックス数,ポリゴン数を計算する
	m_Cylinder.nNumAllVtx = (DIGIT_X + 1) * (DIGIT_Z + 1);
	m_Cylinder.nNumPrimitive = ((DIGIT_X * DIGIT_Z) * 2) + (4 * (DIGIT_Z - 1));
	m_Cylinder.nNumIdx = (m_Cylinder.nNumPrimitive + 2);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_Cylinder.nNumAllVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtx,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_Cylinder.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdx,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	float fTexX = 1.0f / DIGIT_X;
	float fTexY = 1.0f / DIGIT_Z;
	int nCnt = 0;

	// 法線設定用
	D3DXVECTOR3 nor = VECTOR3_NULL;

	// 縦
	for (int nCntZ = 0; nCntZ <= DIGIT_Z; nCntZ++)
	{
		// 横
		for (int nCntX = 0; nCntX <= DIGIT_X; nCntX++)
		{
			// 角度計算
			float fAngle = (D3DX_PI * 2.0f) / DIGIT_X * nCntX;

			// 頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(sinf(fAngle) * m_Cylinder.fRadius, nCntZ * 30.0f, cosf(fAngle) * m_Cylinder.fRadius);

			// 法線ベクトルの設定
			// シリンダーの外側を向くように、水平成分のみを取り出して正規化する
			nor = D3DXVECTOR3(pVtx[nCnt].pos.x, 0.0f, pVtx[nCnt].pos.z);
			D3DXVec3Normalize(&pVtx[nCnt].nor, &nor);

			// 頂点カラーの設定
			pVtx[nCnt].col = LASER;

			// テクスチャ座標の設定
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX, nCntZ * fTexY);

			// 加算
			nCnt++;
		}
	}

	// 頂点バッファをアンロック
	m_pVtx->Unlock();

	// インデックスポインタを宣言
	WORD* pIdx;

	// インデックスバッファのロック
	m_pIdx->Lock(0, 0, (void**)&pIdx, 0);

	WORD IndxNum = DIGIT_X + 1;// X

	WORD IdxCnt = 0;// 配列

	WORD Num = 0;

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < DIGIT_Z; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= DIGIT_X; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// 最後の行じゃなかったら
		if (IndxCount1 < m_Cylinder.nNumDigitZ - 1)
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
void CSimpleMeshCylinder::Uninit(void)
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
void CSimpleMeshCylinder::Update(void)
{
	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// マトリックスにかけ合わせる
	m_mtxWorld = mtxRot * mtxTrans;
}
//=========================================================
// 描画処理
//=========================================================
void CSimpleMeshCylinder::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Zテスト
	pDevice->SetRenderState(D3DRS_CULLMODE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdx);

	// テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ読み込み
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャセット
	pDevice->SetTexture(0, pTexture->GetAddress(m_Cylinder.nTexIdx));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_Cylinder.nNumAllVtx, 0, m_Cylinder.nNumPrimitive);

	// テクスチャを戻す
	pDevice->SetTexture(0, nullptr);

	// Zテストを元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}
//=========================================================
// テクスチャ割り当て
//=========================================================
void CSimpleMeshCylinder::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// テクスチャ割り当て
	m_Cylinder.nTexIdx = pTexture->Register("data/TEXTURE/field100.jpg");
}