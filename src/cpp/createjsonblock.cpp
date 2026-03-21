//===================================================================
//
// ブロックをJsonから生成する処理 [ createjsonblock.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonblock.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "jsonconverter.h"
#include "blockmanager.h"
#include "jsonmanager.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateBlock::Create(const json& Jsondata)
{
	// マネージャーからjsonマネージャーを取得
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	auto BlockManager = jsonManager->GetBlockManager();

	// ポインタが設定されていなかったら
	if (!BlockManager) return;

	// ファイルパス情報の設定
	std::string ModelPath = CJsonConverter::ConverterPath(Jsondata, "filepath");

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 拡大率情報の設定
	D3DXVECTOR3 Scale = CJsonConverter::ConverterVec3(Jsondata, "scale");

	// 実際のブロック生成
	BlockManager->CreateManager(pos, rot, Scale, ModelPath.c_str());
}