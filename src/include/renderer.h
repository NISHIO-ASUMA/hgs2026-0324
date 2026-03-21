//========================================================
//
// レンダリング処理 [ renderer.h ]
// Author: Asuma Nishio
//
//========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <unordered_map>
#include <vector>
#include <string>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "debugproc.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CInstanceModel;

//*********************************************************
// レンダラークラスを定義
//*********************************************************
class CRenderer
{
public:

	//****************************
	// インスタンシング構造体
	//****************************
	struct InstanceData
	{
		D3DXMATRIX mtxworld; // ワールドマトリックス
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void OnWireFrame(void);
	void OffWireFrame(void);
	void SetSize(UINT uWidth, UINT uHeight) { m_Width = uWidth, m_Height = uHeight; }
	void GetFps(const int& nFps) { m_fps = nFps; }

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	UINT GetWidth(void) { return m_Width; }
	UINT GetHeight(void) { return m_Height; }
	static CDebugproc* GetDebug(void) { return m_pDebug; }

	/// <summary>
	/// インスタンシング登録関数
	/// </summary>
	/// <param name="nIdxModel">モデルインデックス</param>
	/// <param name="pModel">モデルポインタ</param>
	void AddInstanceObject(const int nIdxModel, CInstanceModel* pModel);

	/// <summary>
	/// 全インスタンシングオブジェクト描画関数
	/// </summary>
	/// <param name=""></param>
	void DrawInstancingAll(void);

	/// <summary>
	/// インスタンス配列内オブジェクトクリア関数
	/// </summary>
	/// <param name=""></param>
	void ClearDrawInstance(void) { m_RegisterInstObject.clear(); };

private:

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	static CDebugproc* m_pDebug;		// デバッグ
	int m_fps;							// fps
	bool m_isDeviceLost;				// デバイスロスト対処

	D3DPRESENT_PARAMETERS m_d3dpp;		// プレゼンテーションパラメータ
	UINT m_Width;						// 横幅
	UINT m_Height;						// 高さ

	LPDIRECT3DVERTEXBUFFER9 m_instanceVB;	// インスタンシング用頂点バッファ
	std::unordered_map<int, std::vector<CInstanceModel*>> m_RegisterInstObject = {}; // インスタンシング登録配列
};