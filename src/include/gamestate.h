//=========================================================
//
// ゲーム状態管理処理 [ gamestate.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 前方宣言
//*********************************************************
class CGame;

//*********************************************************
// ゲーム状態管理クラスを定義
//*********************************************************
class CGameState
{
public:

	//************************
	// 進行状態管理列挙型
	//************************
	enum PROGRESS
	{
		PROGRESS_NONE,		// 何も無い
		PROGRESS_NORMAL,	// 通常状態
		PROGRESS_END,		// ゲーム終了
		PROGRESS_LOSE,		// 挑戦失敗
		PROGRESS_MAX
	};

	CGameState();
	~CGameState();

	void OnStart();
	void OnUpdate();
	void OnExit();

	void SetProgress(PROGRESS progtype) { m_Progress = progtype; }
	void SetGame(CGame* pGame) { m_pGame = pGame; }

	int GetProgress(void) { return static_cast<int>(m_Progress); }

private:

	CGame * m_pGame;		// ゲームシーンポインタ
	PROGRESS m_Progress;	// 進行状態の列挙型変数	
	int m_nCount;			// 進行管理カウント
};