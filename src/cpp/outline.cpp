//=========================================================
// 
// アウトラインシェーダークラス [ outline.cpp ]
// Author : Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "outline.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "renderer.h"

//=========================================================
// コンストラクタ
//=========================================================
COutLine::COutLine() : m_pOutLine(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
COutLine::~COutLine()
{

}
//=========================================================
// 初期読み込み
//=========================================================
HRESULT COutLine::Init(const char* pShaderFile)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ローカル変数
	LPD3DXBUFFER pErr = nullptr; 

	// シェーダーファイル読み込み
	HRESULT hr = D3DXCreateEffectFromFile
	(
		pDevice,			// デバイス
		pShaderFile,		// ファイル
		nullptr,
		nullptr,
		D3DXSHADER_DEBUG,
		nullptr,
		&m_pOutLine,		// エフェクトポインタ
		&pErr
	);

	// 読み込み失敗時
	if (FAILED(hr))
	{
		if (pErr)
		{
			// エラーメッセージ表示
			MessageBoxA
			(
				nullptr,
				(char*)pErr->GetBufferPointer(),
				"Out_Line.hlslが読み込めません",
				MB_OK
			);
			pErr->Release();
		}

		return hr;
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void COutLine::Uninit(void)
{
	// ポインタの破棄
	if (m_pOutLine)
	{
		m_pOutLine->Release();
		m_pOutLine = nullptr;
	}
}
//=========================================================
// シェーダー開始
//=========================================================
void COutLine::Begin(void)
{
	m_pOutLine->Begin(0,0);
}
//=========================================================
// パス開始
//=========================================================
void COutLine::BeginPass(const int nPass)
{
	m_pOutLine->BeginPass(nPass);
}
//=========================================================
// パス終了
//=========================================================
void COutLine::EndPass(void)
{
	m_pOutLine->EndPass();
}
//=========================================================
// シェーダー終了
//=========================================================
void COutLine::End(void)
{
	m_pOutLine->End();
}
//=========================================================
// パラメーター設定
//=========================================================
void COutLine::SetParameter(const float fOutLineWidth,const D3DXVECTOR4& color,const D3DXMATRIX& mtxworld)
{
	// ポインタが無かったら
	if (!m_pOutLine) return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 行列を取得する
	D3DXMATRIX mtxProj, mtxView;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);  // プロジェクション
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);		// ビューマトリックス

	// アウトライン幅
	m_pOutLine->SetFloat("g_fOutlineWidth", fOutLineWidth);

	// アウトラインカラー
	m_pOutLine->SetVector("g_Color", &color);

	// テクニック取得
	D3DXHANDLE hTech = m_pOutLine->GetTechniqueByName("OutLine");
	m_pOutLine->SetTechnique(hTech);

	// ワールドマトリックス設定
	D3DXMATRIX set = mtxworld * mtxView * mtxProj;
	m_pOutLine->SetMatrix("g_mtxViewProj", &set);

	// コミット切り替え
	m_pOutLine->CommitChanges();
}