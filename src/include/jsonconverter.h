//=========================================================
//
// Jsonデータ変換用ユーティリティ関数クラス [ jsonconverter.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <string>
#include <json.hpp>

//*********************************************************
// 名前空間を定義
//*********************************************************
namespace JsonUtil
{
	using json = nlohmann::json;
};

//*********************************************************
// ユーティリティ関数を格納するクラスを定義
//*********************************************************
class CJsonConverter
{
public:

	CJsonConverter() = default;
	~CJsonConverter() = default;

	/// <summary>
	/// VECTOR3型の変換関数
	/// </summary>
	/// <param name="JsonData">jsonデータ</param>
	/// <param name="KeyName">キーの名前</param>
	/// <returns></returns>
	static D3DXVECTOR3 ConverterVec3(const JsonUtil::json& JsonData, const char* KeyName);

	/// <summary>
	/// int型の変換関数
	/// </summary>
	/// <param name="JsonData">jsonデータ</param>
	/// <param name="KeyName">キーの名前</param>
	/// <returns></returns>
	static int ConverterInt(const JsonUtil::json& JsonData, const char* KeyName);

	/// <summary>
	/// float型の変換関数
	/// </summary>
	/// <param name="JsonData">jsonデータ</param>
	/// <param name="KeyName">キーの名前</param>
	/// <returns></returns>
	static float ConverterFloat(const JsonUtil::json& JsonData, const char* KeyName);

	/// <summary>
	/// bool型の変換関数
	/// </summary>
	/// <param name="JsonData">jsonデータ</param>
	/// <param name="KeyName">キーの名前</param>
	/// <returns></returns>
	static bool ConverterBool(const JsonUtil::json& JsonData, const char* KeyName);

	/// <summary>
	/// string型の変換関数
	/// </summary>
	/// <param name="JsonData">jsonデータ</param>
	/// <param name="KeyName">キーの名前</param>
	/// <returns></returns>
	static std::string ConverterPath(const JsonUtil::json& JsonData, const char* KeyName);

};