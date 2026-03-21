//=========================================================
//
// 移動可能な3Dオブジェクト処理 [ move3dobject.h ]
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
// 動く3Dポリゴンオブジェクトクラスを定義
//*********************************************************
class CMove3DObject : public CObject
{
public:

	CMove3DObject(int nPriority = static_cast<int>(CObject::PRIORITY::MOVE3D));
	~CMove3DObject();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UpdatePosition(void);
	void DecayMove(const float fValue);
	void SetTexture(const char* pTexName);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetPosOld(const D3DXVECTOR3& oldpos) { m_posOld = oldpos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	inline void SetCol(const D3DXCOLOR& col = COLOR_WHITE) { m_col = col; }
	inline void SetSize(float fWidth, float fHeight) { m_fWidth = fWidth, m_fHeight = fHeight; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetOldPos(void) const { return m_posOld; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXVECTOR3 GetMove(void) const { return m_move; }
	inline D3DXCOLOR GetCol(void) const { return m_col; }
	inline float GetWidth(void) const { return m_fWidth; }
	inline float GetHeight(void) const { return m_fHeight; }

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ

	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;		// 座標情報
	D3DXVECTOR3 m_posOld;	// 1フレーム前の座標情報
	D3DXVECTOR3 m_move;		// 移動情報
	D3DXVECTOR3 m_rot;		// 角度情報
	D3DXCOLOR m_col;		// 色情報

	int m_nIdxTexture;		// テクスチャインデックス
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
};