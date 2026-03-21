//=========================================================
// 
// ポーズ処理 [ pause.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "pause.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"
#include "pausemanager.h"

//*********************************************************
// 定数名前空間
//*********************************************************
namespace PAUSEBASE
{
	constexpr const char* BACKTEXTURE = "data/TEXTURE/PauseBack.png";			// 背景の半透明ポリゴン
	constexpr const char* QUITMENU = "data/TEXTURE/pause_quit.png";				// タイトル選択ポリゴン
	constexpr const char* RETRYMENU = "data/TEXTURE/pause_retry.png";			// リトライ選択ポリゴン
	constexpr const char* CONTINUEMENU = "data/TEXTURE/pause_continue.png";		// ゲーム継続選択ポリゴン
};

//=========================================================
// コンストラクタ
//=========================================================
CPause::CPause(int nPriority) : CObject2D(nPriority),
m_nIdxTexture(-1),
m_nPauseType(NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CPause::~CPause()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CPause* CPause::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight, const D3DXCOLOR& col,int nType)
{
	// インスタンス生成
	CPause* pPause = new CPause;
	if (pPause == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pPause->Init())) return nullptr;

	// 2Dオブジェクト設定
	pPause->SetPos(pos);
	pPause->SetSize(fWidth, fHeight);
	pPause->SetCol(col);
	pPause->SetType(nType);
	pPause->SetTexture();

	return pPause;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPause::Init(void)
{
	// オブジェクトの種類を設定
	SetObjType(TYPE_PAUSE);

	// オブジェクトの初期化
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPause::Uninit(void)
{
	// オブジェクトの破棄
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CPause::Update(void)
{
	// ポーズが有効なら
	if (!CPauseManager::GetPause()) return;

	// オブジェクトの更新処理
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CPause::Draw(void)
{
	// ポーズが有効なら
	if (!CPauseManager::GetPause()) return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャセット
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// オブジェクトの描画処理
	CObject2D::Draw();
}
//=========================================================
// テクスチャ処理
//=========================================================
void CPause::SetTexture(void)
{
	// テクスチャポインタをマネージャーから取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// 種類によってテクスチャ割り当てを切り替える
	switch (m_nPauseType)
	{
	case MENU_BACK:		// 背景
		m_nIdxTexture = pTexture->Register(PAUSEBASE::BACKTEXTURE);
		break;

	case MENU_RETRY:	// リトライ選択時
		m_nIdxTexture = pTexture->Register(PAUSEBASE::RETRYMENU);
		break;

	case MENU_CONTINUE: // コンテニュー選択時
		m_nIdxTexture = pTexture->Register(PAUSEBASE::CONTINUEMENU);
		break;

	case MENU_QUIT:		// クイット選択時
		m_nIdxTexture = pTexture->Register(PAUSEBASE::QUITMENU);
		break;

	default:
		m_nIdxTexture = -1;
		break;
	}
}