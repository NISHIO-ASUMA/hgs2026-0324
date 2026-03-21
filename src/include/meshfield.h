//=========================================================
//
// メッシュフィールド処理 [ meshfield.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// メッシュフィールドクラスを定義
//*********************************************************
class CMeshField : public CObject
{
public:

	//**************************
	// 構造体宣言
	//**************************
	struct MeshField
	{
		float fRadius;		// X方向の半径
		float fRadiusZ;		// Z方向の半径
		int nNumPrimitive;	// プリミティブ数
		int nNumIdx;		// インデックス数
		int nNumAllVtx;		// 全体頂点数
		int nNumX;			// X頂点
		int nNumZ;			// Z頂点
		int nTexIdx;		// テクスチャインデックス
	};

	CMeshField(int nPriority = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshField();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetTexture(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRaiusZ(float fRadius) { m_MeshFiled.fRadiusZ = fRadius; }
	void SetRaiusX(float fRadius) { m_MeshFiled.fRadius = fRadius; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetRadiusX(void) { return m_MeshFiled.fRadius; }
	float GetRadiusZ(void) { return m_MeshFiled.fRadiusZ; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fRadiusX">半径</param>
	/// <param name="fRadiusZ">半径</param>
	/// <param name="nNumX">Xの分割数</param>
	/// <param name="nNumZ">Zの分割数</param>
	/// <returns></returns>
	static CMeshField* Create
	(
		const D3DXVECTOR3& pos,
		const float& fRadiusX,
		const float& fRadiusZ,
		const int& nNumX,
		const int& nNumZ
	);

private:

	LPDIRECT3DINDEXBUFFER9 m_pIdx; // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// 頂点バッファ

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	MeshField m_MeshFiled;	// 構造体変数
};