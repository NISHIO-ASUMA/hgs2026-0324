//=========================================================
//
// ステンシルシャドウ処理 [ shadowstencil.h ]
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
#include "objectX.h"

//*********************************************************
// ステンシルシャドウクラスを定義
//*********************************************************
class CShadowStencil :public CObjectX
{
public:

	CShadowStencil(int nPriority = static_cast<int>(CObject::PRIORITY::SHADOW));
	~CShadowStencil();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CShadowStencil* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtx;		// 頂点バッファ

	static constexpr const char* MODELPATH = "STAGEOBJ/Shadowmodel.x"; // モデルパス
};