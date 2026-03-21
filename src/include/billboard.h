//=========================================================
//
// ビルボード処理 [ billboard.h ]
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
#include "object.h"

//*********************************************************
// ビルボードクラスを定義
//*********************************************************
class CBillboard : public CObject
{
public:

	CBillboard(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CBillboard();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Flash(const int nMaxFlashTime,const int Digittime);
	void SetTexture(const char* pTexName);
	void SetAnim(const int nMaxPattern, const int nMaxAnimCount, float fTexU, float fTexV);
	void SetUV(float fTexU, float fTexU1, float fTexV);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetCol(D3DXCOLOR col = COLOR_WHITE) { m_col = col; }
	inline void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }
	inline void SetEnableZtest(bool isflags) { m_isTests = isflags; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; };
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; };
	inline D3DXCOLOR GetCol(void) const { return m_col; };
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成処理</param>
	/// <param name="rot">角度</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pTexName">テクスチャファイル名</param>
	/// <returns></returns>
	static CBillboard* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		float fWidth, 
		float fHeight, 
		const char* pTexName
	);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_rot;	// 角度情報
	D3DXVECTOR3 m_pos;	// 座標情報
	D3DXCOLOR m_col;	// 色情報
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス

	int m_nIdxTexture;			// テクスチャインデックス
	int m_FlashCount;			// 点滅間隔
	int m_nCountAnim;			// アニメ―ション間隔
	int m_nPatterAnim;			// アニメ―ションパターン
	float m_fHeight, m_fWidth;	// 横幅,高さ
	bool m_isTests;				// Zテストするかどうか
};