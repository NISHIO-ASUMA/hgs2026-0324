//=========================================================
//
// ゲームタイマー処理 [ gametime.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CNumber;

//*********************************************************
// タイムクラスを定義
//*********************************************************
class CGameTime : public CObject
{
public:

	CGameTime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CGameTime();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetWidth(const float& fWidth) { m_fWidth = fWidth; }
	inline void SetHeight(const float& fHeight) { m_fHeight = fHeight; }

	inline int GetToAll(void) const { return m_nAllTime; }
	inline int GetToCurrent(void) const { return m_nSecond; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <returns></returns>
	static CGameTime* Create
	(
		const D3DXVECTOR3& pos,
		const float& fWidth = 60.0f,
		const float& fHeight = 40.0f);

private:

	//************************************
	// 定数構造体宣言
	//************************************
	struct Config
	{
		static constexpr int DIGIT_TIME			= 2;		  // 桁数
		static constexpr int NUMTIME			= 90;		  // 最大タイマー
		static constexpr int CARVETIME			= 60;		  // カウント上限
		static constexpr int DIVIDE				= 10;		  // 桁分割の値
		static constexpr float VALUE_WIDTH		= 150.0f;	  // 横幅の増加分
		static constexpr float VALUE_FLOAT		= 2.0f;		  // ずらす基準値
		static constexpr const char* TEXNAME	= "time.png"; // テクスチャ名
	};

	void Second(void);		// 秒計算
	void Minute(void);		// 分計算

	int m_nAllTime;		// 全体の時間
	int m_nSecond;		// 現在時間 ( 秒 )
	int m_nMinute;		// 現在時間 ( 分 )
	int m_nDecTime;		// 減った時間量
	int m_nCount;		// 時間カウンター
	float m_fHeight;	// 横幅
	float m_fWidth;		// 高さ

	CNumber* m_pNumberMinute[Config::DIGIT_TIME];	// ナンバークラスの分表示ポインタ
	CNumber* m_pNumberSecond[Config::DIGIT_TIME];	// ナンバークラスの秒表示ポインタ
	D3DXVECTOR3 m_pos;		// 座標
};