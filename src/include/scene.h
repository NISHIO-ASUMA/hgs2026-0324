//=========================================================
//
// シーン処理 [ scene.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// シーンクラスを定義
//*********************************************************
class CScene
{
public:

	//*************************
	// シーンの種類列挙型
	//*************************
	enum MODE
	{
		MODE_NONE,		// 何も無い
		MODE_START,		// 開始
		MODE_TITLE,		// タイトル
		MODE_TUTORIAL,	// チュートリアル
		MODE_GAME,		// ゲーム
		MODE_RESULT,	// リザルト
		MODE_LOSERESULT,// 失敗リザルト
		MODE_RANKING,	// ランキング
		MODE_MAX
	};

	CScene(MODE mode);
	virtual ~CScene();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	inline MODE GetScene(void) const { return m_Scene; }

private:

	MODE m_Scene;	// シーン管理変数
};