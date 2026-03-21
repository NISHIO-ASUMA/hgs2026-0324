//=========================================================
//
// モデルファイル管理クラス [ xfilemanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "xfilemanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include "json.hpp"
#include <fstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "texture.h"

//*********************************************************
// 使用名前空間
//*********************************************************
using json = nlohmann::json;

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
int CXfileManager::m_nNumAll = NULL;

//=========================================================
// コンストラクタ
//=========================================================
CXfileManager::CXfileManager() : m_aFileData{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CXfileManager::~CXfileManager()
{
	// 全部の破棄
	UnLoad();
}
//=========================================================
// 全モデル読み込み
//=========================================================
HRESULT CXfileManager::Load(void)
{
	// jsonファイルロード
	LoadJson();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 全モデル破棄
//=========================================================
void CXfileManager::UnLoad(void)
{
	// 情報の破棄
	for (auto iter = m_aFileData.begin(); iter != m_aFileData.end(); iter++)
	{
		// メッシュの破棄
		if ((*iter).pMesh != nullptr)
		{
			(*iter).pMesh->Release();
			(*iter).pMesh = nullptr;
		}

		// マテリアルポインタの破棄
		if ((*iter).pBuffMat != nullptr)
		{
			(*iter).pBuffMat->Release();
			(*iter).pBuffMat = nullptr;
		}

		// テクスチャの破棄
		(*iter).pTexture.clear();

		// 動的ファイルパスの破棄
		if (!(*iter).FilePath.empty())
		{
			(*iter).FilePath.clear();
		}

		// マテリアルデータの破棄
		if ((*iter).dwNumMat != NULL)
		{
			(*iter).dwNumMat = 0;
		}
	}

	// 配列クリア
	m_aFileData.clear();
}
//=========================================================
// モデル登録処理
//=========================================================
int CXfileManager::Register(const char* pFileName)
{
	// すでに登録済みならそのインデックスを返す
	for (int nCnt = 0; nCnt < static_cast<int>(m_aFileData.size()); nCnt++)
	{
		if (m_aFileData[nCnt].FilePath == pFileName)
			return nCnt;
	}

	// 新規登録
	XFILEDATA newData = {};
	newData.FilePath = pFileName;

	//=======================================
	// モデル読み込み
	//=======================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice) return -1;

	// Xファイルをロードする
	HRESULT hr = D3DXLoadMeshFromX
	(
		pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&newData.pBuffMat,
		NULL,
		&newData.dwNumMat,
		&newData.pMesh
	);

	// 例外メッセージ
	if (FAILED(hr))
	{
		std::string msg = "モデル読み込みに失敗しました: ";
		msg += pFileName;
		MessageBox(GetActiveWindow(), msg.c_str(), "CXfileManager", MB_OK);
		return -1;
	}

	//=======================================
	// スムース法線生成処理
	//=======================================
	LPD3DXMESH pTempMesh = newData.pMesh;
	std::vector<DWORD> adjacency;

	// 隣接情報用バッファ確保
	DWORD FaceNum = pTempMesh->GetNumFaces();

	// バッファのサイズを動的生成
	adjacency.resize(FaceNum * 3);

	// 隣接情報生成
	pTempMesh->GenerateAdjacency(0.0001f, adjacency.data());

	// 法線生成
	D3DXComputeNormals(pTempMesh, adjacency.data());

	// 法線付きFVFでクローン
	DWORD fvf = pTempMesh->GetFVF();

	// 複製先ポインタ
	LPD3DXMESH pCloneMesh = nullptr;

	// クローンメッシュ作成
	HRESULT hrClone = pTempMesh->CloneMeshFVF
	(
		D3DXMESH_SYSTEMMEM,
		fvf | D3DFVF_NORMAL,
		CManager::GetInstance()->GetRenderer()->GetDevice(),
		&pCloneMesh
	);

	// クローン作製成功時
	if (SUCCEEDED(hrClone))
	{
		// 元メッシュ解放と法線情報の差し替え
		pTempMesh->Release();
		pTempMesh = pCloneMesh;

		// 隣接情報を再生成
		adjacency.clear();
	}

	// メッシュを差し替え
	newData.pMesh = pTempMesh;

	//=======================================
	// モデルサイズ算出
	//=======================================
	D3DXVECTOR3 Vtxmax = VECTOR3_NULL;
	D3DXVECTOR3 Vtxmin = VECTOR3_NULL;

	// メッシュ頂点数
	int nNumVtx = newData.pMesh->GetNumVertices();

	DWORD sizeFVF = D3DXGetFVFVertexSize(newData.pMesh->GetFVF());
	BYTE* pVtxBuff = nullptr;
	newData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ローカルポインタ
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (Vtx.x > Vtxmax.x) Vtxmax.x = Vtx.x;
		if (Vtx.y > Vtxmax.y) Vtxmax.y = Vtx.y;
		if (Vtx.z > Vtxmax.z) Vtxmax.z = Vtx.z;

		if (Vtx.x < Vtxmin.x) Vtxmin.x = Vtx.x;
		if (Vtx.y < Vtxmin.y) Vtxmin.y = Vtx.y;
		if (Vtx.z < Vtxmin.z) Vtxmin.z = Vtx.z;

		pVtxBuff += sizeFVF;
	}

	// アンロック
	newData.pMesh->UnlockVertexBuffer();

	// 矩形のサイズ生成
	newData.Size.x = Vtxmax.x - Vtxmin.x;
	newData.Size.y = Vtxmax.y - Vtxmin.y;
	newData.Size.z = Vtxmax.z - Vtxmin.z;

	//=======================================
	// テクスチャ登録
	//=======================================
	newData.pTexture.clear();
	newData.pTexture.resize(newData.dwNumMat);

	if (newData.pBuffMat)
	{
		// マテリアルデータのポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)newData.pBuffMat->GetBufferPointer();

		// テクスチャポインタ取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(newData.dwNumMat); nCnt++)
		{
			if (pMat[nCnt].pTextureFilename)
			{
				// テクスチャID登録
				int TexID = pTexture->Register(pMat[nCnt].pTextureFilename);
				newData.pTexture[nCnt] = TexID;
			}
			else
			{
				// 初期値をセット
				newData.pTexture[nCnt] = -1;
			}
		}
	}

	// 配列に登録する
	m_aFileData.push_back(newData);

	// 登録数を更新して返す
	return m_nNumAll++; 
}
//=========================================================
// jsonファイル読み込み
//=========================================================
HRESULT CXfileManager::LoadJson(void)
{
	// ファイルオープン
	std::ifstream openfile(LOAD_NAME);

	// 例外処理
	if (!openfile.is_open())
	{
		MessageBox(GetActiveWindow(), "XFile.json が開けません", "xfilemanager", MB_OK);
		return E_FAIL;
	}

	// json読み込み
	json jsonload;
	openfile >> jsonload;
	openfile.close();

	// 情報がなかったら
	if (jsonload.is_null() || jsonload.empty())
	{
		MessageBox(GetActiveWindow(), "XFile.json にデータがありません", "xfilemanager", MB_OK);
		return E_FAIL;
	}

	// 配列クリア
	m_aFileData.clear();

	// 情報をセットしていく
	for (const auto& entry : jsonload)
	{
		// 文字がなかったら
		if (!entry.contains("XFile") || entry["XFile"].is_null())
			continue;

		// ファイルパス取得
		std::string filePath = entry["XFile"].get<std::string>();

		if (filePath.empty())
			continue;

		// モデル読み込み
		LoadModel(filePath.c_str());

		// 総数加算
		m_nNumAll++;
	}

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// モデルの部分読み込み処理
//=========================================================
void CXfileManager::LoadModel(const char* pModelName)
{
	// 新規登録
	XFILEDATA newData = {};
	newData.FilePath = pModelName;

	//=======================================
	// モデル読み込み
	//=======================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	if (!pDevice)
		return;

	// モデル読み込み
	HRESULT hr = D3DXLoadMeshFromX
	(
		pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&newData.pBuffMat,
		NULL,
		&newData.dwNumMat,
		&newData.pMesh
	);

	// 例外設定
	if (FAILED(hr))
	{
		std::string msg = "モデル読み込みに失敗しました: ";
		msg += pModelName;
		MessageBox(GetActiveWindow(), msg.c_str(), "CXfileManager", MB_OK);
		return;
	}

	//=======================================
	// スムース法線生成処理
	//=======================================
	LPD3DXMESH pTempMesh = newData.pMesh; // 一時メッシュ
	std::vector<DWORD> adjacency;

	// 隣接情報用バッファ確保
	DWORD FaceNum = pTempMesh->GetNumFaces();

	// バッファのサイズを動的生成
	adjacency.resize(FaceNum * 3);

	// 隣接情報生成
	pTempMesh->GenerateAdjacency(0.0001f, adjacency.data());

	// 法線生成
	D3DXComputeNormals(pTempMesh,adjacency.data());

	// 法線付きFVFでクローン
	DWORD fvf = pTempMesh->GetFVF();

	// 複製先ポインタ
	LPD3DXMESH pCloneMesh = nullptr;

	// クローンメッシュ作成
	HRESULT hrClone = pTempMesh->CloneMeshFVF
	(
		D3DXMESH_SYSTEMMEM,
		fvf | D3DFVF_NORMAL,
		CManager::GetInstance()->GetRenderer()->GetDevice(),
		&pCloneMesh
	);

	// クローン作製成功時
	if (SUCCEEDED(hrClone))
	{
		// 元メッシュ解放＆差し替え
		pTempMesh->Release();
		pTempMesh = pCloneMesh;

		// 隣接情報を再生成
		adjacency.clear();
	}
	
	// メッシュを差し替え
	newData.pMesh = pTempMesh;

	//=======================================
	// モデルサイズ算出
	//=======================================
	D3DXVECTOR3 Vtxmax = VECTOR3_NULL;
	D3DXVECTOR3 Vtxmin = VECTOR3_NULL;

	// 頂点数の取得
	int nNumVtx = newData.pMesh->GetNumVertices();

	DWORD sizeFVF = D3DXGetFVFVertexSize(newData.pMesh->GetFVF());
	BYTE* pVtxBuff = nullptr;
	newData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// ローカルポインタ
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (Vtx.x > Vtxmax.x) Vtxmax.x = Vtx.x;
		if (Vtx.y > Vtxmax.y) Vtxmax.y = Vtx.y;
		if (Vtx.z > Vtxmax.z) Vtxmax.z = Vtx.z;

		if (Vtx.x < Vtxmin.x) Vtxmin.x = Vtx.x;
		if (Vtx.y < Vtxmin.y) Vtxmin.y = Vtx.y;
		if (Vtx.z < Vtxmin.z) Vtxmin.z = Vtx.z;

		pVtxBuff += sizeFVF;
	}

	// アンロック
	newData.pMesh->UnlockVertexBuffer();

	// サイズ
	newData.Size.x = Vtxmax.x - Vtxmin.x;
	newData.Size.y = Vtxmax.y - Vtxmin.y;
	newData.Size.z = Vtxmax.z - Vtxmin.z;

	//===============================================================
	// テクスチャ登録
	//===============================================================
	newData.pTexture.clear();
	newData.pTexture.resize(newData.dwNumMat);

	if (newData.pBuffMat)
	{
		// マテリアルポインタ
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)newData.pBuffMat->GetBufferPointer();

		// テクスチャクラスポインタ取得
		CTexture* pTexture = CManager::GetInstance()->GetTexture();

		for (int nCnt = 0; nCnt < static_cast<int>(newData.dwNumMat); nCnt++)
		{
			// nullチェック
			if (pMat[nCnt].pTextureFilename)
			{
				// テクスチャインデックスセット
				int nTexIdx = pTexture->Register(pMat[nCnt].pTextureFilename);

				// 配列に登録
				newData.pTexture[nCnt] = nTexIdx;
			}
			else
			{
				// -1で初期化
				newData.pTexture[nCnt] = -1;
			}
		}
	}

	// 登録完了
	m_aFileData.push_back(newData);
}