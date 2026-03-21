//=========================================================
//
// 負けリザルトシーン管理処理 [ loseresultmanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 負けリザルトシーン管理クラスを定義
//*********************************************************
class CLoseResultManager
{
public:

	~CLoseResultManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CLoseResultManager* GetInstance(void);

private:

	CLoseResultManager();	// 格納コンストラクタ
};