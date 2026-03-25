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
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pEnemyList.size()); }

	/// <summary>
	/// 配列の中の敵を取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CEnemy* GetIdxEnemy(const int& nIdx) { return m_pEnemyList[nIdx]; }

	/// <summary>
	/// 配列から消去する処理
	/// </summary>
	/// <param name="pthis"></param>
	void Erase(CEnemy* pthis);

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

	CEnemyManager();					// コンストラクタ

private:

	std::vector<CEnemy*>m_pEnemyList;	// 敵の動的配列
};