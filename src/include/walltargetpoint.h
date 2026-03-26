//=========================================================
//
// ワープターゲットのポイントクラス [ walltargetpoint.h ]
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
class CSphereCollider;
class CLockOnUi;

//*********************************************************
// ターゲットのポイント単体のクラスの定義
//*********************************************************
class CWallTargetPoint : public CObjectX
{
public:

	CWallTargetPoint(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CWallTargetPoint();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline CSphereCollider* GetCollider(void) { return m_pCollider.get(); }
	CLockOnUi* GetLockUi(void) { return m_pLockOnUi; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CWallTargetPoint* Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

private:
	std::unique_ptr<CSphereCollider> m_pCollider;	// 矩形のコライダー
	CLockOnUi* m_pLockOnUi;							// ビルボード
};