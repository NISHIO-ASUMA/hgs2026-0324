//=========================================================
//
// ターゲットのポイント管理処理 [ walltargetmanager.h ]
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
class CWallTargetPoint;

//*********************************************************
// ターゲットポイントクラスを定義
//*********************************************************
class CWallTargetManager
{
public:

	~CWallTargetManager();

	HRESULT Init(const char* pLoadName);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pTargetList.size()); }

	/// <summary>
	/// 配列の中のポイントを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CWallTargetPoint* GetIdxPoint(const int& nIdx) { return m_pTargetList[nIdx]; }

	/// <summary>
	/// 生成関数
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="rot"></param>
	/// <param name="scale"></param>
	/// <param name="pModelName"></param>
	/// <returns></returns>
	CWallTargetPoint* CreateManager
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
	static CWallTargetManager* GetInstance(void)
	{
		static CWallTargetManager Instance;
		return &Instance;
	}

private:

	CWallTargetManager();						// コンストラクタ
	void LoadJson(const char* pLoadName);		// json読み込み

private:

	std::vector<CWallTargetPoint*>m_pTargetList;	// 動的配列
};