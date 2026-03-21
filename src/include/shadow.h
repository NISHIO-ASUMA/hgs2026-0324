//=========================================================
//
// 影処理 [ shadow.h ]
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
#include "object3D.h"

//*********************************************************
// 影オブジェクトクラスを定義
//*********************************************************
class CShadow : public CObject3D
{
public:

	CShadow(int nPriority = static_cast<int>(CObject::PRIORITY::SHADOW));
	~CShadow();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void UpdatePos(const D3DXVECTOR3& pos) { SetPos(pos); }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CShadow* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:

	//*****************************
	// 定数構造体
	//*****************************
	struct Config
	{
		static constexpr float SIZE			 = 30.0f;			// 大きさ
		static constexpr const char* TEXNAME = "shadow00.jpg";  // テクスチャ名
	};
};