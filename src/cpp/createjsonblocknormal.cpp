//===================================================================
//
// 通常ブロックをJsonから生成する処理 [ createjsonblocknormal.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonblocknormal.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "block.h"
#include "jsonconverter.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateBlockNormal::Create(const json& Jsondata)
{
	// ファイルパス情報の設定
	std::string ModelPath = CJsonConverter::ConverterPath(Jsondata, "filepath");

	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "pos");

	// 角度情報の設定
	D3DXVECTOR3 rot = CJsonConverter::ConverterVec3(Jsondata, "rot");

	// 拡大率情報の設定
	D3DXVECTOR3 Scale = CJsonConverter::ConverterVec3(Jsondata, "scale");

	// 実際のブロック生成
	CBlock::Create(pos, rot, Scale, ModelPath.c_str());
}