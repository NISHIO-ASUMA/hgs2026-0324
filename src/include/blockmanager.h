//=========================================================
//
// ブロック管理処理 [ blockmanager.h ]
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
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "block.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;

//*********************************************************
// ブロック一括管理クラスを定義
//*********************************************************
class CBlockManager
{
public:

	CBlockManager();
	~CBlockManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	HRESULT Load(void);

	/// <summary>
	/// 配列のサイズを取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) { return static_cast<int>(m_pBlocks.size()); }

	/// <summary>
	/// 配列の中のブロックを取得する
	/// </summary>
	/// <param name="nIdx">取得する配列番号</param>
	/// <returns></returns>
	inline CBlock* GetBlock(const int nIdx) { return m_pBlocks[nIdx]; }

	/// <summary>
	/// ブロック生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">角度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="pModelName">生成ファイル名</param>
	/// <returns></returns>
	CBlock* CreateManager
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		const D3DXVECTOR3& scale, 
		const char* pModelName
	);

private:

	static constexpr const char* FILE_NAME = "data/JSON/Gamestage.json";	// ファイル名
	std::vector<CBlock*>m_pBlocks;											// 配置するブロックの動的配列
};