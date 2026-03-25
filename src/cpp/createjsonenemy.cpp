//===================================================================
//
// 敵をJsonから生成する処理 [ createjsonenemy.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonenemy.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "enemy.h"
#include "enemymanager.h"
#include "jsonconverter.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateEnemy::Create(const json& Jsondata)
{
	// インスタンス取得
	auto instance = CEnemyManager::GetInstance();
	if (instance == nullptr) return;

	// ファイルパス情報の設定
	std::string ModelPath = CJsonConverter::ConverterPath(Jsondata, "filepath");

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 拡大率情報の設定
	D3DXVECTOR3 Scale = CJsonConverter::ConverterVec3(Jsondata, "scale");

	// 実際の敵生成
	instance->CreateManager(pos, rot, Scale, ModelPath.c_str());
}