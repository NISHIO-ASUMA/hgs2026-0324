//=========================================================
//
// ゲーム管理処理 [ gamemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gamemanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "result.h"

//=========================================================
// コンストラクタ
//=========================================================
CGameManager::CGameManager() : m_isGameEnd(false)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CGameManager::~CGameManager()
{
	// 終了処理
	Uninit();
}
//=========================================================
// インスタンス取得
//=========================================================
CGameManager* CGameManager::GetInstance(void)
{
	static CGameManager pInstance;
	return &pInstance;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	//pSound->Play(CSound::SOUND_LABEL_GAMEBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CGameManager::Update(void)
{
#ifdef _DEBUG
	// 画面遷移
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		// リザルト画面に遷移
		CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CResult>());
		return;
	}
#endif
}