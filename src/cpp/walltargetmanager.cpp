//=========================================================
//
// ターゲットのポイント管理処理 [ walltargetmanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "walltargetmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <fstream>
#include <iostream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "json.hpp"
#include "walltargetpoint.h"
#include "manager.h"
#include "jsonmanager.h"

//*********************************************************
// 名前空間
//*********************************************************
using json = nlohmann::json;

//=========================================================
// コンストラクタ
//=========================================================
CWallTargetManager::CWallTargetManager() : m_pTargetList{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CWallTargetManager::~CWallTargetManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWallTargetManager::Init(const char* pLoadName)
{
	// 配列クリア
	m_pTargetList.clear();

	// ファイル読み込み
	auto JsonManager = CManager::GetInstance()->GetJsonManager();
	JsonManager->Load(pLoadName);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWallTargetManager::Uninit(void)
{
	m_pTargetList.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CWallTargetManager::Update(void)
{

}
//=========================================================
// 生成関数
//=========================================================
CWallTargetPoint* CWallTargetManager::CreateManager
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot, 
	const D3DXVECTOR3& scale, 
	const char* pModelName
)
{
	// インスタンス生成
	CWallTargetPoint* pNewPoint = CWallTargetPoint::Create(pos, rot, scale, pModelName);
	if (pNewPoint) m_pTargetList.push_back(pNewPoint);

	return pNewPoint;
}