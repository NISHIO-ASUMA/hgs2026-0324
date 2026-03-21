//=========================================================
//
// パラメータ処理 [ parameter.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// パラメータークラスを定義
//*********************************************************
class CParameter
{
public:

	CParameter();
	~CParameter();

	inline void SetHp(int nHp) { m_nHp = nHp; }
	inline void SetMaxHp(int nMaxHp) { m_nMaxHp = nMaxHp; }
	inline void AddHp(int nValue) { m_nHp += nValue; }

	inline int GetHp()const { return m_nHp; }
	inline int GetMaxHp() const { return m_nMaxHp; }

private:

	int m_nHp;		// 体力
	int m_nMaxHp;	// 最大体力
};