//=========================================================
//
// タイトル管理処理 [ titlemanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "titlemanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"

//=========================================================
// コンストラクタ
//=========================================================
CTitleManager::CTitleManager()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTitleManager::~CTitleManager()
{

}
//==========================================================
//インスタンス取得
//==========================================================
CTitleManager* CTitleManager::GetInstance(void)
{
	// インスタンスを返す
	static CTitleManager pTitleManager;
	return &pTitleManager;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTitleManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_TITLE_BGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTitleManager::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CTitleManager::Update(void)
{
	// 入力デバイス取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();

	// 取得失敗時
	if (pKey == nullptr) return;
	if (pJoyPad == nullptr) return;

	// フェード取得
	CFade* pFade = CManager::GetInstance()->GetFade();
	if (pFade == nullptr) return;

	// キー入力時の遷移
	if ((pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_START) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A)))
	{
		// サウンド取得
		CSound* pSound = CManager::GetInstance()->GetSound();
		if (pSound == nullptr) return;

		// ゲームシーンに遷移
		pFade->SetFade(std::make_unique<CGame>());

		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_TITLEENTER);
		return;
	}
}