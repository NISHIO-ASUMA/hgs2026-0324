//===================================================================
//
// アニメーションする2Dオブジェクト処理 [ animationobject2d.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once 

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "object.h"

//*******************************************************************
// アニメーションする2Dオブジェクトの基底クラスを定義
//*******************************************************************
class CAnimationObject2D : public CObject
{
public:

	CAnimationObject2D(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CAnimationObject2D();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetFlash(const int nFirstcount, const int nEndcount, const D3DXCOLOR& col);
	void SetTexture(const char* pRegisterName);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetCol(const D3DXCOLOR& col) { m_col = col; }
	inline void SetSize(const float fWidth, const float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }
	inline void SetUV(const float TexU, const float TexV) { m_fTexU = TexU,m_fTexV = TexV; }
	inline void SetHeight(const float fHeight) { m_fWidth = fHeight; }
	inline void SetAnimFlag(const bool isLoop) { m_isLoop = isLoop; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }		// 座標を取得
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }		// 角度を取得
	inline D3DXCOLOR GetCol(void) const { return m_col; }		// 色を取得
	inline float GetWidth(void) const { return m_fWidth; }		// 横幅を取得
	inline float GetHeight(void) const { return m_fHeight; }	// 高さを取得

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pTexName">テクスチャ名</param>
	/// <returns></returns>
	static CAnimationObject2D* Create
	(
		const D3DXVECTOR3& pos,
		const float fWidth,
		const float fHeight,
		const char * pTexName
	);

private:

	//*******************************
	// 定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float LERP_MIN = 1.0f;
		static constexpr float LERP_MAX = 2.0f;
	};

	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファのポインタ

	D3DXVECTOR3 m_pos;	// 座標情報
	D3DXVECTOR3 m_rot;	// 角度情報
	D3DXCOLOR m_col;	// 色情報

	int m_nColorCount;	// 点滅カウント
	int m_nIdxTexture;	// テクスチャインデックス
	int m_nAnimCount;	// アニメーションカウント
	int m_nAnimPattren; // アニメーションパターン
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ
	float m_fTexV;		// テクスチャV
	float m_fTexU;		// テクスチャU
	bool m_isLoop;		// ループアニメーションフラグ
};