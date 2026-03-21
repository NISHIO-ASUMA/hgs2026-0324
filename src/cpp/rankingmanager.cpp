//=========================================================
//
// ランキング管理処理 [ rankingmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "rankingmanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "title.h"

//=========================================================
// インスタンス取得
//=========================================================
CRankingManager* CRankingManager::GetInstance(void)
{
	static CRankingManager pRankManager;
	return &pRankManager;
}
//=========================================================
// コンストラクタ
//=========================================================
CRankingManager::CRankingManager()
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CRankingManager::~CRankingManager()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CRankingManager::Init(void)
{
	// サウンド取得
	auto Sound = CManager::GetInstance()->GetSound();
	if (Sound == nullptr)return E_FAIL;

	// BGM再生
	Sound->Play(CSound::SOUND_LABEL_RANKING);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CRankingManager::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CRankingManager::Update(void)
{
	// キー入力で画面遷移
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_A) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// フェード取得
		CFade* pFade = CManager::GetInstance()->GetFade();
		if (pFade != nullptr)
		{
			// タイトルシーンに遷移
			pFade->SetFade(std::make_unique<CTitle>());
			return;
		}
	}
}