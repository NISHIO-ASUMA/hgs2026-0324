//=========================================================
//
// スコア処理 [ score.h ]
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
#include "number.h"
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CLoad;

//*********************************************************
// スコアクラスを定義
//*********************************************************
class CScore : public CObject
{
public:

	CScore(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CScore();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void AddScore(int nValue);
	void DeleteScore(void);
	void SaveScore(void);

	inline void SetScore(const int& nDestScore) { m_nScore = nDestScore; }
	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	inline int GetScore(void) const { return m_nScore; }
	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CScore* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth = 100.0f,
		const float& fHeight = 100.0f
	);

private: 

	//*************************
	// 定数構造体
	//*************************
	struct Config
	{
		static constexpr int NUM_SCORE			= 8;							// 桁数
		static constexpr int NUM_DIGIT			= 10;							// 表示する分割桁数の値
		static constexpr float DIGIT_VALUE		= 2.0f;							// 加算量
		static constexpr const char* SAVE_NAME	= "data/SCORE/GameScore.bin";	// 書き出しファイル名
		static constexpr const char* TEX_NAME	= "number003.png";				// テクスチャ名
	};

	D3DXCOLOR m_col;	// カラー
	D3DXVECTOR3 m_pos;	// 座標
	D3DXVECTOR3 m_rot;	// 角度
	
	int m_nScore;		// スコア保持用
	float m_fWidth;		// 横幅
	float m_fHeight;	// 高さ

	std::array<CNumber*, Config::NUM_SCORE>m_apNumber;	// 桁数分のナンバーのポインタ
	std::unique_ptr<CLoad>m_pLoad;						// ユニークポインタ
};
