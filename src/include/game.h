//==========================================================
//
// メインゲーム処理 [ game.h ]
// Author: Asuma Nishio
//
//==========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "scene.h"
#include "pausemanager.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CGameManager;
class CGameState;

//*********************************************************
// ゲームシーンクラスを定義
//*********************************************************
class CGame : public CScene
{
public:

	CGame();
	~CGame();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline CGameState* GetState(void) const { return m_pState; }

	static CPauseManager* GetPause(void) { return m_pPausemanager; }

private:
	static CPauseManager* m_pPausemanager;			// ポーズマネージャークラスポインタ
	CGameState * m_pState;							// ゲーム進行管理クラスのポインタ
};