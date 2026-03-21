//=========================================================
//
// 矩形と球の当たり判定クラス処理 [ boxtospherecollision.h ]
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
//	前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;

//*********************************************************
// 矩形と球のあたり判定クラスを定義
//*********************************************************
class CBoxToSphereCollision :public CCollision
{
public:

	CBoxToSphereCollision() : CCollision(TYPE_BOXSPHERE) {} ;
	~CBoxToSphereCollision() = default;

	/// <summary>
	/// 矩形コライダーと球形コライダーの当たり判定を実行する関数
	/// </summary>
	/// <param name="pthis">自身の矩形コライダー</param>
	/// <param name="pother">判定先の球形コライダー</param>
	/// <returns></returns>
	static bool Collision(CBoxCollider* pthis, CSphereCollider* pother);

private:
};