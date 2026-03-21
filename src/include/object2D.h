//=========================================================
//
// 2Dオブジェクト基底クラス処理 [ object2D.h ]
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
// オブジェクト2Dクラスを定義
//*********************************************************
class CObject2D : public CObject
{
public:

	//****************************
	// アンカーポイントタイプ
	//****************************
	enum ANCHORTYPE
	{
		ANCHORTYPE_NONE,
		ANCHORTYPE_CENTER,
		ANCHORTYPE_LEFTSIDE,
		ANCHORTYPE_RIGHTSIDE,
		ANCHORTYPE_MAX
	};

	//****************************
	// 描画タイプ
	//****************************
	enum DRAWTYPE
	{
		DRAWTYPE_NONE,
		DRAWTYPE_ANIM,
		DRAWTYPE_MAX,
	};

	CObject2D(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CObject2D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCenter(void);
	void SetLeft(void);
	void SetRight(void);
	void SetTexture(const char* pRegisterName);
	void SetFlash(const int nFirstcount, const int nEndcount, const D3DXCOLOR col);

	inline void SetCol(D3DXCOLOR col) { m_col = col; }
	inline void SetUV(float TexU, float TexV);
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }
	inline void SetWidth(float fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(float fHeight) { m_fWidth = fHeight; }
	inline void SetAnchor(int Type = ANCHORTYPE_CENTER) { m_nAnchorType = Type; }
	inline void SetDrawType(int nType) { m_nDrawType = nType; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

	static CObject2D* Create(void);

private:

	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファのポインタ

	D3DXVECTOR3 m_pos;	// 座標情報
	D3DXVECTOR3 m_rot;	// 角度情報
	D3DXCOLOR m_col;	// 色情報

	int m_nAnchorType;	// 基準座標設定
	int m_nColorCount;	// 点滅カウント
	int m_nDrawType;	// 描画タイプ
	int m_nIdxTexture;	// テクスチャインデックス
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ
};
