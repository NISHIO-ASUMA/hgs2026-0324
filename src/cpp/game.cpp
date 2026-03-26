//=========================================================
//
// メインゲーム処理 [ game.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "game.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "gamemanager.h"
#include "gamesceneobject.h"
#include "gamestate.h"
#include "input.h"
#include "camera.h"
#include "result.h"
#include "fade.h"
#include "gametime.h"
#include "goal.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CPauseManager* CGame::m_pPausemanager = nullptr; // ポーズマネージャーのポインタ

//=========================================================
// コンストラクタ
//=========================================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{

}
//=========================================================
// デストラクタ
//=========================================================
CGame::~CGame()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGame::Init(void)
{
	// カメラ初期化
	auto pCamera = CManager::GetInstance()->GetCamera();
	pCamera->Init();

	// ポーズマネージャー生成
	m_pPausemanager = new CPauseManager;
	if (m_pPausemanager == nullptr) return E_FAIL;

	// ポーズマネージャー初期化処理
	m_pPausemanager->Init();

	// ゲームマネージャー初期化
	CGameManager::GetInstance()->Init();
	CGameManager::GetInstance()->SetIsGameEnd(false);

	// ゲームオブジェクト初期化
	CGameSceneObject::GetInstance()->Init();

	// ゲームステート生成
	m_pState = new CGameState;
	if (m_pState == nullptr) return E_FAIL;

	// 状態管理開始
	m_pState->SetGame(this);
	m_pState->OnStart();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGame::Uninit(void)
{
	// ゲーム管理の破棄
	CGameManager::GetInstance()->Uninit();

	// ゲームオブジェクトの破棄
	CGameSceneObject::GetInstance()->Uninit();

	// nullチェック
	if (m_pPausemanager != nullptr)
	{
		// 終了処理
		m_pPausemanager->Uninit();
		delete m_pPausemanager;
		m_pPausemanager = nullptr;
	}

	// nullチェック
	if (m_pState)
	{
		// 終了処理
		m_pState->OnExit();
		delete m_pState;
		m_pState = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CGame::Update(void)
{	
	// ゲーム進行管理の更新処理
	m_pState->OnUpdate();

	// 現在状態取得
	auto State = m_pState->GetProgress();
	if (State == m_pState->PROGRESS_END) return;

	// ポーズのキー入力判定
	m_pPausemanager->SetEnablePause();
	
	// ポーズ管理クラスの更新処理
	m_pPausemanager->Update();
	
#if 1
	// falseの時に更新
	if (!m_pPausemanager->GetPause() && State == m_pState->PROGRESS_NORMAL)
	{
		// ゲームマネージャー更新
		CGameManager::GetInstance()->Update();

		// ゲームオブジェクト更新
		CGameSceneObject::GetInstance()->Update();

		// 時間が0になったら
		if (CGameSceneObject::GetInstance()->GetTime()->GetToAll() <= 0)
		{
			// ゲーム終了状態に設定
			m_pState->SetProgress(CGameState::PROGRESS_END);
			return;
		}

		// ゴール結果が有効なら
		if (CGameSceneObject::GetInstance()->GetGoal()->GetIsGoalFlag() == true)
		{
			// ゲーム終了状態に設定
			m_pState->SetProgress(CGameState::PROGRESS_END);
			return;
		}

		// ゲーム終了フラグが有効なら
		if (CGameManager::GetInstance()->GetIsGameEnd())
		{
			// ゲーム敗北状態に設定
			m_pState->SetProgress(CGameState::PROGRESS_END);
			return;
		}
	}
#endif

#ifdef _DEBUG
	// 画面遷移デバッグキー
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F5))
	{
		// 画面遷移
		auto fade = CManager::GetInstance()->GetFade();
		if (fade != nullptr) fade->SetFade(std::make_unique<CResult>());
		return;
	}
#endif // _DEBUG
}
//=========================================================
// 描画処理
//=========================================================
void CGame::Draw(void)
{
}