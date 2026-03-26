//=========================================================
//
// 敵の処理 [ enemy.h ]
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

//*********************************************************
// 敵クラスを定義
//*********************************************************
class CEnemy : public CObjectX
{
public:

	//*******************************
	// 敵の種類
	//*******************************
	enum TYPE
	{
		ANT,		// アリ 1
		OTAMA,		// オタマジャクシ 3
		BUTTERFLY,	// 蝶 5
		SPIDER,		// 蜘蛛 10
		MAX
	};

	CEnemy(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DecLife(const int& nDamage);
	void SetType(const int& nType) { m_nType = nType; }

	bool Collision(CSphereCollider* pOther);
	inline CSphereCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CEnemy* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:

	//******************************
	// 定数構造体
	//******************************
	struct Config
	{
		static constexpr int LIFE = 1;
		static constexpr float ROT = 0.03f;
	};

	std::unique_ptr<CSphereCollider> m_pCollider;	// 矩形のコライダー
	int m_nLife;									// 体力
	int m_nType;									// 種類
};

