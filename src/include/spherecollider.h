//=========================================================
//
// 球形コライダー処理 [ spherecollider.h ]
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
// 球形コライダークラスを定義
//*********************************************************
class CSphereCollider : public CCollider
{
public:

	CSphereCollider();
	~CSphereCollider();

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius(void) { return m_fRadius; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fRadius">当たる半径</param>
	/// <returns></returns>
	static std::unique_ptr<CSphereCollider>Create
	(
		const D3DXVECTOR3& pos, 
		const float& fRadius
	);

private:

	float m_fRadius; // 当たる半径
};

