//=========================================================
//
// プレイヤー処理 [ player.h ]
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
#include "movecharactor.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;
class CInputKeyboard;
class CJoyPad;
class CEffectLaser;
class CMeshCylinder;
class CWallTargetPoint;

//*********************************************************
// プレイヤークラスを定義
//*********************************************************
class CPlayer : public CMoveCharactor
{
public:

	//******************************
	// モーション列挙型
	//******************************
	enum MOTION
	{
		NEUTRAL,		// ニュートラル
		MOVE,			// キー移動
		WALLACTION,		// 壁アクション
		JUMP,			// ジャンプ
		LANDING,		// 着地
		MAX
	};

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void KeyMove(void);
	void KeyPad(void);

	// 当たり判定
	bool CollisionBox(CBoxCollider* pOther, D3DXVECTOR3 * pOutPos);
	bool CollisionWorldBox(CSphereCollider* pOther);
	bool CollisionSphere(CSphereCollider* pOther);

	void PlayAction(void);
	void ActionSetting(const D3DXVECTOR3& pos);
	void SerachTarget();

	CBoxCollider* GetCollider(void) const { return m_pBoxCollder.get(); }
	CBoxCollider* GetWorldCollider(void) const { return m_pWorldBoxCollder.get(); }
	CSphereCollider* GetSphere(void) const { return m_pSphereCollider.get(); }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPlayer* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:

	//******************************
	// 定数構造体
	//******************************
	struct Config
	{
		static constexpr float MOVESPEED = 6.0f;			// 移動速度固定値
		static constexpr float COLLISION = 20.0f;			// コリジョン半径
		static constexpr float WORLDCOLLISION = 2000.0f;		// 世界とのコリジョン半径
		static constexpr float SPHERECOLLISION = 60.0f;		// コリジョン半径

		static constexpr float JUMP = 15.0f;				// ジャンプ量
		static constexpr float NorRot = D3DX_PI * 2.0f;		// 回転補正
		static constexpr const char* SCRIPT = "data/MOTION/Flog_Motion.txt";// モーションファイル名
	};

	//******************************
	// 定数構造体
	//******************************
	struct Action
	{
		static constexpr float AUTOSPEED = 12.0f;	 // 移動速度固定値
		static constexpr float CheckDistance = 7.0f; // 判定最小値
		static constexpr float CYLINDER = 3.0f;		 // 生成半径
	};

private:

	std::unique_ptr<CBoxCollider>m_pBoxCollder;			// 矩形コライダー
	std::unique_ptr<CBoxCollider>m_pWorldBoxCollder;	// 世界との当たるボックスを作成
	std::unique_ptr<CSphereCollider>m_pSphereCollider;	// 球形コライダー

	bool m_isWall;								// 壁張り付きかどうか
	bool m_isLanding;							// 着地したかどうか
	bool m_isJump;								// ジャンプ
	bool m_isStayPos;							// ステイ中か


	D3DXVECTOR3 m_TargetPos;					// 目的座標設定用の入れ物
	CEffectLaser* m_pLaser;						// レーザーエフェクト
	CMeshCylinder* m_pCylinder;					// メッシュシリンダー

	//**************************
	// ワープ関係
	//**************************
	int m_AroundTargetCount;					// ターゲットの取得インデックス
	std::vector<CWallTargetPoint*> m_pNearbyTargets; // 範囲内のターゲット一時保存用
	int m_SelectIndex = 0;						// 現在何番目を選択中か
};