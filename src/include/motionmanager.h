//=========================================================
//
// モーションファイル管理クラス [ motionmanager.h ]
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
#include <string>
#include <vector>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CMotion;
class CModel;

//*********************************************************
// モーション管理クラスを定義
//*********************************************************
class CMotionManager
{
public:

	//*********************************
	// キー構造体宣言
	//*********************************
	struct KEY
	{
		float fPosX;	// 位置X
		float fPosY;	// 位置Y
		float fPosZ;	// 位置Z
		float fRotX;	// 角度X
		float fRotY;	// 角度Y
		float fRotZ;	// 角度Z
	};

	//*********************************
	// キー情報の構造体宣言
	//*********************************
	struct KEY_INFO
	{
		int nFrame;				// フレーム数
		std::vector<KEY> aKey;  // 動的なキー数
	};

	//*********************************
	// モーション情報の構造体宣言
	//*********************************
	struct INFO
	{
		bool bLoop;						// ループするかしないか
		int nNumKey;					// キーの総数
		std::vector<KEY_INFO> aKeyInfo; // モーションの動的キーフレーム
	};

	//*********************************
	// モーション情報を保存する構造体
	//*********************************
	struct MOTIONFILE
	{
		std::string FilePath;				// ファイル名
		std::vector<CModel*>apModel;		// モデルインデックス
		std::vector<std::string>Modelpath;	// モデルパス
		std::vector<int>nParentId;			// idセット
		std::vector<D3DXVECTOR3>offpos;		// オフセット座標
		std::vector<D3DXVECTOR3>offrot;		// オフセット角度
		int nNumModel;						// 最大モデル数
		int nNumMotion;						// 最大モーション数
		std::vector<INFO> m_aMotionInfo;	// モーション情報
	};

	CMotionManager();
	~CMotionManager();

	HRESULT Load(void);
	void UnLoad(void);

	/// <summary>
	/// モーションファイルの登録処理
	/// </summary>
	/// <param name="pFileName">モーションファイル名</param>
	/// <param name="pModel">参照モデル</param>
	/// <param name="nDestMotion">モーション最大数</param>
	/// <param name="isShadow">影の描画フラグ</param>
	/// <returns></returns>
	int Register(const char* pFileName,std::vector<CModel*>& pModel,int nDestMotion,bool isShadow);

	/// <summary>
	/// 実際のモーションロードを行う
	/// </summary>
	/// <param name="pFileName">モーションファイル名</param>
	/// <param name="pModel">参照モデル</param>
	/// <param name="nDestMotion">モーション最大数</param>
	/// <param name="isShadow">影の描画フラグ</param>
	void LoadMotion(const char* pFileName, std::vector<CModel*>& pModel, int nDestMotion, bool isShadow);

	/// <summary>
	/// 動的配列からデータを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<MOTIONFILE>& GetFileData(void) { return m_FileData; }

	/// <summary>
	/// インデックスで取得
	/// </summary>
	/// <param name="nIdx">取得するモーション番号</param>
	/// <returns></returns>
	const MOTIONFILE GetFileDataIdx(const int &nIdx) { return m_FileData[nIdx]; }

private:

	//***********************************
	// クラス内メンバ関数
	//***********************************
	int SetModels(std::istringstream& iss);
	void SetModelFile(std::istringstream& iss, std::vector<CModel*>& pModel, int nCnt, const bool isShadow);
	void SetParts(std::ifstream& file, std::vector<CModel*>& pModel);
	void SetPartsMotion(std::ifstream& file, int nCntMotion);
	void SetKey(std::ifstream& file,int nCntMotion, int nCntKey);
	void SetKeyDate(std::istringstream& ss, const std::string& param, int nCntMotion, int nCntKey, int& posKeyIndex, int& rotKeyIndex);

	// 静的変数
	static int m_nNumAll;

	// ファイルパスを保持しておく配列
	std::vector<MOTIONFILE>m_FileData;
};