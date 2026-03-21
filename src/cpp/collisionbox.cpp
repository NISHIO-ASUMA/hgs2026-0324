//=========================================================
//
// 矩形の当たり判定処理 [ collisionbox.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "collisionbox.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"

//*********************************************************
// 定数空間
//*********************************************************
namespace BOXINFO
{
	constexpr float PUSHVALUE = 0.1f;		// 押し出し加算値
};

//=========================================================
// コンストラクタ
//=========================================================
CCollisionBox::CCollisionBox() : CCollision(CCollision::TYPE_BOX)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CCollisionBox::~CCollisionBox()
{
	
}
//===========================================================
// 矩形同士のコリジョン判定関数
//===========================================================
bool CCollisionBox::Collision(CBoxCollider* thisCollider, CBoxCollider* OtherCollider, D3DXVECTOR3* ExtrusionPos)
{
	//===========================
	// コライダーポインタ情報
	//===========================
	CBoxCollider* pMyCollider = thisCollider;
	CBoxCollider* pOtherCollider = OtherCollider;

	//===========================
	// 自分と相手の座標を取得
	//===========================
	D3DXVECTOR3 MyPos = pMyCollider->GetPos();
	D3DXVECTOR3 OtherPos = pOtherCollider->GetPos();

	//===========================
	// コライダーサイズを取得
	//===========================
	D3DXVECTOR3 MySize = pMyCollider->GetInfo().Size;
	D3DXVECTOR3 OtherSize = pOtherCollider->GetInfo().Size;

	//================================
	// 自分と相手の半分のサイズを取得
	//================================
	D3DXVECTOR3 MyHalfSize = MySize * HALF;
	D3DXVECTOR3 OtherHalfSize = OtherSize * HALF;

	//===============================
	// 最大最小座標を計算する
	//===============================
	D3DXVECTOR3 MyPosMax = MyPos + MyHalfSize;
	D3DXVECTOR3 MyPosMin = MyPos - MyHalfSize;
	D3DXVECTOR3 OtherPosMax = OtherPos + OtherHalfSize;
	D3DXVECTOR3 OtherPosMin = OtherPos - OtherHalfSize;
	
	//===========================
	// 過去の座標を取得
	//===========================
	D3DXVECTOR3 MyPosOldMin = pMyCollider->GetInfo().posOld - MyHalfSize;
	D3DXVECTOR3 MyPosOldMax = pMyCollider->GetInfo().posOld + MyHalfSize;
	D3DXVECTOR3 OtherPosOldMin = pOtherCollider->GetInfo().posOld - OtherHalfSize;
	D3DXVECTOR3 OtherPosOldMax = pOtherCollider->GetInfo().posOld + OtherHalfSize;

	// 高さの範囲内に入って無かったら判定をしない
	const bool isHitRangeY = MyPosOldMin.y <= OtherPosOldMax.y && MyPosOldMax.y >= OtherPosOldMin.y;
	if (!isHitRangeY) return false;

	//==============================
	// z軸の範囲内に入っているとき
	//==============================
	if (MyPosOldMin.z < OtherPosMax.z && MyPosMax.z > OtherPosMin.z)
	{
		if (MyPosOldMax.x <= OtherPosMin.x && MyPosMax.x > OtherPosMin.x)
		{// 左からめり込む

			// 左側への押し出し座標を計算
			ExtrusionPos->x = OtherPosMin.x - MyHalfSize.x - BOXINFO::PUSHVALUE;
			return true;
		}
		else if (MyPosOldMin.x >= OtherPosOldMax.x && MyPosMin.x < OtherPosMax.x)
		{// 右からめり込む
			
			// 右側への押し出し座標を計算
			ExtrusionPos->x = OtherPosMax.x + MyHalfSize.x + BOXINFO::PUSHVALUE;
			return true;
		}
	}
	
	//==============================
	// x軸の範囲内に入っているとき
	//==============================
	if (MyPosOldMin.x < OtherPosMax.x && MyPosMax.x > OtherPosMin.x)
	{
		if (MyPosOldMax.z <= OtherPosMin.z && MyPosMax.z > OtherPosMin.z)
		{// 手前からめり込む

			// 手前への押しだし座標を計算
			ExtrusionPos->z = OtherPosMin.z - MyHalfSize.z - BOXINFO::PUSHVALUE;
			return true;
		}
		else if (MyPosOldMin.z >= OtherPosMax.z && MyPosMin.z < OtherPosMax.z)
		{// 奥から手前にめり込む

			// 奥側に押し出す座標を計算する
			ExtrusionPos->z = OtherPosMax.z + MyHalfSize.z + BOXINFO::PUSHVALUE;
			return true;
		}
	}

	// 未ヒット時
	return false;
}