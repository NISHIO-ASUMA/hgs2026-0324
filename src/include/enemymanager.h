//=========================================================
//
// 敵の管理処理 [ enemymanager.h ]
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
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CEnemy;

//*********************************************************
// 敵管理クラスを定義
//*********************************************************
class CEnemyManager
{
public:

	~CEnemyManager();

	HRESULT Init(const char* pLoadName);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	/// <param name="pModelName"></param>
	/// <returns></returns>
	CEnemy* CreateManager
	(
		const D3DXVECTOR3& pos,
		const D3DXVECTOR3& rot,
		const D3DXVECTOR3& scale,
		const char* pModelName
	);

	/// <summary>
	/// シングトン取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CEnemyManager* GetInstance(void)
	{
		static CEnemyManager Instance;
		return &Instance;
	}

private:

	CEnemyManager(); // コンストラクタ

private:

	std::vector<CEnemy*>m_pEnemyList; // 敵の動的配列
};