//=========================================================
//
// メッシュシリンダー処理 [ meshcylinder.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "meshcylinder.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "debugproc.h"

//=========================================================
// コンストラクタ
//=========================================================
CMeshCylinder::CMeshCylinder(int nPriority) : CObject(nPriority),
m_pIdx(nullptr),
m_pVtx(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_EndPos(VECTOR3_NULL),
m_Cylinder{},
m_mtxWorld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CMeshCylinder::~CMeshCylinder()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CMeshCylinder* CMeshCylinder::Create(const D3DXVECTOR3& pos, const float& fRadius) 
{
	// インスタンス生成
	CMeshCylinder* pMeshcylinder = new CMeshCylinder;
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
// シリンダー初期化処理
//=========================================================
HRESULT CMeshCylinder::Init(void)
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

	float fTexX = 1.0f / DIGIT_X;
	int nCnt = 0;

	for (int nCntZ = 0; nCntZ <= DIGIT_Z; nCntZ++)
	{
		// 長さの比率 (0.0 ～ 1.0)
		float fLerp = (float)nCntZ / (float)DIGIT_Z;

		for (int nCntX = 0; nCntX <= DIGIT_X; nCntX++)
		{
			float fAngle = (D3DX_PI * 2.0f) / DIGIT_X * nCntX;

			// Z軸方向に伸びるように座標を設定
			float x = sinf(fAngle) * m_Cylinder.fRadius;
			float y = cosf(fAngle) * m_Cylinder.fRadius;
			float z = fLerp;

			pVtx[nCnt].pos = D3DXVECTOR3(x, y, z);

			// 法線設定
			D3DXVECTOR3 nor = D3DXVECTOR3(x, y, 0.0f);
			D3DXVec3Normalize(&pVtx[nCnt].nor, &nor);

			pVtx[nCnt].col = COLOR_RED;
			pVtx[nCnt].tex = D3DXVECTOR2(fTexX * nCntX, fLerp);

			nCnt++;
		}
	}

	// 頂点アンロック
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
void CMeshCylinder::Uninit(void)
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
void CMeshCylinder::Update(void)
{
	if (m_EndPos == VECTOR3_NULL) return;

	// 終点から始点
	D3DXVECTOR3 vDir = m_EndPos - m_pos;
	float fDistance = D3DXVec3Length(&vDir);

	if (fDistance < 0.0001f) return; 

	// 1. スケーリング 
	D3DXMATRIX mtxScale;
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, fDistance);

	// 2. 回転
	D3DXVECTOR3 vAxisZ;
	D3DXVec3Normalize(&vAxisZ, &vDir);

	// 上方向ベクトル
	D3DXVECTOR3 vUp(0, 1, 0);
	if (fabsf(D3DXVec3Dot(&vAxisZ, &vUp)) > 0.99f) vUp = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// 内積と正規化
	D3DXVECTOR3 vAxisX, vAxisY;
	D3DXVec3Cross(&vAxisX, &vUp, &vAxisZ);
	D3DXVec3Normalize(&vAxisX, &vAxisX);
	D3DXVec3Cross(&vAxisY, &vAxisZ, &vAxisX);

	D3DXMATRIX mtxRot;
	D3DXMatrixIdentity(&mtxRot);

	mtxRot._11 = vAxisX.x; 
	mtxRot._12 = vAxisX.y; 
	mtxRot._13 = vAxisX.z;
	mtxRot._21 = vAxisY.x; 
	mtxRot._22 = vAxisY.y; 
	mtxRot._23 = vAxisY.z;
	mtxRot._31 = vAxisZ.x; 
	mtxRot._32 = vAxisZ.y; 
	mtxRot._33 = vAxisZ.z;

	// 移動 (始点へ)
	D3DXMATRIX mtxTrans;
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// 行列合成
	m_mtxWorld = mtxScale * mtxRot * mtxTrans;
}
//=========================================================
// 描画処理
//=========================================================
void CMeshCylinder::Draw(void)
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
void CMeshCylinder::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// テクスチャ割り当て
	m_Cylinder.nTexIdx = pTexture->Register("data/TEXTURE/field100.jpg");
}