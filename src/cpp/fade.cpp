//=========================================================
//
// フェード処理 [ fade.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "fade.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CFade::CFade() : m_pScene(nullptr),
m_pVtx(nullptr),
m_col(COLOR_BLACK),
m_fade(FADE_NONE)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CFade::~CFade()
{
	// シーンポインタの破棄
	m_pScene.reset();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CFade::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * BASEVERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtx,
		NULL);

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtx->Unlock();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CFade::Uninit(void)
{
	// シーンポインタの破棄
	m_pScene.reset();

	// nullptrチェック
	if (m_pVtx)
	{
		m_pVtx->Release();
		m_pVtx = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CFade::Update(void)
{
	// 何もなかったら
	if (m_fade == FADE_NONE) return;

	// 頂点情報のポインタ
	VERTEX_2D* pVtx = nullptr;

	// FADEIN状態
	if (m_fade == FADE_IN)
	{
		// α値を減算
		m_col.a -= AlphaFade;

		if (m_col.a <= 0.0f)
		{
			m_col.a = 0.0f;
			m_fade = FADE_NONE;		// 何もしていない状態
		}
	}
	// FADEOUT状態
	else if (m_fade == FADE_OUT)
	{
		// α値を加算
		m_col.a += AlphaFade;

		if (m_col.a >= 1.0f)
		{
			m_col.a = 1.0f;
			m_fade = FADE_IN;

			// 次のモード設定
			CManager::GetInstance()->SetScene(std::move(m_pScene));

			return;
		}
	}

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtx->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	// アンロック
	m_pVtx->Unlock();
}
//=========================================================
// 描画処理
//=========================================================
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtx, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャをnull化する
	pDevice->SetTexture(0, nullptr);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=========================================================
// 設定処理
//=========================================================
void CFade::SetFade(std::unique_ptr<CScene> pNewScene)
{
	// タイプがNONEじゃ無かったら
	if (m_fade != FADE_NONE)
	{
		// すでにフェード中なら破棄
		pNewScene.reset();
		return;
	}

	// 現在のシーンをクリアして、新しいシーンを保持
	m_pScene = std::move(pNewScene);

	// フェードアウト開始
	m_fade = FADE_OUT;
}