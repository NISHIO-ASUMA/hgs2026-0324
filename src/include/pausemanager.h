//=========================================================
//
// ポーズ管理処理 [ pausemanager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 前方宣言
//*********************************************************
class CPause;

//*********************************************************
// ポーズメニュー管理クラスを定義
//*********************************************************
class CPauseManager
{
public:

	CPauseManager();
	~CPauseManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	/// <summary>
	/// ポーズの有効状態を切り替える
	/// </summary>
	/// <param name="isFlags">引数で設定する値</param>
	static void SetEnablePause(bool isFlags) { m_isPause = isFlags; }

	/// <summary>
	/// ポーズの状態を判別する
	/// </summary>
	/// <param name=""></param>
	void SetEnablePause(void);

	/// <summary>
	/// 現在のフラグを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static bool GetPause(void) { return m_isPause; }

private:

	static constexpr int SELECT_BEGIN = 1;	// メニュー開始番号
	static constexpr int SELECT_END = 3;	// メニュー終了番号
	static constexpr int SELECT_MAX = 4;	// 最大生成数

	static bool m_isPause;			// ポーズかどうか
	int m_nSelectIdx;				// セレクト番号
	CPause* m_pPause[SELECT_MAX];	// ポーズクラスのポインタ配列
};