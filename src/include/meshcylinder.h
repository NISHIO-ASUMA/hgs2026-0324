//=========================================================
//
// メッシュシリンダー処理 [ meshcylinder.h ]
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
// メッシュシリンダークラスを定義
//*********************************************************
class CMeshCylinder : public CObject
{
public:

	//****************************
	// 構造体定義
	//****************************
	struct Cylinder
	{
		int nTexIdx;		// テクスチャインデックス
		int nNumPrimitive;	// プリミティブ数
		int nNumIdx;		// インデックス数
		int nNumAllVtx;		// 全体頂点数
		int nNumDigitX;		// X平面分割数
		int nNumDigitZ;		// Z平面分割数
		float fRadius;		// 半径
	};

	CMeshCylinder(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CMeshCylinder();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void SetTexture(void);

	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetEndPos(const D3DXVECTOR3& endpos) { m_EndPos = endpos; }

	inline D3DXVECTOR3 GetPos(void) { return m_pos; }
	inline float GetRadius(void) const { return m_Cylinder.fRadius; }

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fRadius">半径</param>
	/// <returns></returns>
	static CMeshCylinder* Create
	(
		const D3DXVECTOR3& pos, 
		const float& fRadius
	);

private:

	static constexpr int DIGIT_X = 20;		// X方向の分割数
	static constexpr int DIGIT_Z = 1;		// Z方向への分割数

	LPDIRECT3DINDEXBUFFER9 m_pIdx; // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// 頂点バッファ

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_EndPos;	// 終端座標
	Cylinder m_Cylinder;	// 構造体変数
};