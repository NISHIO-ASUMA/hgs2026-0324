//=========================================================
//
// jsonファイル読み込み管理処理 [ jsonmanager.h ]
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
#include <string>
#include <memory>
#include <unordered_map>

//*********************************************************
// 前方宣言
//*********************************************************
class CJsonCreator;
class CBlockManager;
//class CWorldWallManager;

//*********************************************************
// jsonを一括管理するクラスを定義
//*********************************************************
class CJsonManager
{
public:

	CJsonManager();
	~CJsonManager();

	HRESULT Load(const char* LoadFileName);
	HRESULT Init(void);
	void Uninit(void);

	void SetBlockManager(CBlockManager* pManager) { m_pBlockManager = pManager; }
	//void SetWorldWallManager(CWorldWallManager* pManager) { m_pWorldWallManager = pManager; }

	CBlockManager* GetBlockManager(void){ return m_pBlockManager; }
	//CWorldWallManager* GetWorldWallManager(void) { return m_pWorldWallManager; }

private:

	std::unordered_map<std::string, std::unique_ptr<CJsonCreator>> m_Creator; // 生成するオブジェクトのマップ配列

	CBlockManager* m_pBlockManager = nullptr;			// ブロックマネージャーのポインタ
	//CWorldWallManager* m_pWorldWallManager = nullptr;	// 世界の壁管理クラスのポインタ
};