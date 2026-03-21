//=========================================================
//
// モデルファイル管理クラス [ xfilemanager.h ]
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
// 管理クラスを定義
//*********************************************************
class CXfileManager
{
public:

	//*************************
	// Xファイルデータ構造体
	//*************************
	struct XFILEDATA
	{
		D3DXVECTOR3 Size;			// モデルのサイズ
		LPD3DXMESH pMesh;			// メッシュのポインタ
		LPD3DXBUFFER pBuffMat;		// マテリアルのポインタ
		DWORD dwNumMat;				// マテリアル数
		std::vector<int>pTexture;	// テクスチャの動的ポインタ
		std::string FilePath;		// ファイル名
	};

	CXfileManager();
	~CXfileManager();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// Xファイルのモデルの登録をする
	/// </summary>
	/// <param name="pFileName">登録するモデル名</param>
	/// <returns></returns>
	int Register(const char* pFileName);

	/// <summary>
	/// 配列番号を指定して情報を取得
	/// </summary>
	/// <param name="nIdx">取得するオブジェクトインデックス</param>
	/// <returns>インデックス番号に応じたデータ</returns>
	XFILEDATA GetInfo(const int& nIdx) { return m_aFileData[nIdx]; }

	/// <summary>
	/// 動的配列の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>配列ポインタ</returns>
	const std::vector<XFILEDATA>& GetList(void) { return m_aFileData; }

private:

	HRESULT LoadJson(void);					// jsonfile読み込み関数
	void LoadModel(const char* pModelName); // モデル登録関数

	std::vector<XFILEDATA>m_aFileData;		// モデルデータ管理配列

	static int m_nNumAll;					// モデルの総数
	static constexpr const char* LOAD_NAME = "data/JSON/XFile.json"; // jsonファイル名
};