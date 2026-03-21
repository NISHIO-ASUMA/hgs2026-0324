//=========================================================
//
// チュートリアル管理処理 [ tutorialmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//********************************************************
// クラス定義ヘッダーファイル
//********************************************************
#include "tutorialmanager.h"

//********************************************************
// インクルードファイル
//********************************************************
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "ui.h"
#include "sound.h"
#include "input.h"
#include "game.h"
#include "camera.h"
//#include "tutorialui.h"

//=========================================================
// インスタンス取得
//=========================================================
CTutorialManager* CTutorialManager::GetInstance(void)
{
	// 生成されたインスタンス
	static CTutorialManager pTutoManager;
	return &pTutoManager;
}
//=========================================================
// コンストラクタ
//=========================================================
CTutorialManager::CTutorialManager()
{

}
//=========================================================
// デストラクタ
//=========================================================
CTutorialManager::~CTutorialManager()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CTutorialManager::Init(void)
{
	// カメラ初期化
	auto Camera = CManager::GetInstance()->GetCamera();
	Camera->Init();

	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	//pSound->Play(CSound::SOUND_LABEL_TUTORIALBGM);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CTutorialManager::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CTutorialManager::Update(void)
{
	// 画面遷移キー
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_TAB) || 
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// ゲームシーンに遷移する
		CManager::GetInstance()->GetFade()->SetFade(std::make_unique<CGame>());
		return;
	}
}