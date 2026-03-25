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

//=========================================================
// コンストラクタ
//=========================================================
CWallTargetPoint::CWallTargetPoint(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr)
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
#ifdef _DEBUG
	CObjectX::Draw();
#endif // _DEBUG
}