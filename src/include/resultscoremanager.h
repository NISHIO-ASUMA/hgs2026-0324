//=========================================================
//
// リザルト用スコア管理処理 [ resultscoremanager.h ]
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
#include <array>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CResultScore;
class CLoad;

//*********************************************************
// リザルトスコア管理クラスを定義
//*********************************************************
class CResultScoreManager
{
public:

	~CResultScoreManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Load(void);

	/// <summary>
	/// 各番号のスコアを取得
	/// </summary>
	/// <param name="nidx">取得番号</param>
	/// <returns></returns>
	CResultScore* GetIdx(const int& nidx) { return m_pResultScore[nidx]; }

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CResultScoreManager* GetInstancce(void);

private:

	CResultScoreManager();

	//***************************
	// 定数格納構造体
	//***************************
	struct Config
	{
		static constexpr int IDX_FEED			  = 0;			// 餌スコアインデックス
		static constexpr int IDX_QUEEN			  = 1;			// 防衛スコアインデックス
		static constexpr int IDX_ALL			  = 2;			// 最終スコアインデックス
		static constexpr int SCORELISTNUM		  = 3;			// 配列の最大数
		static constexpr int MAX_MATHSCORE		  = 2000000;	// 最大加算値
		static constexpr int QUEEN_HP			  = 200;		// 基準の体力値
		static constexpr const char* EATSCORE	  = "data/SCORE/EnemyScore.bin";	
		static constexpr const char* POINTSCORE = "data/SCORE/GameScore.bin";	
	};	

	int MathRateScore(void); // 計算関数

	int m_nEatScore;		// 食べた数のスコアを格納
	int m_nPointScore;		// ポイントスコアを格納
	int m_nLastScore;		// 最終スコアを格納
	int m_nMathScore;		// 計算用スコア

	std::array<CResultScore*, Config::SCORELISTNUM>m_pResultScore;	// リザルトスコアポインタ
	std::unique_ptr<CLoad>m_pLoad;									// ロードクラスのポインタ
};