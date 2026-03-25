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
	LoadJson(pLoadName);

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
//=========================================================
// 外部ファイル読み込み
//=========================================================
void CWallTargetManager::LoadJson(const char* pLoadName)
{
	// 開くファイル
	std::ifstream file(pLoadName);

	// 開け無かった
	if (!file.is_open())
	{
		// 例外
		MessageBox(GetActiveWindow(), "ファイルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONERROR);
		return;
	}

	// jsonデータをセットする
	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// 既存のブロックを消す
	for (auto block : m_pTargetList)
	{
		if (block != nullptr)
		{
			// ブロックの終了処理
			block->Uninit();
		}
	}

	// 動的配列を空にする
	m_pTargetList.clear();

	// SetObjectsの配列を回す
	for (const auto& b : j["SetObjects"])
	{
		// ファイルパス
		std::string filepath = b["filepath"];

		// 座標
		D3DXVECTOR3 pos
		(
			b["pos"][0],
			b["pos"][1],
			b["pos"][2]
		);

		// 角度
		D3DXVECTOR3 rot
		(
			b["rot"][0],
			b["rot"][1],
			b["rot"][2]
		);

		// サイズ
		D3DXVECTOR3 size
		(
			b["scale"][0],
			b["scale"][1],
			b["scale"][2]
		);

		// 実際のポイントの生成
		CreateManager(pos, rot, size, filepath.c_str());
	}
}