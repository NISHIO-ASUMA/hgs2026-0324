//=========================================================
//
// Jsonデータ変換用関数クラス [ jsonconverter.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "jsonconverter.h"

//=========================================================
// VECTOR3型の変数情報を返す
//=========================================================
D3DXVECTOR3 CJsonConverter::ConverterVec3(const JsonUtil::json& JsonData, const char* KeyName)
{
	D3DXVECTOR3 Localvec3 = VECTOR3_NULL;

	// jsonから読み取った値を反映
	Localvec3.x = JsonData[KeyName][0];
	Localvec3.y = JsonData[KeyName][1];
	Localvec3.z = JsonData[KeyName][2];

	return Localvec3;
}
//=========================================================
// int型の変数情報を返す
//=========================================================
int CJsonConverter::ConverterInt(const JsonUtil::json& JsonData, const char* KeyName)
{
	// デフォルトで0を返す
	return JsonData.value(KeyName, 0);
}
//=========================================================
// float型の変数情報を返す
//=========================================================
float CJsonConverter::ConverterFloat(const JsonUtil::json& JsonData, const char* KeyName)
{
	// デフォルトで0を返す
	return JsonData.value(KeyName, 0.0f);
}
//=========================================================
// bool型の変数情報を返す
//=========================================================
bool CJsonConverter::ConverterBool(const JsonUtil::json& JsonData, const char* KeyName)
{
	// デフォルトでfalseを返す
	return JsonData.value(KeyName, false);
}
//=========================================================
// std::string型の変数情報を返す
//=========================================================
std::string CJsonConverter::ConverterPath(const JsonUtil::json& JsonData, const char* KeyName)
{
	if (!JsonData.contains(KeyName))
	{
		return "";
	}

	if (!JsonData[KeyName].is_string())
	{
		return "";
	}

	return JsonData[KeyName].get<std::string>();
}