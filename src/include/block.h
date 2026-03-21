//=========================================================
//
// ブロック処理 [ block.h ]
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
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "objectX.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// ブロックオブジェクトクラスを定義
//*********************************************************
class CBlock : public CObjectX
{
public:

	CBlock(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CBlock();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	bool Collision(CBoxCollider * pOther,D3DXVECTOR3 * OutPos);
	inline CBoxCollider* GetCollider(void) { return m_pCollider.get(); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CBlock* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		const D3DXVECTOR3& scale, 
		const char* pModelName
	);

private:
	std::unique_ptr<CBoxCollider> m_pCollider;	// 矩形のコライダー
};

