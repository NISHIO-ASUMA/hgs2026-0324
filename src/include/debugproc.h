//=========================================================
//
// デバッグプロセス処理 [ debubgproc.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// デバッグプロセスクラスを定義
//*********************************************************
class CDebugproc
{
public:

	CDebugproc();
	~CDebugproc();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static void Draw(int PosX, int PosY,const D3DXCOLOR& color = COLOR_WHITE);
	static void Print(const char* fmt, ...);

private:

	static constexpr int FONTSIZE = 20;		// フォントサイズ

	static LPD3DXFONT m_pFont;				// デバッグフォント
	static char m_aStr[MAX_WORD];			// 文字列格納バッファ
	static bool m_isUse;					// 使用しているかフラグ
};