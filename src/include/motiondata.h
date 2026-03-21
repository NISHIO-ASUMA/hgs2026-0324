//=========================================================
//
// モーションデータ保持管理クラス [ motiondata.h ]
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
#include <vector>
#include <memory>

//*********************************************************
// データを管理する構造体をまとめたクラス定義
//*********************************************************
class CMotionData
{
public:
	//***************************
	// キー構造体宣言
	//***************************
	struct KEY
	{
		float fPosX;	// 位置X
		float fPosY;	// 位置Y
		float fPosZ;	// 位置Z
		float fRotX;	// 角度X
		float fRotY;	// 角度Y
		float fRotZ;	// 角度Z
	};

	//***************************
	// キー情報の構造体宣言
	//***************************
	struct KEY_INFO
	{
		int nFrame = 0;				// フレーム数
		std::vector<KEY> aKey;		// 動的なキー数
	};

	//***************************
	// モーション情報の構造体宣言
	//***************************
	struct INFO
	{
		bool bLoop = false;				// ループするかしないか
		int nNumKey = 0;				// キーの総数
		std::vector<KEY_INFO> aKeyInfo; // モーションの動的キーフレーム
	};

	void SetNumModels(const int nModel) { m_nNumModels = nModel; }
	void SetNumMotions(const int nMotion) { m_nNumMotions = nMotion; }
	int GetNumModels(void) const { return m_nNumModels; }
	int GetNumMotions(void) const { return m_nNumMotions; }

	/// <summary>
	/// 動的配列情報の取得
	/// </summary>
	/// <returns></returns>
	const std::vector<INFO>& GetMotions() const { return m_motions;}

	/// <summary>
	/// モーションのインデックス番号から取得
	/// </summary>
	/// <param name="index">取得したい番号</param>
	/// <returns></returns>
	const INFO& GetMotion(int index) const { return m_motions[index]; }

	/// <summary>
	/// 配列のサイズを設定する
	/// </summary>
	/// <param name="nSize"></param>
	void ResizeMotions(const int nSize) { m_motions.resize(nSize); }

	/// <summary>
	/// 構造体情報の参照取得
	/// </summary>
	/// <param name="index">インデックス番号</param>
	/// <returns></returns>
	INFO& GetMotionRef(int index) { return m_motions[index]; }

private:

	int m_nNumModels = 0;		 // モデル数
	int m_nNumMotions = 0;		 // モーション数
	std::vector<INFO> m_motions; // モーションデータ
};