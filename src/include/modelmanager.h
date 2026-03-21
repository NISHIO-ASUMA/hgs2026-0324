//=========================================================
//
// キャラクターモデルファイル管理クラス [ modelmanager.h ]
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
class CModelManager
{
public:

	//********************************
	// インスタンシングデータ構造体
	//********************************
	struct MODEL_DATA
	{
		LPDIRECT3DVERTEXBUFFER9 VtxBuffer;		// 頂点バッファ
		LPDIRECT3DINDEXBUFFER9  IndexBuffer;	// インデックスバッファ
		UINT vtxCount;							// 頂点カウント
		UINT PrimCount;							// ポリゴンカウント
	};

	//********************************
	// 読み込み用構造体
	//********************************
	struct ModelManagerInfo
	{
		LPD3DXMESH pMesh;			// メッシュのポインタ
		LPD3DXBUFFER pBuffMat;		// マテリアルのポインタ
		DWORD dwNumMat;				// マテリアル数
		std::vector<int>pTexture;	// テクスチャの動的ポインタ
		std::string FilePath;		// ファイル名
		int nParnent;				// 親を持つ
		bool isInstancing;			// インスタンシング判断
		MODEL_DATA modelData;		// 構造体変数
	};


	CModelManager();
	~CModelManager();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// モデルの登録
	/// </summary>
	/// <param name="pModelName">モデル名</param>
	/// <returns></returns>
	int Register(const char* pModelName);

	/// <summary>
	/// 配列番号を指定して情報を取得
	/// </summary>
	/// <param name="nIdx">取得するオブジェクトインデックス</param>
	/// <returns>インデックス番号に応じたデータ</returns>
	inline const ModelManagerInfo GetInfo(int nIdx) const { return m_aModelData[nIdx]; }

	/// <summary>
	/// 動的配列の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>配列ポインタ</returns>
	inline const std::vector<ModelManagerInfo>& GetList(void) const { return m_aModelData; }

private:

	HRESULT LoadJson(void);						// jsonfile読み込み関数
	void LoadModel(const char* pModelName,bool &LoadFlags); // モデル登録関数

	std::vector<ModelManagerInfo>m_aModelData;	// モデルデータ管理配列
	static int m_nNumAll;						// モデルの総数

	static constexpr const char* LOAD_FILE = "data/JSON/Model.json"; // 読み込み対象ファイル名
};