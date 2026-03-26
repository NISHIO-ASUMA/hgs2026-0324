//=========================================================
//
// ロックオンのビルボード [ lockonui.h ]
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
#include "billboard.h"

//*********************************************************
// ターゲットのポイント単体のクラスの定義
//*********************************************************
class CLockOnUi : public CBillboard
{
public:

	CLockOnUi(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CLockOnUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	
	void SetIsDraw(const bool& isDraw) { m_isDraw = isDraw; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">モデルパス</param>
	/// <returns></returns>
	static CLockOnUi * Create
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR2& size,
		const char* TexName
	);

private:
	bool m_isDraw; // 描画フラグ
};