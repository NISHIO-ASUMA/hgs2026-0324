//=========================================================
//
// ゲーム状態管理処理 [ gamestate.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gamestate.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "gamesceneobject.h"
#include "fade.h"
#include "result.h"
#include "gamemanager.h"
#include "loseresult.h"
#include "score.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace GAMESTATE
{
	constexpr int STATECOUNT = 30; // 画面切り替えるまでのカウント
};

//=========================================================
// コンストラクタ
//=========================================================
CGameState::CGameState() : m_pGame(nullptr),
m_Progress(PROGRESS_NONE),
m_nCount(NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CGameState::~CGameState()
{
	
}
//=========================================================
// 状態開始関数
//=========================================================
void CGameState::OnStart()
{
	// 初期状態をセットする
	m_Progress = PROGRESS_NORMAL;
}
//=========================================================
// 状態更新関数
//=========================================================
void CGameState::OnUpdate()
{
	// ポインタが無かったら
	if (m_pGame == nullptr) return;

	// フェード取得
	auto pFade = CManager::GetInstance()->GetFade();
	if (pFade == nullptr) return;

	// シーンオブジェクト取得
	switch (m_Progress)
	{
	case CGameState::PROGRESS_NONE: // 何もない
		break;

	case CGameState::PROGRESS_NORMAL: // 進行を続ける
		break;

	case CGameState::PROGRESS_END: // 終了し,画面遷移

		// カウントを加算
		m_nCount++;

		if (m_nCount >= GAMESTATE::STATECOUNT)
		{
			// カウンターを初期化
			m_nCount = NULL;

			// 60フレーム経過
			m_Progress = PROGRESS_NONE; 

			// スコア情報の書き出し
			CGameSceneObject::GetInstance()->GetScore()->SaveScore();

			// フェードが取得できたら
			if (pFade != nullptr)
			{
				// リザルトシーンに遷移
				pFade->SetFade(std::make_unique<CResult>());
			}
		}
		break;

	case PROGRESS_LOSE:

		// カウントを加算
		m_nCount++;

		if (m_nCount >= GAMESTATE::STATECOUNT)
		{
			// カウンターを初期化
			m_nCount = NULL;

			// 60フレーム経過
			m_Progress = PROGRESS_NONE;

			// フェードが取得できたら
			if (pFade != nullptr)
			{
				// 負けリザルトシーンに遷移
				pFade->SetFade(std::make_unique<CLoseResult>());
			}
		}
		break;

	default:
		break;
	}
}
//=========================================================
// 状態終了関数
//=========================================================
void CGameState::OnExit()
{
	// ポインタのnull初期化
	m_pGame = nullptr;
}