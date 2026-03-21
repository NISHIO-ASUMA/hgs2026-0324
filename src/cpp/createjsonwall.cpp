//===================================================================
//
// 壁モデルをJsonから生成する処理 [ createjsonwall.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonwall.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "manager.h"
#include "jsonmanager.h"
#include "jsonconverter.h"
//#include "worldwallmanager.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateWall::Create(const json& Jsondata)
{
	//// マネージャーからjsonマネージャーを取得
	//auto jsonManager = CManager::GetInstance()->GetJsonManager();
	//auto WallManager = jsonManager->GetWorldWallManager();

	//// ポインタが設定されていなかったら
	//if (!WallManager) return;

	// ファイルパス情報の設定
	std::string ModelPath = CJsonConverter::ConverterPath(Jsondata, "filepath");

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 拡大率情報の設定
	D3DXVECTOR3 Scale = CJsonConverter::ConverterVec3(Jsondata, "scale");

	// 実際の壁モデル生成
	//WallManager->CreateManager(pos, rot, Scale, ModelPath.c_str());
}