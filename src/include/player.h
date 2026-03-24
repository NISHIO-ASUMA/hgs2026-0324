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
class CInputKeyboard;
class CJoyPad;

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
		//MOVE,			// キー移動
		//WALLACTION,	// 壁アクション
		//LANDING,		// 着地
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

	bool CollisionBox(CBoxCollider* pOther, D3DXVECTOR3 * pOutPos);

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
		static constexpr float MOVESPEED = 2.5f;		// 移動速度固定値
		static constexpr float COLLISION = 15.0f;		// コリジョン半径
		static constexpr float NorRot = D3DX_PI * 2.0f;	// 回転補正
		static constexpr const char* SCRIPT = "data/MOTION/testMotion.txt";
	};

private:

	std::unique_ptr<CBoxCollider>m_pBoxCollder; // 矩形コライダー

	bool m_isWall;								// 壁張り付きかどうか
	bool m_isLanding;							// 着地
	bool m_isJump;								// ジャンプ
};