//=========================================================
//
// 当たり判定基底クラス処理 [ collision.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// コリジョンクラスを定義
//*********************************************************
class CCollision
{
public:

	//***************************
	// コリジョンの種類定義
	//***************************
	enum TYPE
	{
		TYPE_NONE,		// 無し
		TYPE_BOX,		// 矩形
		TYPE_SPHERE,	// 球
		TYPE_BOXSPHERE, // 球と矩形
		TYPE_MAX
	};

	CCollision(TYPE Type);
	~CCollision();

	inline void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	inline void SetType(TYPE Type) { m_Type = Type; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline TYPE GetType(void) const { return m_Type; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="Type">種類</param>
	/// <returns></returns>
	static CCollision* Create(const D3DXVECTOR3& pos, TYPE Type);

private:
	D3DXVECTOR3 m_pos;	// 判定する座標
	TYPE m_Type;		// 種類
};






