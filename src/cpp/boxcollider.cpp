//=========================================================
//
// 矩形のコライダー処理 [ boxcollider.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "boxcollider.h"

//=========================================================
// コンストラクタ
//=========================================================
CBoxCollider::CBoxCollider() : m_BoxInfo{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CBoxCollider::~CBoxCollider()
{

}
//=========================================================
// 生成処理
//=========================================================
std::unique_ptr<CBoxCollider> CBoxCollider::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& posold, 
	const D3DXVECTOR3& size
)
{
	// インスタンス生成
	auto pCollider = std::make_unique<CBoxCollider>();
	if (pCollider == nullptr) return nullptr;

	// 値をセット
	pCollider->SetPos(pos);
	pCollider->m_BoxInfo.posOld = posold;
	pCollider->m_BoxInfo.Size = size;

	// 生成されたポインタを返す
	return pCollider;
}