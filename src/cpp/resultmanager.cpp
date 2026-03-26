//=========================================================
//
// リザルトシーン管理処理 [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "resultmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <fstream>
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "ranking.h"

//=========================================================
// コンストラクタ
//=========================================================
CResultManager::CResultManager()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CResultManager::~CResultManager()
{
	
}
//=========================================================
// インスタンス取得
//=========================================================
CResultManager* CResultManager::GetInstance(void)
{
	static CResultManager pResultManager;
	return &pResultManager;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CResultManager::Init(void)
{	
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->Play(CSound::SOUND_LABEL_RESULTBGM);

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CResultManager::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CResultManager::Update(void)
{
	// 入力デバイスを取得
	CInputKeyboard* pInput = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJyoPad = CManager::GetInstance()->GetJoyPad();

	if (pInput == nullptr) return;
	if (pJyoPad == nullptr) return;

	// 決定キーが押された
	if ((pInput->GetTrigger(DIK_RETURN) || pJyoPad->GetTrigger(pJyoPad->JOYKEY_A)))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetInstance()->GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// ランキングシーン遷移
			pFade->SetFade(std::make_unique<CTitle>());
			return;
		}
	}
}