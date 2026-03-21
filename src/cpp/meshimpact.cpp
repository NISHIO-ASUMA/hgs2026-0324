//=========================================================
//
// メッシュインパクト処理 [ meshimpact.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "meshimpact.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "debugproc.h"
#include "effect.h"
#include "gamemanager.h"

//*********************************************************
// 定数宣言
//*********************************************************
namespace IMPACTINFO
{
	constexpr int NUM_X = 30; // X平面の分割数
	constexpr int NUM_Z = 1;  // Z平面の分割数
	constexpr int VERTEX = (NUM_X + 1) * (NUM_Z + 1); // 頂点数
	constexpr int PRIM = (VERTEX - 2);		// ポリゴン数
	constexpr int INDEX = (VERTEX + 2);		// インデックスバッファ数
	constexpr float HEIGHTSIZE = 3.0f;		// 上昇速度
}

//=========================================================
// コンストラクタ
//=========================================================
CMeshImpact::CMeshImpact(int nPriority) : CObject(nPriority),
m_pIdx(nullptr),
m_pVtx(nullptr),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_col(D3DCOLOR_RGBA(0, 185, 46, 255)),
m_MeshImpact{},
m_mtxWorld{}
{
}
//=========================================================
// デストラクタ
//=========================================================
CMeshImpact::~CMeshImpact()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CMeshImpact* CMeshImpact::Create(const D3DXVECTOR3& pos, const int& nLife, const float& fRadius, const float& fInRadius, const float& fSpeed)
{
	// インスタンス生成
	CMeshImpact* pMesh = new CMeshImpact;
	if (pMesh == nullptr) return nullptr;

	pMesh->m_pos = pos;								// 座標
	pMesh->m_MeshImpact.fInRadius = fInRadius;		// 内径
	pMesh->m_MeshImpact.fOutRadius = fRadius;	// 外径
	pMesh->m_MeshImpact.nLife = nLife;				// 継続時間
	pMesh->m_MeshImpact.fSpeed = fSpeed;			// 拡散速度
	pMesh->m_MeshImpact.DecAlpha = pMesh->m_col.a / nLife; // αの減少値

	pMesh->SetObjType(TYPE_MESH);					// オブジェクトのタイプを設定

	// 初期化失敗
	if (FAILED(pMesh->Init()))	return nullptr;

	return pMesh;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMeshImpact::Init(void)
{
	// テクスチャ座標計算
	float fTex = 1.0f / IMPACTINFO::NUM_X;

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * IMPACTINFO::VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtx,
		NULL);

	// 頂点バッファのポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点カウント用の変数
	int nCnt = 0;

	//頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラーの設定
	D3DXCOLOR col = m_col;

	// 座標代入
	D3DXVECTOR3 pos = m_pos;

	// 外側の半径
	float OutRadius = m_MeshImpact.fOutRadius;

	// 外径の計算
	for (int nCntOut = 0; nCntOut <= IMPACTINFO::NUM_X; nCntOut++)
	{
		// 角度を計算
		float fAngel = (D3DX_PI * 2.0f) / IMPACTINFO::NUM_X * nCntOut;

		// 頂点座標の設定
		pVtx[nCnt].pos = D3DXVECTOR3(sinf(fAngel) * OutRadius,2.0f,cosf(fAngel) * OutRadius);

		// 法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[nCnt].col = col;

		// テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(fTex * nCntOut, 1.0f);

		// 加算
		nCnt++;
	}

	// 内側の半径
	float InRadius = m_MeshImpact.fInRadius;

	// 内径の計算
	for (int nCntIn = 0; nCntIn <= IMPACTINFO::NUM_X; nCntIn++)
	{
		// 角度計算
		float fAngel = (D3DX_PI * 2.0f) / IMPACTINFO::NUM_X * nCntIn;

		// 頂点座標の設定
		pVtx[nCnt].pos = D3DXVECTOR3(sinf(fAngel) * InRadius,2.0f,cosf(fAngel) * InRadius);

		// 法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[nCnt].col = col;

		// テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(fTex * nCntIn, 1.0f);

		// 加算
		nCnt++;
	}

	// 頂点バッファのアンロック
	m_pVtx->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * IMPACTINFO::INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdx,
		NULL);

	// インデックスバッファへのポインタ
	WORD* pIdx;

	//インデックスバッファのロック
	m_pIdx->Lock(0, 0, (void**)&pIdx, 0);

	// X方向への分割数
	WORD IndxNum = IMPACTINFO::NUM_X + 1;

	// 配列
	WORD IdxCnt = 0;

	// 格納用変数
	WORD Num = 0;

	// インデックスの設定
	for (int IndxCount1 = 0; IndxCount1 < IMPACTINFO::NUM_Z; IndxCount1++)
	{
		for (int IndxCount2 = 0; IndxCount2 <= IMPACTINFO::NUM_X; IndxCount2++, IndxNum++, Num++)
		{
			pIdx[IdxCnt] = IndxNum;
			pIdx[IdxCnt + 1] = Num;
			IdxCnt += 2;
		}

		// 最後の行じゃなかったら
		if (IndxCount1 < IMPACTINFO::NUM_Z - 1)
		{
			pIdx[IdxCnt] = Num - 1;
			pIdx[IdxCnt + 1] = IndxNum;
			IdxCnt += 2;
		}
	}

	// インデックスバッファのアンロック
	m_pIdx->Unlock();

	// 結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMeshImpact::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtx != nullptr)
	{
		// 解放,nullptr化
		m_pVtx->Release();
		m_pVtx = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdx != nullptr)
	{
		// 解放,nullptr化
		m_pIdx->Release();
		m_pIdx = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMeshImpact::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点計算変数
	int nCntVertex = NULL;

	// 速度を加算,だんだん広げる
	m_MeshImpact.fOutRadius += m_MeshImpact.fSpeed;
	m_MeshImpact.fInRadius += m_MeshImpact.fSpeed;

	//頂点バッファをロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// 座標代入
	D3DXVECTOR3 pos = m_pos;

	// X座標方向への分割数 (外側)
	for (int nCntImpact = 0; nCntImpact <= IMPACTINFO::NUM_X; nCntImpact++)
	{
		// 角度の計算
		float fAngel = (D3DX_PI * 2.0f) / IMPACTINFO::NUM_X * nCntImpact;

		// 頂点座標の設定
		pVtx[nCntVertex].pos = D3DXVECTOR3(
			sinf(fAngel) * m_MeshImpact.fOutRadius,
			3.0f,
			cosf(fAngel) * m_MeshImpact.fOutRadius);

		// カラーの設定
		pVtx[nCntVertex].col = m_col;

		// 頂点カウントをインクリメント
		nCntVertex++;
	}

	//  X座標方向への分割数 (内側)
	for (int nCntImpact = 0; nCntImpact <= IMPACTINFO::NUM_X; nCntImpact++)
	{
		// 角度を計算
		float fAngel = (D3DX_PI * 2.0f) / IMPACTINFO::NUM_X * nCntImpact;

		// 頂点座標の設定
		pVtx[nCntVertex].pos = D3DXVECTOR3(
			sinf(fAngel) * m_MeshImpact.fInRadius,
			3.0f,
			cosf(fAngel) * m_MeshImpact.fInRadius);

		// 頂点カラーの設定
		pVtx[nCntVertex].col = m_col;

		// 頂点カウントをインクリメント
		nCntVertex++;
	}

	// 頂点バッファのアンロック
	m_pVtx->Unlock();

	// 寿命を減らす
	m_MeshImpact.nLife--;
	m_col.a -= m_MeshImpact.DecAlpha;

	// 寿命が尽きた
	if (m_MeshImpact.nLife <= 0)
	{
		// 未使用にする
		Release();

		// 処理を返す
		return;
	}

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
void CMeshImpact::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Zテストを適用
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// αブレンディングの加算合成
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdx);

	// テクスチャフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, IMPACTINFO::VERTEX, 0, IMPACTINFO::PRIM);

	// テクスチャを戻す
	pDevice->SetTexture(0, nullptr);

	// Zテストを戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// αブレンディングを戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

#ifdef _DEBUG
	// デバッグフォント
	CDebugproc::Print("衝撃波の座標 { %.2f,%.2f,%.2f }", m_pos.x, m_pos.y, m_pos.z);
	CDebugproc::Draw(1000, 180);
#endif // _DEBUG
}
//=========================================================
// 当たり判定処理
//=========================================================
bool CMeshImpact::Collision(D3DXVECTOR3* pPos)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// フラグ変数
	bool isHit = false;

	// 頂点をロック
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// X座標の分割数で見る
	for (int nCnt = 0; nCnt < IMPACTINFO::NUM_X; nCnt++)
	{
		// インパクト頂点のワールド座標とのXZ距離を計算
		float dx = pPos->x - (m_pos.x + pVtx[nCnt].pos.x);
		float dy = pPos->y - (m_pos.y + pVtx[nCnt].pos.y);
		float dz = pPos->z - (m_pos.z + pVtx[nCnt].pos.z);

		// 半径の差分をとる
		float fDisSize = (m_MeshImpact.fOutRadius - m_MeshImpact.fInRadius);

		// Y方向で外れてるなら当たらない
		if (dy > IMPACTINFO::HEIGHTSIZE)
		{
			continue;
		}

		// XZ平面の範囲計算
		float fDisVerTexXZ = sqrtf(dx * dx + dz * dz);

		// 差分が小さくなったら
		if (fDisVerTexXZ <= fDisSize)
		{
			// 当たっている
			isHit = true;
		}
	}

	// 頂点バッファをアンロック
	m_pVtx->Unlock();

	// 判定結果を返す
	return isHit;
}