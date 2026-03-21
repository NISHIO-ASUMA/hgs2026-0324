//=========================================================
//
// 負けリザルトのオブジェクト管理処理 [ loseresultobject.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 負けリザルトオブジェクト管理クラスを定義
//*********************************************************
class CLoseResultObject
{
public:

	~CLoseResultObject();

	HRESULT Init(void);
	void Uninit(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CLoseResultObject* GetInstance(void);

private:

	CLoseResultObject();					// コンストラクタ

	static CLoseResultObject* m_pInstance;	// シングルトン変数
};