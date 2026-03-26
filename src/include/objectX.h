//=========================================================
//
// Xファイルオブジェクト基底クラス処理 [ objectX.h ]
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
// オブジェクトXクラスを定義
//*********************************************************
class CObjectX : public CObject
{
public:

	CObjectX(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CObjectX();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void); 
	void DrawShadow(void);
	void DrawOutLine(const D3DXVECTOR4& color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
	void SetFilePass(const char* pFilePass);
	void SetOutLineColor(const D3DXVECTOR4& color = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot = VECTOR3_NULL) { m_rot = rot; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_Scale = scale; }
	inline void SetMtxWorld(const D3DXMATRIX &mtxworld) { m_mtxWorld = mtxworld; }
	inline void SetCol(const D3DCOLORVALUE& color = V_COLOR_WHITE) { m_col = color; }
	inline void SetModelIdx(const int nIdx) { m_nIdxModel = nIdx; }
	inline void SetShadow(const bool &isFlags) { m_isShadow = isFlags; }
	inline void SetIsOutLine(const bool& isOutLine) { m_isOutLine = isOutLine; }
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; };
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; };
	inline D3DXMATRIX GetMtxWorld(void) const { return m_mtxWorld; }
	inline D3DCOLORVALUE GetCol(void) const { return m_col; }
	inline int GetModelIdx(void) const { return m_nIdxModel; }
	
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pModelName">モデルファイル名</param>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CObjectX* Create
	(
		const char* pModelName, 
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot = VECTOR3_NULL
	);

private:

	//******************************
	//　定数構造体宣言
	//*******************************
	struct Config
	{
		static constexpr float OUTLINE_SIZE = 2.0f; // アウトラインの幅
	};

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_Scale;	// 拡大率
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR4 m_OutLineColor; // アウトラインカラー
	D3DCOLORVALUE m_col;	// モデルカラー

	int m_nIdxModel;		// モデルインデックス
	bool m_isShadow;		// 影を出すかどうか
	bool m_isOutLine;		// アウトライン適用かどうか

};