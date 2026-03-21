//=========================================================
//
// 円形3Dオブジェクト処理 [ circle3d.h ]
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
#include "object3D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CSphereCollider;

//*********************************************************
// 円形3Dオブジェクトクラスを定義
//*********************************************************
class CCircle3D : public CObject3D
{
public:

	CCircle3D(int nPriority = static_cast<int>(CObject::PRIORITY::MOVE3D));
	~CCircle3D();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CSphereCollider* other);

	inline void SetHitRange(float fRange) { m_fHitRange = fRange; }

	inline CSphereCollider* GetSphereCollider(void) const { return m_pSphere.get(); }
	inline float GetfRange(void) const { return m_fHitRange; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="fRadius">判定半径</param>
	/// <returns></returns>
	static CCircle3D* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const float fWidth,
		const float fHeight,
		const float fRadius
	);

private:

	//*************************************
	// 定数構造体
	//*************************************
	struct Config
	{
		static constexpr float HIT_RANGE	 = 80.0f;		 // 判定範囲
		static constexpr const char* TEXNAME = "Circle.png"; // テクスチャ名
	};

	std::unique_ptr<CSphereCollider>m_pSphere; // 球形コライダー
	float m_fHitRange;			// 判定半径
};