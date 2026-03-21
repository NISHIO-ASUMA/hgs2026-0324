//=========================================================
//
// テクスチャ処理 [ texture.h ]
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
#include <vector>

//*********************************************************
// テクスチャ管理クラスを定義
//*********************************************************
class CTexture
{
public:

	CTexture();
	~CTexture();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// テクスチャを配列に登録する
	/// </summary>
	/// <param name="pFileName">登録テクスチャ名</param>
	/// <returns></returns>
	int Register(const char* pFileName);

	/// <summary>
	/// 指定インデックスのアドレスを取得する
	/// </summary>
	/// <param name="nIdx">テクスチャ登録番号</param>
	/// <returns></returns>
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:

	//*************************
	// テクスチャデータ構造体
	//*************************
	struct TEXDATA
	{
		std::string TexName;		// テクスチャ名
		LPDIRECT3DTEXTURE9 TexData; // テクスチャポインタ
	};

	//*************************
	// 定数構造体宣言
	//*************************
	struct Config
	{
		static constexpr const char* LOAD_NAME = "data/JSON/Texture.json"; // 読み込むファイル名
	};

	HRESULT LoadJson(void);			 // json読み込み関数

	std::vector<TEXDATA>m_pTextures; // テクスチャの動的配列
};