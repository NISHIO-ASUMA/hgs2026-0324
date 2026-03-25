//=========================================================
//
// ゴール処理 [ goal.h ]
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
#include "objectX.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;
class CBoxCollider;

//*********************************************************
// ゴールクラスを定義
//*********************************************************
class CGoal : public CObjectX
{
public:

	CGoal(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CGoal();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(CBoxCollider* other);

	void SetisGoal(const bool& isGoal) { m_isGoal = isGoal; }
	bool GetIsGoalFlag(void) const { return m_isGoal; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	static CGoal* Create(const D3DXVECTOR3& pos);

private:

	//******************************
	// 定数構造体
	//******************************
	struct Config
	{
		static constexpr float RADIUS = 80.0f; // 当たり半径
		static constexpr const char* MODELNAME = "STAGEOBJ/tornado00.x"; // モデルパス
	};

private:

	std::unique_ptr<CSphereCollider>m_pSphereCollider; // 球形コライダー
	bool m_isGoal;									   // 終了フラグ
};
