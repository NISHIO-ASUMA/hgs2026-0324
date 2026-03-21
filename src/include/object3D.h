//=========================================================
//
// 3Dオブジェクト基底クラス処理 [ object3D.h ]
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
// オブジェクト3Dクラスを定義
//*********************************************************
class CObject3D : public CObject
{ 
public:

	CObject3D(int nPriority = static_cast<int>(CObject::PRIORITY::BASENUMBER));
	~CObject3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTexture(const char * pTexName);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetCol(D3DXCOLOR col = COLOR_WHITE) { m_col = col;}
	inline void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

	static CObject3D* Create(const D3DXVECTOR3& pos);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ

	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;	// 座標情報
	D3DXVECTOR3 m_rot;	// 角度情報
	D3DXCOLOR m_col;	// 色情報

	int m_nIdxTexture;	// テクスチャインデックス
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ
};