//=========================================================
//
// 敵の管理処理 [ enemymanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "enemymanager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "manager.h"
#include "jsonmanager.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyManager::CEnemyManager() : m_pEnemyList{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemyManager::~CEnemyManager()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemyManager::Init(const char * pLoadName)
{
	// 敵の配列のクリア
	m_pEnemyList.clear();

	// jsonmanager取得
	auto JsonManager = CManager::GetInstance()->GetJsonManager();
	JsonManager->Load(pLoadName);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemyManager::Uninit(void)
{
	// 敵の配列のクリア
	m_pEnemyList.clear();

}
//=========================================================
// 更新処理
//=========================================================
void CEnemyManager::Update(void)
{

}
//=========================================================
// 生成処理
//=========================================================
CEnemy* CEnemyManager::CreateManager
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale,
	const char* pModelName
)
{
	// 生成処理
	CEnemy* pNewEnemy = CEnemy::Create(pos, rot, scale, pModelName);
	if (pNewEnemy)
	{
		m_pEnemyList.push_back(pNewEnemy);
	}

	return pNewEnemy;
}
