//=========================================================
//
// エフェクト処理 [ effect.h ]
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
#include "billboard.h"

//*********************************************************
// エフェクトクラスを定義
//*********************************************************
class CEffect : public CBillboard
{
public:

	CEffect(int nPriority = static_cast<int>(CObject::PRIORITY::EFFECT));
	~CEffect();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetLife(int nLife) { m_nLife = nLife; }
	int GetLife(void) { return m_nLife; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="col">色</param>
	/// <param name="move">移動量</param>
	/// <param name="nLife">寿命</param>
	/// <param name="fRadius">半径</param>
	/// <returns></returns>
	static CEffect* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXCOLOR& col,
		const D3DXVECTOR3& move,
		const int& nLife,
		const float& fRadius
	);

private:

	//******************************
	// 定数構造体
	//******************************
	struct Config
	{
		static constexpr const char* TEXNAME = "effect.jpg"; // テクスチャパス
		static constexpr float DecPaw = 0.005f;				 // 減算値
	};

	int m_nLife;		// 寿命
	float m_fRadius;	// 半径
	D3DXVECTOR3 m_move;	// 移動量
};