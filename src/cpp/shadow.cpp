//=========================================================
//
// 影処理 [ shadow.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "shadow.h"
#include "manager.h"

//=========================================================
// オーバーロードコンストラクタ
//=========================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CShadow::~CShadow()
{
	// 無し
}
//==========================================================
// 生成処理
//==========================================================
CShadow* CShadow::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CShadow* pShadow = new CShadow;
	if (pShadow == nullptr) return nullptr;

	// オブジェクト設定
	pShadow->SetPos(pos);
	pShadow->SetRot(rot);
	pShadow->SetSize(Config::SIZE, Config::SIZE);
	pShadow->SetTexture(Config::TEXNAME);

	// 初期化失敗時
	if (FAILED(pShadow->Init())) return nullptr;

	// 影のポインタを返す
	return pShadow;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CShadow::Init(void)
{
	// 親クラスの初期化
	CObject3D::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CShadow::Uninit(void)
{
	// 親クラスの終了処理
	CObject3D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CShadow::Update(void)
{
	// 親クラスの更新処理
	CObject3D::Update();
}
//=========================================================
// 影の描画処理
//=========================================================
void CShadow::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 親クラスの描画
	CObject3D::Draw();

	// 減算合成を戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}