//========================================================
//
// UI処理 [ ui.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//*********************************************************
// UIクラスを定義
//*********************************************************
class CUi : public CObject2D
{
public:

	CUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ポインタ生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="nFlashFrame">点滅間隔</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="Filename">テクスチャ名</param>
	/// <param name="isUseFlash">点滅するか</param>
	/// <param name="isAlphaEnable">透明度処理を有効化するか</param>
	/// <param name="nAlphaFrame">透明化のフレーム</param>
	/// <param name="isAlphaTest">αテストするか</param>
	/// <returns></returns>
	static CUi* Create
	(
		const D3DXVECTOR3& pos, 
		const int& nFlashFrame,
		const float& fWidth, 
		const float& fHeight,
		const char * Filename,
		bool isUseFlash = false,
		bool isAlphaEnable = false,
		int nAlphaFrame = 0,
		bool isAlphaTest = false
	);

private:

	void UpdateAlphaEnable(void);	// 点滅関数

	int m_nFlashFrame;				// 点滅間隔
	int m_nAlphaFrame;				// フェード全体のフレーム数
	int m_nAlphaCnt;				// 現在のフレーム

	bool m_isFlash;					// 点滅するかどうか
	bool m_isAlphaEnable;			// 描画開始から透明度を触るかどうか
	bool m_isAlphaTest;				// αテストするかどうか
};