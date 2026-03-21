//=========================================================
//
// フェード処理 [ fade.h ]
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
// 前方宣言
//*********************************************************
class CScene;

//*********************************************************
// フェードクラスを定義
//*********************************************************
class CFade
{
public:

	//**********************
	// フェード状態列挙型
	//**********************
	enum FADE
	{
		FADE_NONE,	// 何もない
		FADE_IN,	// フェードイン
		FADE_OUT,	// フェードアウト
		FADE_MAX
	};

	CFade();
	~CFade();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	/// <summary>
	/// シーンを設定する
	/// </summary>
	/// <param name="pNewScene">フェード先のインスタンス</param>
	void SetFade(std::unique_ptr<CScene> pNewScene);

	inline FADE GetFade(void) const { return m_fade; }

private:

	static constexpr float AlphaFade = 0.025f; // フェードで加算する値

	LPDIRECT3DVERTEXBUFFER9 m_pVtx;		// 頂点バッファ
	D3DXCOLOR m_col;					// ポリゴンカラー
	FADE m_fade;						// フェードの列挙型変数

	std::unique_ptr<CScene> m_pScene; 	// シーンクラスポインタ
};
