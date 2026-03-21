//=========================================================
//
// テクスチャ処理 [ texture.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "texture.h"

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "json.hpp"
#include <fstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json;

//=========================================================
// コンストラクタ
//=========================================================
CTexture::CTexture() : m_pTextures{}
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CTexture::~CTexture()
{
	// すべての破棄
	UnLoad();
}
//=========================================================
// テクスチャ読み込み
//=========================================================
HRESULT CTexture::Load(void)
{
	// 配列のクリア
	m_pTextures.clear();

	// json適用
	LoadJson();

	return S_OK;
}
//=========================================================
// テクスチャ破棄
//=========================================================
void CTexture::UnLoad(void)
{
	// 要素の破棄
	for (auto& tex : m_pTextures)
	{
		if (tex.TexData)
		{
			tex.TexData->Release();
			tex.TexData = nullptr;
		}
	}

	// 配列のクリア
	m_pTextures.clear();
}
//=========================================================
// テクスチャ番号を登録する
//=========================================================
int CTexture::Register(const char* pFileName)
{
	// もしファイル名がないなら
	if (!pFileName) return -1;

	// すでにあるかチェック
	for (int nCnt = 0; nCnt < m_pTextures.size(); nCnt++)
	{
		// 登録されているならインデックスを返す
		if (m_pTextures[nCnt].TexName == pFileName)
			return nCnt;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 登録用の空の構造体変数
	TEXDATA data = {};

	// テクスチャの作成に成功したら
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, pFileName, &data.TexData)))
	{
		// テクスチャ名を設定
		data.TexName = pFileName;

		// 配列に追加
		m_pTextures.push_back(data);

		// インデックス番号を返す
		return static_cast<int>(m_pTextures.size())- 1;
	}

	return -1;
}
//=========================================================
// テクスチャ番号取得
//=========================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	// 例外処理
	if (nIdx < NULL || nIdx >= static_cast<int>(m_pTextures.size())) return nullptr;

	// テクスチャ番号を取得
	return m_pTextures[nIdx].TexData;
}
//=========================================================
// jsonファイル読み込み
//=========================================================
HRESULT CTexture::LoadJson(void)
{
	// JSONファイルを開く
	std::ifstream ifs(Config::LOAD_NAME);

	// ファイルが開けなかったら
	if (!ifs.is_open())
	{
		MessageBox(GetActiveWindow(), "jsonファイルを開けません", "エラー", MB_OK);
		return E_FAIL;
	}

	// JSON読み込み
	json jsondata;
	ifs >> jsondata;

	// 配列のクリア
	m_pTextures.clear();

	for (const auto& texture : jsondata)
	{
		// "Texture"が見つからなかったら
		if (!texture.contains("Texture")) continue;

		// 文字列を取得
		std::string file = texture["Texture"].get<std::string>();
		if (file.empty()) continue;

		// 登録する
		Register(file.c_str());
	}

	return S_OK;
}