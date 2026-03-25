//=========================================================
//
// ポーズ管理処理 [ pausemanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "pausemanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "pause.h"
#include "sound.h"
#include "camera.h"

//*********************************************************
// 定数空間
//*********************************************************
namespace PAUSEMANAGEINFO
{
	const D3DXVECTOR3 BACEPOS = { 200.0f,180.0f,0.0f }; // 基準点座標
	constexpr int FLASHFLAME = 60;				// 点滅間隔
	constexpr float BACEWIDTH = 180.0f;			// 基準の横幅
	constexpr float BACEHEIGHT = 40.0f;			// 基準の高さ
	constexpr float UPPERWIDTH = 180.0f;		// 拡大した横幅
	constexpr float UPPERHEIGHT = 50.0f;		// 拡大した高さ
	constexpr float SPACEHEIGHT = 160.0f;		// ポリゴン同士の間隔
};

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
bool CPauseManager::m_isPause = false; // ポーズフラグ

//=========================================================
// コンストラクタ
//=========================================================
CPauseManager::CPauseManager(): m_pPause{},
m_nSelectIdx(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CPauseManager::~CPauseManager()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPauseManager::Init(void)
{
	// フラグ初期化
	m_isPause = false;

	// 初期セレクト番号設定
	m_nSelectIdx = CPause::MENU_RETRY;

	// 基準座標を設定
	D3DXVECTOR3 Bacepos = PAUSEMANAGEINFO::BACEPOS;
	
	// ポーズ生成
	for (int nPause = 0; nPause < SELECT_MAX; nPause++)
	{
		// 座標セット
		D3DXVECTOR3 pos = Bacepos;

		// 高さを空ける
		pos.y += nPause * PAUSEMANAGEINFO::SPACEHEIGHT;

		// ポーズUIを生成 
		if (nPause == CPause::MENU_BACK)
		{
			// 背景生成
			m_pPause[nPause] = CPause::Create(CENTERPOS, HALFWIDTH, HALFHEIGHT, COLOR_WHITE, nPause);
		}
		else
		{
			// 選択用ポリゴンの生成
			m_pPause[nPause] = CPause::Create(pos, PAUSEMANAGEINFO::BACEWIDTH, PAUSEMANAGEINFO::BACEHEIGHT, COLOR_WHITE, nPause);
		}
	}

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPauseManager::Uninit(void)
{
	
}
//=========================================================
// 更新処理
//=========================================================
void CPauseManager::Update(void)
{
	// ポーズ中じゃなかったら
	if (!m_isPause)	return;

	// 入力デバイス取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pJoyPad = CManager::GetInstance()->GetJoyPad();

	// nullチェック
	if (pKey == nullptr) return;
	if (pJoyPad == nullptr) return;

	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return;

	// 上キー入力
	if (pKey->GetTrigger(DIK_UP) || pKey->GetTrigger(DIK_W) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_UP))
	{
		// サウンド再生
		pSound->Play(pSound->SOUND_LABEL_SELECT);

		// インデックス番号を減算
		m_nSelectIdx--;

		// 最小値以下なら最小値に設定
		if (m_nSelectIdx < SELECT_BEGIN)
			m_nSelectIdx = SELECT_END;
	}

	// 下キー入力
	if (pKey->GetTrigger(DIK_DOWN) || pKey->GetTrigger(DIK_S) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_DOWN))
	{
		// サウンド再生
		pSound->Play(pSound->SOUND_LABEL_SELECT);

		// インデックス番号を加算
		m_nSelectIdx++;

		// 最大値以上なら最大値に設定
		if (m_nSelectIdx > SELECT_END)
			m_nSelectIdx = SELECT_BEGIN;
	}

	// フェード取得
	CFade* pFade = CManager::GetInstance()->GetFade();
	if (pFade == nullptr) return;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera == nullptr) return;

	// 選択されているメニューのポリゴンカラーを変更
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		// 背景は変えない
		if (nCnt == CPause::MENU_BACK) continue;

		// nullじゃなかったら
		if (m_pPause[nCnt])
		{
			// カラー変更
			if (nCnt == m_nSelectIdx)
			{
				m_pPause[nCnt]->SetFlash(NULL, PAUSEMANAGEINFO::FLASHFLAME,COLOR_YERROW);			// 点滅処理
				m_pPause[nCnt]->SetSize(PAUSEMANAGEINFO::UPPERWIDTH, PAUSEMANAGEINFO::UPPERHEIGHT); // 少し大きくする
			}
			else
			{
				m_pPause[nCnt]->SetCol(COLOR_WHITE);												// 白
				m_pPause[nCnt]->SetSize(PAUSEMANAGEINFO::BACEWIDTH, PAUSEMANAGEINFO::BACEHEIGHT);	// 元のサイズ
			}
		}
	}

	// Enterキー or Aボタン入力
	if (pKey->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(pJoyPad->JOYKEY_A))
	{
		// サウンド再生
		pSound->Play(pSound->SOUND_LABEL_RETURN);

		switch (m_nSelectIdx)
		{
		case CPause::MENU_BACK:	// 背景
			break;

		/// <summary>
		/// リトライ時はゲームを最初から
		/// </summary>
		/// <param name=""></param>
		case CPause::MENU_RETRY:
			if (pFade != nullptr) pFade->SetFade(std::make_unique<CGame>());
			SetEnablePause(false);
			break;

		/// <summary>
		/// コンテニュー時はそのまま継続
		/// </summary>
		/// <param name=""></param>
		case CPause::MENU_CONTINUE:
			SetEnablePause(false);	
			break;

		/// <summary>
		/// クイット時はタイトルに遷移
		/// </summary>
		/// <param name=""></param>
		case CPause::MENU_QUIT:
			if (pFade != nullptr) pFade->SetFade(std::make_unique <CTitle>());
			SetEnablePause(false);
			break;
		}
	}
}
//=========================================================
// ポーズかどうか判別する
//=========================================================
void CPauseManager::SetEnablePause(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetInstance()->GetSound();
	if (pSound == nullptr) return;

	// Pキー or Start が押された
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_P) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// サウンド再生
		pSound->Play(CSound::SOUND_LABEL_PAUSESTART);

		// フラグ変更
		m_isPause = m_isPause ? false : true;
	}

	if (m_isPause == false) CManager::GetInstance()->GetMouse()->SetCursorVisibility(false);
}