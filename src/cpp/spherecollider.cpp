//=========================================================
//
// 球形コライダー処理 [ spherecollider.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "spherecollider.h"

//=========================================================
// コンストラクタ
//=========================================================
CSphereCollider::CSphereCollider() : m_fRadius(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CSphereCollider::~CSphereCollider()
{

}
//=========================================================
// 球形コライダー生成処理
//=========================================================
std::unique_ptr<CSphereCollider>CSphereCollider::Create
(
	const D3DXVECTOR3& pos, 
	const float& fRadius
)
{
	// インスタンス生成
	auto pSphere = std::make_unique<CSphereCollider>();
	if (pSphere == nullptr) return nullptr;

	// 値をセット
	pSphere->SetPos(pos);
	pSphere->m_fRadius = fRadius;

	// 生成されたポインタ
	return pSphere;
}