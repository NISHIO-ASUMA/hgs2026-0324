//=========================================================
//
// チュートリアルのオブジェクト管理 [ tutorialobject.h ]
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
// 前方宣言
//*********************************************************
class CBlockManager;
class CTutoTopAnt;
class CTutoArrayAnt;
class CBlock;
class CFeed;
class CEventArea;
//class CWorldWallManager;

//*********************************************************
// チュートリアルで使うオブジェクトを管理するクラスを定義
//*********************************************************
class CTutorialObject
{
public:

	~CTutorialObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	inline CBlockManager* GetBlockManager(void) const { return m_pBlockManager.get(); }
	inline CTutoArrayAnt* GetArrayAnt(void) { return m_pArrayAnt; }
	inline CTutoTopAnt* GetTutoTopAnt(void) { return m_pTopAnt; }
	inline CFeed* GetTutoFeed(void) { return m_pFeed; }

	static CTutorialObject* GetInstance(void);

private:

	CTutorialObject();

	static CTutorialObject* m_pInstance;			// シングルトン変数

	std::unique_ptr<CBlockManager>m_pBlockManager;	// ブロック管理クラス
	//std::unique_ptr<CWorldWallManager>m_pWorldWall;	// 壁管理クラス

	CTutoTopAnt* m_pTopAnt;							// トップアリ
	CTutoArrayAnt* m_pArrayAnt;						// 仲間の黒アリ
	CFeed* m_pFeed;									// 餌のポインタ
};