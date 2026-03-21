//=========================================================
//
// ブロック処理 [ block.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "block.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "xfilemanager.h"
#include "boxcollider.h"
#include "collisionbox.h"

//=========================================================
// コンストラクタ
//=========================================================
CBlock::CBlock(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CBlock::~CBlock()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CBlock* CBlock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const char* pModelName)
{
	// インスタンス生成
	CBlock* pBlock = new CBlock;
	if (pBlock == nullptr) return nullptr;

	// オブジェクト設定
	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetScale(scale);
	pBlock->SetFilePass(pModelName);

	// 初期化失敗時
	if (FAILED(pBlock->Init())) return nullptr;

	return pBlock;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CBlock::Init(void)
{
	// 親クラスの初期化処理
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号を取得
	int nModelIdx = GetModelIdx();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// モデルのパス取得
	std::string str = pXManager->GetInfo(nModelIdx).FilePath;

	// コライダーの設定
	if (str == "data/MODEL/STAGEOBJ/Reef.x") m_pCollider = nullptr;
	else
		m_pCollider = CBoxCollider::Create(GetPos(), GetPos(), Size);
	
	// マトリックスシャドウを有効化する
	if (str == "data/MODEL/STAGEOBJ/wallback.x")
		SetShadow(false);
	else
		SetShadow(true);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CBlock::Uninit(void)
{
	// 矩形コライダーの破棄
	m_pCollider.reset();

	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CBlock::Update(void)
{
	// 現在の座標取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー座標の更新
	if (m_pCollider) m_pCollider->SetPos(pos);
}
//=========================================================
// 描画処理
//=========================================================
void CBlock::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}
//=========================================================
// 当たり判定
//=========================================================
bool CBlock::Collision(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullチェック
	if (m_pCollider == nullptr) return false;

	// 矩形同士の当たり判定を返す
	return CCollisionBox::Collision(m_pCollider.get(), pOther, OutPos);
}