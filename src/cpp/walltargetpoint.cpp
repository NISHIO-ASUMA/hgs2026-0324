//=========================================================
//
// ワープターゲットのポイントクラス [ walltargetpoint.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "walltargetpoint.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"
#include "lockonui.h"

//=========================================================
// コンストラクタ
//=========================================================
CWallTargetPoint::CWallTargetPoint(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr),
m_pLockOnUi(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CWallTargetPoint::~CWallTargetPoint()
{

}
//=========================================================
// 生成処理
//=========================================================
CWallTargetPoint* CWallTargetPoint::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const D3DXVECTOR3& scale, 
	const char* pModelName
)
{
	CWallTargetPoint* pPoint = new CWallTargetPoint;
	if (pPoint == nullptr) return nullptr;

	// オブジェクト設定
	pPoint->SetPos(pos);
	pPoint->SetRot(rot);
	pPoint->SetScale(scale);
	pPoint->SetFilePass(pModelName);

	// 初期化失敗時
	if (FAILED(pPoint->Init())) return nullptr;

	return pPoint;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWallTargetPoint::Init(void)
{
	CObjectX::Init();

	// コライダー生成
	m_pCollider = CSphereCollider::Create(GetPos(), 600.0f);

	// ui生成
	m_pLockOnUi = CLockOnUi::Create(GetPos(), VECTOR3_NULL, D3DXVECTOR2(60.0f,60.0f), "crosshair.png");

	// サイズ変更
	SetScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f));

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWallTargetPoint::Uninit(void)
{
	m_pCollider.reset();

	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CWallTargetPoint::Update(void)
{
	auto pos = GetPos();

	if (m_pCollider)
	{
		m_pCollider->SetPos(pos);
	}

	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CWallTargetPoint::Draw(void)
{
	CObjectX::Draw();
}