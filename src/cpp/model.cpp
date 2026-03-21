//=========================================================
//
// モデル処理 [ model.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "model.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "modelmanager.h"
#include "outline.h"

//=========================================================
// コンストラクタ
//=========================================================
CModel::CModel() : m_pParent(nullptr),
m_pTexture(nullptr),
m_nModelIdx(-1),
m_pos(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_offPos(VECTOR3_NULL),
m_offRot(VECTOR3_NULL),
m_scale(INITSCALE),
m_parttype(PARTTYPE_NONE),
m_isShadow(false),
m_Isinstancing(false),
m_mtxworld{}
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CModel::~CModel()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CModel::Init
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot,
	const char * pFilename,
	const bool isShadow
)
{
	// モデルセット
	SetModelPass(pFilename);

	// 位置の設定
	m_pos = pos;
	m_rot = rot;

	// オフセット位置を設定
	m_offPos = pos;
	m_offRot = rot;

	// フラグを設定する
	SetMtxShadow(isShadow);

	// 結果を返す
	return S_OK;
}
//=========================================================
// 生成処理
//=========================================================
CModel* CModel::Create
(
	const D3DXVECTOR3 &pos, 
	const D3DXVECTOR3 &rot, 
	const char* pFilename,
	const bool isShadow
)
{
	// インスタンス生成
	CModel* pModel = new CModel;
	if (pModel == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pModel->Init(pos, rot, pFilename,isShadow))) return  nullptr;

	// ポインタを返す
	return pModel;
}
//=========================================================
// 終了処理
//=========================================================
void CModel::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CModel::Update(void)
{

}
//=========================================================
// 描画処理
//=========================================================
void CModel::Draw(void)
{
	// インデックスが-1なら
	if (m_nModelIdx == -1)
		return;

	// デバイスポインタを宣言
	auto Rendere = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Rendere->GetDevice();

	// ファイルマネージャー取得
	CModelManager* pXMgr = CManager::GetInstance()->GetModelManagere();
	if (!pXMgr) return;

	// 配列情報
	auto& fileData = pXMgr->GetList();
	if (m_nModelIdx >= static_cast<int>(fileData.size())) return;

	// 配列
	auto& model = fileData[m_nModelIdx];
	if (!model.pMesh) return;

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_offRot.y, m_rot.x + m_offRot.x, m_rot.z + m_offRot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_offPos.x, m_pos.y + m_offPos.y, m_pos.z + m_offPos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// 親のペアネント格納用変数
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// 親が存在する
		// ワールドマトリックス取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// マトリックス取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のマトリックスとかけ合わせる
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// 現在のマテリアル
	pDevice->GetMaterial(&matDef);

	// マテリアルが取得できたら
	if (model.pBuffMat)
	{
		// マテリアルデータのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

		// テクスチャ取得
		CTexture* pTex = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// マテリアルのセット
			pDevice->SetMaterial(&pMat[nCnt].MatD3D);

			// テクスチャ取得
			int texIdx = model.pTexture[nCnt];

			// テクスチャセット
			pDevice->SetTexture(0, (texIdx >= 0) ? pTex->GetAddress(texIdx) : nullptr);

			// モデルの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// 有効なら
	if (m_isShadow) DrawMtxShadow();
}
//=========================================================
// マトリックスシャドウ描画処理
//=========================================================
void CModel::DrawMtxShadow(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return;

	// Xファイルマネージャー取得
	CModelManager* pMgr = CManager::GetInstance()->GetModelManagere();
	if (!pMgr) return;

	// 配列情報の取得
	auto& fileData = pMgr->GetList();
	if (m_nModelIdx < NULL || m_nModelIdx >= static_cast<int>(fileData.size())) return;

	// モデル要素を取得
	auto& model = fileData[m_nModelIdx];
	if (!model.pMesh) return;

	// ライト方向
	D3DXVECTOR4 lightDir(0.0f, -0.63f, -0.02f, 0.0f);

	// 平面投影座標を設定
	D3DXPLANE plane;
	D3DXVECTOR3 point = D3DXVECTOR3(0.0f, 0.7f, 0.0f);
	D3DXVECTOR3 normal = D3DXVECTOR3(0.0f, -0.7f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &point, &normal);

	// 影マトリックス生成
	D3DXMATRIX mtxShadow;
	D3DXMatrixShadow(&mtxShadow, &lightDir, &plane);

	// 影をモデルの位置に合わせる
	D3DXMATRIX mtxWorldShadow;
	D3DXMatrixMultiply(&mtxWorldShadow, &m_mtxworld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorldShadow);

	// 半透明に設定
	D3DMATERIAL9 shadowMat = {};
	shadowMat.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.35f);
	shadowMat.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.35f);

	// マテリアルセット
	pDevice->SetMaterial(&shadowMat);

	// メッシュ描画
	for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
	{
		model.pMesh->DrawSubset(nCnt);
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&shadowMat);
}
//=========================================================
// アウトライン描画設定関数
//=========================================================
void CModel::DrawOutLine(const D3DXVECTOR4& color,const float fOutLinewidth)
{
	// インデックスが-1なら
	if (m_nModelIdx == -1)
		return;

	// ファイルマネージャー取得
	CModelManager* pXMgr = CManager::GetInstance()->GetModelManagere();
	if (!pXMgr) return;

	// 配列情報
	auto& fileData = pXMgr->GetList();
	if (m_nModelIdx >= static_cast<int>(fileData.size())) return;

	// 配列
	auto& model = fileData[m_nModelIdx];
	if (!model.pMesh) return;

	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_offRot.y, m_rot.x + m_offRot.x, m_rot.z + m_offRot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_offPos.x, m_pos.y + m_offPos.y, m_pos.z + m_offPos.z);

	// 各行列の計算
	m_mtxworld = mtxScale * mtxRot * mtxTrans;
	
	// 親のペアネント格納用変数
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// 親が存在する
		// ワールドマトリックス取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// マトリックス取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のマトリックスとかけ合わせる
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxParent);

	// マテリアルが取得できたら
	if (model.pBuffMat)
	{
		for (int nCnt = 0; nCnt < static_cast<int>(model.dwNumMat); nCnt++)
		{
			// シェーダーパラメーター設定
			COutLine::GetInstance()->SetParameter(fOutLinewidth, color, m_mtxworld);

			// モデルの描画
			model.pMesh->DrawSubset(nCnt);
		}
	}
}
//=========================================================
// モデルインデックス登録
//=========================================================
void CModel::SetModelPass(const char* pModelName)
{
	// モデルマネージャーからモデルの情報取得
	auto ModelManager = CManager::GetInstance()->GetModelManagere();
	if (ModelManager == nullptr) return;

	// インデックスセット
	m_nModelIdx = ModelManager->Register(pModelName);
}
//=========================================================
// 親パーツ設定処理
//=========================================================
void CModel::SetParent(CModel* pModel)
{
	// 親モデルの設定
	m_pParent = pModel;
}