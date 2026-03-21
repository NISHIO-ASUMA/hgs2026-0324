//=========================================================
//
// 動くキャラクタークラス処理 [ movecharactor.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "movecharactor.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "model.h"
#include "shadowstencil.h"
#include "manager.h"
#include "motionmanager.h"
#include "outline.h"

//=========================================================
// コンストラクタ
//=========================================================
CMoveCharactor::CMoveCharactor(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_move(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_rotDest(VECTOR3_NULL),
m_scale(INITSCALE),
m_pModel{},
m_pMotion{},
m_pShadowS{},
m_fMoveValue(NULL),
m_isStencilUse(false),
m_isOutLine(false),
m_isInstancing(false),
m_mtxworld{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMoveCharactor::~CMoveCharactor()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMoveCharactor::Init(void)
{
	// モデル配列のクリア
	m_pModel.clear();

	// ステンシルフラグ有効時
	if (m_isStencilUse)
	{
		// ステンシル影モデル生成
		m_pShadowS = CShadowStencil::Create(m_pos, m_rot);
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMoveCharactor::Uninit(void)
{
	// 動的確保分のモデルの破棄
	for (auto iter = m_pModel.begin(); iter != m_pModel.end(); iter++)
	{
		// nullチェック
		if ((*iter) != nullptr)
		{
			(*iter)->Uninit();
			delete (*iter);
			(*iter) = nullptr;
		}
	}

	// 配列のクリア
	m_pModel.clear();

	// ポインタ破棄
	m_pMotion.reset();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMoveCharactor::Update(void)
{
	// 移動量の減衰
	m_move.x += (0.0f - m_move.x) * 0.75f;
	m_move.z += (0.0f - m_move.z) * 0.75f;

	// 角度補完
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{// 左回転
		// 角度
		m_rot.y = m_rot.y + D3DX_PI * 2.0f;
	}
	else if (m_rot.y - m_rotDest.y > D3DX_PI)
	{// 右回転
		// 角度
		m_rot.y = m_rot.y - D3DX_PI * 2.0f;
	}

	// 現在の角度
	m_rot.y += (m_rotDest.y - m_rot.y) * 0.25f;

	// ステンシルシャドウの更新
	if (m_pShadowS)
	{
		// オブジェクト設定
		m_pShadowS->SetPos(m_pos);
		m_pShadowS->SetRot(m_rot);
	}

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, m_scale.x, m_scale.y, m_scale.z);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	// ワールド行列を計算する
	m_mtxworld = mtxScal * mtxRot * mtxTrans;

#ifdef NDEBUG
	// モーションとモデルの更新
	if (m_pMotion) 
		m_pMotion->Update(m_pModel);
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CMoveCharactor::Draw(void)
{
	// デバイス取得
	auto Rendere = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = Rendere->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// モデルの描画
	for (auto& model : m_pModel)
	{
		model->Draw();
	}

	// falseなら
	if (!m_isOutLine) return;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// カリングを切る
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// シェーダー開始
	COutLine::GetInstance()->Begin();
	COutLine::GetInstance()->BeginPass();

	// アウトラインシェーダー関数
	for (auto& model : m_pModel)
	{
		model->DrawOutLine();
	}

	// シェーダー終了
	COutLine::GetInstance()->EndPass();
	COutLine::GetInstance()->End();

	// カリングを戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
//=========================================================
// 位置情報の更新
//=========================================================
void CMoveCharactor::UpdatePosition(void)
{
	m_posOld = m_pos;
	m_pos += m_move;
}
//=========================================================
// モーションロード処理
//=========================================================
void CMoveCharactor::MotionLoad(const char* pScriptName, int nDestMotions, const bool isShadow)
{
	// モーションのポインタを取得する
	m_pMotion = CMotion::Load(pScriptName, m_pModel, nDestMotions, isShadow);
}