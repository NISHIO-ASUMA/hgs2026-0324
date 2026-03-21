//=========================================================
//
// 球形当たり判定処理 [ collisionsphere.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "collisionsphere.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "spherecollider.h"

//=========================================================
// コンストラクタ
//=========================================================
CCollisionSphere::CCollisionSphere() : CCollision(CCollision::TYPE_SPHERE)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CCollisionSphere::~CCollisionSphere()
{
	
}
//=========================================================
// 球同士の当たり判定
//=========================================================
bool CCollisionSphere::Collision(CSphereCollider* thisCollider, CSphereCollider* otherCollider)
{
	// コライダーがnullなら
	if (thisCollider == nullptr || otherCollider == nullptr) return false;

	// コライダー情報をセット
	CSphereCollider* MySphere = thisCollider;
	CSphereCollider* OtherSphere = otherCollider;

	// 座標を取得する
	D3DXVECTOR3 MyPos = MySphere->GetPos();
	D3DXVECTOR3 OtherPos = OtherSphere->GetPos();

	// 範囲を取得する
	float fMyRadius = MySphere->GetRadius();
	float fOtherRadius = OtherSphere->GetRadius();

	// 2点の座標の距離差分を算出する
	D3DXVECTOR3 DiffPos = MyPos - OtherPos;
	
	// 長さを取得
	float fDistance = D3DXVec3Length(&DiffPos);
	
	// 半径の合計
	float fRadSum = fMyRadius + fOtherRadius;

	// 半径の合計より近い
	if (fDistance < fRadSum) return true;

	// 未ヒット時
	return false;
}