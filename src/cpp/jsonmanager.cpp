//=========================================================
//
// jsonファイル読み込み管理処理 [ jsonmanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "jsonmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "createjsonui.h"
#include "createjsonmeshfield.h"
#include "createjsonmeshdome.h"
#include "createjsoncharactor.h"
#include "createjsonblock.h"
#include "createjsonblocknormal.h"
#include "createjsonwall.h"
#include "createjsonenemy.h"
#include "createjsonmaker.h"

//*********************************************************
// json空間を使用
//*********************************************************
using json = nlohmann::json;

//*********************************************************
// タグ名の定数宣言
//*********************************************************
namespace PATH_TAGNAME
{
	constexpr const char* UI			= "Ui";			// UIパラメータ
	constexpr const char* MESHDOME		= "MeshDome";	// メッシュドームパラメータ
	constexpr const char* MESHFIELD		= "MeshField";	// メッシュフィールドパラメータ
	constexpr const char* CHARACTOR		= "Charactor";	// キャラクターパラメータ
	constexpr const char* BLOCK			= "Block";		// ブロックパラメータ
	constexpr const char* NORMALBLOCK	= "NormalBlock";// 通常ブロックパラメータ
	constexpr const char* WALL			= "Wall";		// 壁モデルパラメータ
	constexpr const char* ENEMY			= "Enemy";		// 敵パラメータ
	constexpr const char* MAKER			= "Maker";		// ワイヤーポイントマーカー
};

//=========================================================
// コンストラクタ
//=========================================================
CJsonManager::CJsonManager() : m_Creator{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CJsonManager::~CJsonManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CJsonManager::Init(void)
{
	// 関数生成を登録
	m_Creator[PATH_TAGNAME::UI] = std::make_unique<CJsonCreateUi>();
	m_Creator[PATH_TAGNAME::MESHFIELD] = std::make_unique<CJsonCreateMeshField>();
	m_Creator[PATH_TAGNAME::MESHDOME] = std::make_unique<CJsonCreateMeshDome>();
	m_Creator[PATH_TAGNAME::CHARACTOR] = std::make_unique<CJsonCreateCharactor>();
	m_Creator[PATH_TAGNAME::BLOCK] = std::make_unique<CJsonCreateBlock>();
	m_Creator[PATH_TAGNAME::NORMALBLOCK] = std::make_unique<CJsonCreateBlockNormal>();
	m_Creator[PATH_TAGNAME::WALL] = std::make_unique<CJsonCreateWall>();
	m_Creator[PATH_TAGNAME::ENEMY] = std::make_unique<CJsonCreateEnemy>();
	m_Creator[PATH_TAGNAME::MAKER] = std::make_unique<CJsonCreateMaker>();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CJsonManager::Uninit(void)
{
	m_Creator.clear();
}
//=========================================================
// 実際のファイル読み込み処理
//=========================================================
HRESULT CJsonManager::Load(const char* LoadFileName)
{
	// 開くファイルを設定
	std::ifstream file(LoadFileName);

	// ファイルがない場合
	if (!file.is_open())
	{
		// 例外メッセージ出力
		MessageBox(GetActiveWindow(), "ファイルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// jsonデータをセットする
	json jsonfile;
	file >> jsonfile;
	file.close();

	// 識別タグで生成関数を呼び出す
	for (auto& Objectlist : jsonfile)
	{
		// オブジェクトのタグ取得
		std::string tag = Objectlist["ObjectTag"];

		// 該当タグを見つける
		auto iter = m_Creator.find(tag);

		if (iter != m_Creator.end())
		{
			// 生成関数を実行
			iter->second->Create(Objectlist);
		}
	}

	return S_OK;
}