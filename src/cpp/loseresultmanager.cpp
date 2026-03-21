//=========================================================
//
// 負けリザルトシーン管理処理 [ loseresultmanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "loseresultmanager.h"

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
#include "title.h"

//=========================================================
// コンストラクタ
//=========================================================
CLoseResultManager::CLoseResultManager()
{

}
//=========================================================
// デストラクタ
//=========================================================
CLoseResultManager::~CLoseResultManager()
{

}
//=========================================================
// インスタンス取得処理
//=========================================================
CLoseResultManager* CLoseResultManager::GetInstance(void)
{
	static CLoseResultManager Instance;
	return &Instance;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CLoseResultManager::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_LOSERESULTBGM);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CLoseResultManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CLoseResultManager::Update(void)
{
	// キー入力でタイトルに戻る
	if ((CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) ||
		 CManager::GetInstance()->GetJoyPad()->GetTrigger(CManager::GetInstance()->GetJoyPad()->JOYKEY_START) ||
		 CManager::GetInstance()->GetJoyPad()->GetTrigger(CManager::GetInstance()->GetJoyPad()->JOYKEY_A)))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetInstance()->GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// タイトルシーン遷移
			pFade->SetFade(std::make_unique<CTitle>());
			return;
		}
	}
}