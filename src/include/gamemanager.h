//==========================================================
//
// ゲーム進行管理処理 [ gamemanager.h ]
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
// ゲーム進行管理クラスを定義
//*********************************************************
class CGameManager
{
public:
	
	~CGameManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	inline void SetIsGameEnd(const bool isEnd) { m_isGameEnd = isEnd; }
	inline bool GetIsGameEnd(void) { return m_isGameEnd; }

	static CGameManager* GetInstance(void);

private:

	CGameManager();		// 格納コンストラクタ

	bool m_isGameEnd;	// ゲーム進行管理を終わらせるフラグ
};