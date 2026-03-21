//=========================================================
//
// ポーズ処理 [ pause.h ]
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
#include "object2D.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CObject;

//*********************************************************
// ポーズクラスを定義
//*********************************************************
class CPause : public CObject2D
{
public:

	//****************************
	// 選択肢列挙型
	//****************************
	enum MENU
	{
		MENU_BACK,		// 背景
		MENU_RETRY,		// やり直し
		MENU_CONTINUE,  // 継続
		MENU_QUIT,		// タイトル遷移
		MENU_MAX
	};

	CPause(int nPriority = static_cast<int>(CObject::PRIORITY::PAUSE));
	~CPause();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetTexture(void);

	inline void SetType(const int& nType) { m_nPauseType = nType; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="col">カラー</param>
	/// <param name="nType">メニューの種類</param>
	/// <returns></returns>
	static CPause* Create
	(
		const D3DXVECTOR3& pos, 
		float fWidth, 
		float fHeight, 
		const D3DXCOLOR& col, 
		int nType
	);

private:

	int m_nIdxTexture;	// テクスチャインデックス番号
	int m_nPauseType;	// ポーズの種類
};