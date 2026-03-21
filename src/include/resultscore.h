//=========================================================
//
// リザルトに表示するスコアの処理 [ resultscore.h ]
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
#include <array>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CLoad;
class CNumber;

//*********************************************************
// リザルトに出すスコアを表示するクラスを定義
//*********************************************************
class CResultScore : public CObject
{
public:

	CResultScore(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CResultScore();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Save(void);
	void SetAnimScore(const int& nDestScore);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CResultScore* Create
	(
		const D3DXVECTOR3& pos, 
		const float fWidth, 
		const float fHeight
	);

private:

	void UpdateAnimScore(void);		// アニメーションスコア更新関数

	//********************************
	// 定数構造体宣言
	//********************************
	struct Config
	{
		static constexpr int NUM_SCORE			= 8;							// 表示桁数
		static constexpr int NUM_DIGIT			= 10;							// 計算桁数
		static constexpr int WRITE_SCORE		= 5;							// 書き出し数
		static constexpr int MAX_ANIMTIME		= 180;							// 最大継続時間
		static constexpr float POSX_VALUE		= 2.0f;							// X座標加算値
		static constexpr const char* TEXNAME	= "time.png";					// テクスチャ名
		static constexpr const char* SAVEFILE	= "data/SCORE/ResultScore.bin"; // 書き出すファイル名

	};

	std::array<CNumber*,Config::NUM_SCORE>m_pNumber;		// 桁数分のナンバーのポインタ配列
	std::unique_ptr<CLoad>m_pLoad;							// 読み込みクラスのユニークポインタ

	D3DXVECTOR3 m_pos;	// 座標
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ

	int m_nLoadScore;	// 読み取ったスコア
	int m_nCurrentScore;// 今の表示スコア
	int m_nStartScore;	// 開始時のスコア
	int m_nTargetScore;	// ゴールのスコア
	int m_nTimer;		// 経過フレーム
	int m_nDuration;	// 目的の時間
	bool m_isUse;		// 使用しているかどうか
};