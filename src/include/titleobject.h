//=========================================================
//
// タイトルで出現するオブジェクト管理処理 [ titleobject.h ]
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
//class CWorldWallManager;

//*********************************************************
// タイトルオブジェクト管理クラスを定義
//*********************************************************
class CTitleObject
{
public:

	~CTitleObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	
	static CTitleObject* GetInstance(void);

	//inline CWorldWallManager* GetWallManager(void) const { return m_pWallManager.get(); }
	
private:

	CTitleObject();					  // コンストラクタ

	static CTitleObject* m_pInstance; // シングルトン変数

	//std::unique_ptr<CWorldWallManager>m_pWallManager; // 壁管理クラスポインタ

};