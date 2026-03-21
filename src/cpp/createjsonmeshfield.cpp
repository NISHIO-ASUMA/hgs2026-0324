//===================================================================
//
// メッシュフィールドをJsonから生成する処理 [ createjsonmeshfield.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// クラス定義ヘッダーファイル
//*******************************************************************
#include "createjsonmeshfield.h"

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "meshfield.h"
#include "jsonconverter.h"

//===================================================================
// 生成処理
//===================================================================
void CJsonCreateMeshField::Create(const json& Jsondata)
{
	// 座標情報の設定
	D3DXVECTOR3 pos = CJsonConverter::ConverterVec3(Jsondata, "Pos");

	// 大きさの設定
	float fRadiusX = CJsonConverter::ConverterFloat(Jsondata, "RadiusX");
	float fRadiusZ = CJsonConverter::ConverterFloat(Jsondata, "RadiusZ");

	// 分割数の設定
	int nSegmentX = CJsonConverter::ConverterInt(Jsondata, "SegmentX");
	int nSegmentZ = CJsonConverter::ConverterInt(Jsondata, "SegmentZ");

	// 実際のオブジェクト生成
	CMeshField::Create(pos, fRadiusX, fRadiusZ, nSegmentX, nSegmentZ);
}