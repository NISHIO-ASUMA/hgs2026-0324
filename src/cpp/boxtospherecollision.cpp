//===================================================================
//
// 矩形と球の当たり判定クラス処理 [ boxtospherecollision.cpp ]
// Author: Asuma Nishio
// 
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "boxtospherecollision.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "boxcollider.h"
#include "spherecollider.h"

//===================================================================
// 矩形と球の当たり判定処理
//===================================================================
bool CBoxToSphereCollision::Collision(CBoxCollider* pthis, CSphereCollider* pother)
{
	if (pthis == nullptr || pother == nullptr) return false;

	// 位置とサイズ
	D3DXVECTOR3 thisPos = pthis->GetPos();
	D3DXVECTOR3 thisSize = pthis->GetInfo().Size * HALF;
	D3DXVECTOR3 spherePos = pother->GetPos();
	float sphereRadius = pother->GetRadius();

	// AABBの最小最大座標
	D3DXVECTOR3 boxMin = thisPos - thisSize;
	D3DXVECTOR3 boxMax = thisPos + thisSize;

	// 球の中心からAABB上の最近点を求める
	D3DXVECTOR3 closest;
	closest.x = max(boxMin.x, min(spherePos.x, boxMax.x));
	closest.y = max(boxMin.y, min(spherePos.y, boxMax.y));
	closest.z = max(boxMin.z, min(spherePos.z, boxMax.z));

	// 最近点と球の中心の距離を求める
	D3DXVECTOR3 diff = spherePos - closest;
	float dist = D3DXVec3Length(&diff);

	if (dist <= sphereRadius)
	{
		return true;
	}

	return false;
}
