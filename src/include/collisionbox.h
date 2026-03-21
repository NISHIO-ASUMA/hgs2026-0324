//=========================================================
//
// 矩形の当たり判定処理 [ collisionbox.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "collision.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// 矩形の当たり判定をするクラスを定義
//*********************************************************
class CCollisionBox : public CCollision
{
public:

	CCollisionBox();
	~CCollisionBox();

	/// <summary>
	/// 矩形同士の当たり判定を実行する
	/// </summary>
	/// <param name="thisCollider">自分が持ってる矩形コライダー</param>
	/// <param name="OtherCollider">判定先の矩形コライダー</param>
	/// <param name="ExtrusionPos">ヒット時の押し出し座標</param>
	/// <returns></returns>
	static bool Collision
	(
		CBoxCollider* thisCollider, 
		CBoxCollider* OtherCollider,
		D3DXVECTOR3 * ExtrusionPos
	);

private:
};