//=========================================================
//
// メッシュドーム処理 [ meshdome.h ]
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
// メッシュドームクラスを定義
//*********************************************************
class CMeshDome : public CObject
{
public:

	CMeshDome(int nPriority = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshDome();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetTexture(void);

	D3DXVECTOR3 GetPos(void) { return m_pos; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="nRadius">半径</param>
	/// <returns></returns>
	static CMeshDome* Create
	(
		const D3DXVECTOR3& pos,
		const float& nRadius
	);

private:

	static constexpr int MeshDome_X_BLOCK = 20;		// Xブロック数
	static constexpr int MeshDome_Z_BLOCK = 20;		// Zブロック数
	static constexpr int MeshDome_VERTEX_NUM = (MeshDome_X_BLOCK + 1) * (MeshDome_Z_BLOCK + 1); // 頂点数
	static constexpr int MeshDome_INDEX_NUM = MeshDome_X_BLOCK * MeshDome_Z_BLOCK * 6;			// インデックス数
	static constexpr int MeshDome_PRIMITIVE_NUM = (MeshDome_X_BLOCK * MeshDome_Z_BLOCK * 2) + ((MeshDome_Z_BLOCK - 1) * 2); // ポリゴン数

	LPDIRECT3DINDEXBUFFER9 m_pIdx; // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// 頂点バッファ

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	float m_nRadius;		// 半径
	int m_nTexIdx;			// テクスチャインデックス
};