//=========================================================
// 
// UI処理 [ ui.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "ui.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "texture.h"
#include "manager.h"
#include "easing.h"

//=========================================================
// コンストラクタ
//=========================================================
CUi::CUi(int nPriority) : CObject2D(nPriority),
m_isAlphaEnable(false), 
m_isFlash(false),
m_isAlphaTest(false),
m_nAlphaCnt(NULL), 
m_nAlphaFrame(NULL),
m_nFlashFrame(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CUi::~CUi()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CUi* CUi::Create
(
	const D3DXVECTOR3& pos,
	const int& nFlashFrame,
	const float& fWidth,
	const float& fHeight,
	const char* Filename,
	bool isUseFlash,
	bool isAlphaEnable,
	int nAlphaFrame,
	bool isAlphaTest
)
{
	// インスタンス生成
	CUi* pUi = new CUi;
	if (pUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pUi->Init())) return nullptr;

	// オブジェクト設定
	pUi->SetPos(pos);
	pUi->SetSize(fWidth, fHeight);
	pUi->SetAnchor(CObject2D::ANCHORTYPE_CENTER);
	pUi->SetTexture(Filename);

	pUi->m_nFlashFrame = nFlashFrame;
	pUi->m_isFlash = isUseFlash;
	pUi->m_isAlphaEnable = isAlphaEnable;
	pUi->m_nAlphaFrame = nAlphaFrame;
	pUi->m_isAlphaTest = isAlphaTest;

	// 生成されたポインタを返す
	return pUi;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CUi::Init(void)
{
	// オブジェクトの初期化
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CUi::Uninit(void)
{
	// オブジェクトの終了
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CUi::Update(void)
{
	// フェード透明化フラグ有効時
	if (m_isAlphaEnable)
	{
		UpdateAlphaEnable();
	}

	// 点滅有効時
	if (m_isFlash)
	{
		// 点滅処理
		SetFlash(NULL, m_nFlashFrame, COLOR_WHITE);
	}

	// 親クラス更新
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CUi::Draw(void)
{
	// デバイス取得
	const auto& Device = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_isAlphaTest)
	{
		// αテスト実行
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Device->SetRenderState(D3DRS_ALPHAREF, 0);
		Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}
	
	// 親クラスのの描画
	CObject2D::Draw();

	// αテスト有効時
	if (m_isAlphaTest)
	{
		// 設定を元に戻す
		Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}
//=========================================================
// フェード点滅更新関数
//=========================================================
void CUi::UpdateAlphaEnable(void)
{
	// カウント上限に達したら
	if (m_nAlphaCnt >= m_nAlphaFrame)
	{
		// 最終的に透明にして止める
		SetCol(COLOR_CLEAR);
		m_isAlphaEnable = false;
		return;
	}

	// 補完係数を計算する
	float fMathAlpha = static_cast<float>(m_nAlphaCnt) / static_cast<float>(m_nAlphaFrame);
	float fAlpha = 0.0f;

	// フェードイン
	if (fMathAlpha < 0.5f)
	{
		fAlpha = fMathAlpha * 2.0f;
	}
	// フェードアウト
	else
	{
		fAlpha = (1.0f - fMathAlpha) * 2.0f;
	}

	// オブジェクトのカラーに設定
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha));

	// カウンターを加算
	m_nAlphaCnt++;
}