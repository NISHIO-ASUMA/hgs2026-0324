//=========================================================
//
// 数字処理 [ number.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// ナンバークラスを定義
//*********************************************************
class CNumber
{
public:

	CNumber();
	~CNumber();

	HRESULT Init(D3DXVECTOR3 pos, float fwidth, float fheight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexture(const char * pTexName);
	void SetSize(float fWidth, float fHeight);
	void SetDigit(int nDigit);
	void SetCol(const D3DXCOLOR& col);
	void SetFlash(const int& nStartFrame, const int& nEndFrame, const D3DXCOLOR& col);

	inline void SetIsUse(const bool isuse) { m_isUse = isuse; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos;}

	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }
	inline bool GetIsUse(void) const { return m_isUse; }

private:

	//********************************
	// 定数構造体宣言
	//********************************
	struct Config
	{
		static constexpr float DIGIT_VALUE	= 0.1f;	// テクスチャ分割数割合
		static constexpr float END_FLOAT	= 1.0f;	// 補完割合値
		static constexpr float RATIO		= 2.0f;	// 補完率
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファのポインタ
	D3DXVECTOR3 m_pos;					// 座標情報
	D3DXCOLOR m_col;					// 色情報
	float m_fWidth, m_fHeight;			// 横幅,高さ
	float m_TexU, m_TexU1,m_TexV;		// テクスチャ座標
	int n_nColorCount;					// 色変更カウント
	int m_nIdxTexture;					// テクスチャインデックス
	bool m_isUse;						// 使用フラグ
};