//=========================================================
//
// 矩形のコライダー処理 [ boxcollider.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "collider.h"

//*********************************************************
// 矩形のコライダー設定処理
//*********************************************************
class CBoxCollider : public CCollider
{
public:

	//*********************************
	// ボックスコライダー変数構造体
	//*********************************
	struct BoxColliderInfo
	{
		D3DXVECTOR3 posOld;// 前回の座標
		D3DXVECTOR3 Size;  // コライダーの大きさ
	};
	
	CBoxCollider();
	~CBoxCollider();

	void SetPosOld(const D3DXVECTOR3& posold) { m_BoxInfo.posOld = posold; }
	void SetColliderSize(const D3DXVECTOR3& size);
	BoxColliderInfo GetInfo(void) const { return m_BoxInfo; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="posold">1フレーム前の過去座標</param>
	/// <param name="size">コリジョンサイズ</param>
	/// <returns></returns>
	static std::unique_ptr<CBoxCollider>Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& posold, 
		const D3DXVECTOR3& size
	);

private:

	BoxColliderInfo m_BoxInfo; // 構造体変数
};