//=========================================================
//
// モーションファイル管理クラス [ motionmanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "motionmanager.h"

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "motion.h"
#include "model.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
int CMotionManager::m_nNumAll = NULL; // 総数加算

//=========================================================
// コンストラクタ
//=========================================================
CMotionManager::CMotionManager() :m_FileData{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMotionManager::~CMotionManager()
{
	UnLoad();
}
//=========================================================
// 全読み込み処理
//=========================================================
HRESULT CMotionManager::Load(void)
{
	// 値の初期化
	m_FileData.clear();

	return S_OK;
}
//=========================================================
// 全破棄処理
//=========================================================
void CMotionManager::UnLoad(void)
{
	// 値のクリア
	m_FileData.clear();
}
//=========================================================
// モーションファイルの登録処理
//=========================================================
int CMotionManager::Register(const char* pFileName, std::vector<CModel*>& pModel, int nDestMotion, bool isShadow)
{
	// もしすでにファイルが登録されていたら
	for (int nCnt = 0; nCnt < static_cast<int>(m_FileData.size()); nCnt++)
	{
		// 指定番号のインデックスを返す
		if (m_FileData[nCnt].FilePath == pFileName)
		{
			// モデル配列をセットする
			pModel.resize(m_FileData[nCnt].Modelpath.size());

			// モデルのパス名を見る 一緒だったらそのモデルパス名で生成する
			for (int nModel = 0;nModel < static_cast<int>(m_FileData[nCnt].Modelpath.size());nModel++)
			{
				// 保存してあるパスを使用して新たにモデル生成
				const std::string& path = m_FileData[nCnt].Modelpath[nModel];

				// モデルのポインタを生成
				CModel* pNewModel = CModel::Create(VECTOR3_NULL, VECTOR3_NULL, path.c_str(), isShadow);

				// 配列に追加
				pModel[nModel] = pNewModel;

				// 親子関係の設定
				int parentIndex = m_FileData[nCnt].nParentId[nModel];

				// 親モデルが存在する
				if (parentIndex != -1)
				{
					pNewModel->SetParent(pModel[parentIndex]);
				}
				else
				{
					pNewModel->SetParent(nullptr);
				}

				// オフセット設定
				pNewModel->OffSetPos(m_FileData[nCnt].offpos[nModel]);
				pNewModel->OffSetRot(m_FileData[nCnt].offrot[nModel]);
			}

			// ファイルインデックス番号を返す
			return nCnt;
		}
	}

	// 無かったら新規登録と読み込みをする
	MOTIONFILE motionfile = {};
	motionfile.FilePath = pFileName;

	// 配列に情報を追加
	m_FileData.push_back(motionfile);

	// モーションスクリプトファイルの読み込み
	LoadMotion(pFileName, pModel, nDestMotion, isShadow);

	// 加算してインデックスを返す
	return m_nNumAll++;
}
//=========================================================
// 実際のモーション読み込み処理
//=========================================================
void CMotionManager::LoadMotion(const char* pFileName, std::vector<CModel*>& pModel, int nDestMotion, bool isShadow)
{
	// 読み込むファイルを設定する
	std::ifstream file(pFileName);

	// ファイルが開けなかったら
	if (!file)
	{
		MessageBox(NULL, "ファイルオープン失敗", pFileName, MB_OK);
		return;
	}

	// 一行読み込む
	std::string line;

	// 使用ローカル変数
	int nModel = 0;
	int nIdx = 0;
	int nCntMotion = 0;

	// モーション情報のサイズを設定
	m_FileData.back().m_aMotionInfo.resize(nDestMotion);

	// 最大モーション数を保持する
	m_FileData.back().nNumMotion = nDestMotion;

	// 文字列を読み続ける
	while (std::getline(file, line))
	{
		// トークン設定
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		// "NUM_MODEL"読み取り時
		if (token == "NUM_MODEL")
		{
			// モデル数設定
			nModel = SetModels(iss);

			// モデルの配列のサイズをセット
			pModel.resize(nModel);
		}
		// "MODEL_FILENAME"読み取り時
		else if (token == "MODEL_FILENAME")
		{
			// モデルファイル読み込み
			SetModelFile(iss, pModel, nIdx, isShadow);

			// インデックスカウントを加算
			nIdx++;
		}
		// "PARTSSET"読み取り時
		else if (token == "PARTSSET")
		{
			// パーツごとのセット
			SetParts(file, pModel);
		}
		// "MOTIONSET"読み取り時
		else if (token == "MOTIONSET")
		{
			// パーツのモーションの設定
			SetPartsMotion(file,nCntMotion);

			// モーションカウントを加算
			nCntMotion++;
		}
		// "END_SCRIPT"読み取り時
		else if (token == "END_SCRIPT")
		{
			break;
		}
	}

	// ファイルを閉じる
	file.close();
}

//=========================================================
// モデル数をチェックする
//=========================================================
int CMotionManager::SetModels(std::istringstream& iss)
{
	// 文字列設定
	std::string eq;
	int nModel = 0;

	// 読み込んだモデル数を設定
	iss >> eq >> nModel;

	// モデル数格納
	m_FileData.back().nNumModel = nModel;

	// モデル数を返す
	return nModel;
}
//=========================================================
// モデルファイル名をセットする
//=========================================================
void CMotionManager::SetModelFile(std::istringstream& iss, std::vector<CModel*>& pModel, int nCnt, const bool isShadow)
{
	// 文字列設定
	std::string eq, filename;

	// モデルのファイル名読み取り
	iss >> eq >> filename;

	// モデルの生成処理
	CModel* pNewModel = CModel::Create(VECTOR3_NULL, VECTOR3_NULL, filename.c_str(), isShadow);

	// モデルのポインタに格納
	pModel[nCnt] = pNewModel;

	// 配列に登録する
	m_FileData.back().apModel.push_back(pNewModel); // モデルのポインタ
	m_FileData.back().Modelpath.push_back(filename); // モデルのパス情報
}
//=========================================================
// パーツごとの情報をセットする
//=========================================================
void CMotionManager::SetParts(std::ifstream& file, std::vector<CModel*>& pModel)
{
	// 読み込み文字列
	std::string line;

	// モデル番号と親モデルの設定用変数
	int nIdx = -1, nParentIdx = -1;

	// 代入用座標と角度
	D3DXVECTOR3 pos = VECTOR3_NULL;
	D3DXVECTOR3 rot = VECTOR3_NULL;

	// 読み込みループ
	while (std::getline(file, line))
	{
		// 読み込むラインをセット
		std::istringstream partss(line);
		std::string cmd;
		partss >> cmd;

		// "INDEX"を読み取った
		if (cmd == "INDEX")
		{
			// インデックスの変数に値を代入
			std::string eq;
			partss >> eq >> nIdx;
		}
		// "PARENT"を読み取った
		else if (cmd == "PARENT")
		{
			// 親モデル設定用の変数に値を代入
			std::string eq;
			partss >> eq >> nParentIdx;

			// インデックスの範囲内なら
			if (nIdx >= NULL && pModel[nIdx])
			{
				if (nParentIdx != -1)
				{// 親モデルが存在する
					pModel[nIdx]->SetParent(pModel[nParentIdx]);

				}
				else
				{// 親モデルが存在しない
					pModel[nIdx]->SetParent(nullptr);
				}

				// 親パーツの設定
				m_FileData.back().nParentId.push_back(nParentIdx);
			}
		}
		// "POS"を読み取った
		else if (cmd == "POS")
		{
			// 読み取った座標を代入
			std::string eq;
			partss >> eq >> pos.x >> pos.y >> pos.z;

			// 範囲内かチェック
			if (nIdx >= 0 && pModel[nIdx])
			{
				// モデルのオフセット座標に値を設定
				pModel[nIdx]->OffSetPos(pos);

				// 登録情報を配列に追加
				m_FileData.back().offpos.push_back(pos);
			}
		}
		// "ROT"を読み取った
		else if (cmd == "ROT")
		{
			std::string eq;
			std::string rest;
			partss >> eq;

			std::getline(partss, rest); 

			// コメントを削除
			size_t commentpos = rest.find('#');
			if (commentpos != std::string::npos)
			{
				rest = rest.substr(0, commentpos);
			}

			// 空白で分割してfloatに変換
			std::istringstream iss(rest);

			// 角度情報をセットする
			iss >> rot.x >> rot.y >> rot.z;

			if (nIdx >= NULL && pModel[nIdx])
			{
				// オフセットの値にセット
				pModel[nIdx]->OffSetRot(rot);

				// 登録情報を配列に追加
				m_FileData.back().offrot.push_back(rot);
			}
		}
		// "END_PARTSSET"を読み取った
		else if (cmd == "END_PARTSSET")
		{
			// while文を抜ける
			break;
		}
	}
}
//=========================================================
// パーツごとのモーション情報をセットする
//=========================================================
void CMotionManager::SetPartsMotion(std::ifstream& file, int nCntMotion)
{
	// 読み込み文字列
	std::string line;

	// キーセット数のカウント用変数
	int nCntKey = NULL;

	// モーション設定の読み取りループ
	while (std::getline(file, line))
	{
		// 1行ずつ読み込む
		std::istringstream motionss(line);
		std::string token;
		motionss >> token;

		// "LOOP"を読み取った
		if (token == "LOOP")
		{
			std::string eq;
			int loopFlag;
			motionss >> eq >> loopFlag;

			// "="を読み取った
			if (eq == "=")
			{
				// モーションループフラグを設定
				m_FileData.back().m_aMotionInfo[nCntMotion].bLoop = loopFlag;
			}
		}
		// "NUM_KEY"を読み取った
		else if (token == "NUM_KEY")
		{
			// "="代入
			std::string eq;

			// キー数保存
			int numKeys;

			// 値代入
			motionss >> eq >> numKeys;

			// nNumKeyを代入
			m_FileData.back().m_aMotionInfo[nCntMotion].nNumKey = numKeys;

			// キー全体のサイズを設定
			m_FileData.back().m_aMotionInfo[nCntMotion].aKeyInfo.resize(numKeys);

			//	キー数の上限に達するまで
			while (nCntKey < numKeys)
			{
				// キーごとのパーツのサイズセット
				m_FileData.back().m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey.resize(m_FileData.back().nNumModel);

				// キー情報の設定
				SetKey(file,nCntMotion, nCntKey);

				// インクリメントして進める
				nCntKey++;
			}
		}
		// "END_MOTIONSET"を読み取った
		else if (token == "END_MOTIONSET")
		{
			nCntKey = 0;
			break;
		}
	}
}
//=========================================================
// キー全体の情報をセットする
//=========================================================
void CMotionManager::SetKey(std::ifstream& file,int nCntMotion, int nCntKey)
{
	// 行読み込み
	std::string line;

	// 使用変数
	int frame = 0;
	int posKeyIndex = 0, rotKeyIndex = 0;

	// 読んだかフラグ
	bool ReadKey = false;

	// FRAME 行の読み込み
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string cmd, eq;
		ss >> cmd >> eq >> frame;

		// 読み取り一致時
		if (cmd == "FRAME" && eq == "=")
		{
			m_FileData.back().m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = frame;

			break;
		}
	}

	// 読み込みループ
	while (std::getline(file, line))
	{
		if (line.find("END_KEYSET") != std::string::npos)
		{
			// キーセット終了
			break;
		}
		else if (line.find("KEY") != std::string::npos)
		{
			// 有効化
			ReadKey = true;

			// while継続
			continue;
		}
		else if (ReadKey)
		{
			std::istringstream ss(line);
			std::string param;
			ss >> param;

			// "POS または "ROTを読んだ時
			if (param == "POS" || param == "ROT")
			{
				// 各キーごとの設定
				SetKeyDate(ss, param,nCntMotion, nCntKey, posKeyIndex, rotKeyIndex);
			}
			else if (param == "END_KEY")
			{
				// 条件を無効化
				ReadKey = false;
			}
		}
	}
}
//=================================================================
// キーごとの情報設定
//=================================================================
void CMotionManager::SetKeyDate
(
	std::istringstream& ss, 
	const std::string& param,
	int nCntMotion, 
	int nCntKey, 
	int& posKeyIndex, 
	int& rotKeyIndex
)
{
	// 読み込み用1行分のバッファ
	std::string eq;

	// 代入用変数
	D3DXVECTOR3 Vec = VECTOR3_NULL;

	// 座標を代入
	ss >> eq >> Vec.x >> Vec.y >> Vec.z;

	// "="が来たら
	if (eq != "=") return;

	// キー情報変数に代入する
	KEY_INFO& keyInfo = m_FileData.back().m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey];

	// "POS"読み取り時
	if (param == "POS")
	{
		// 各座標に代入する
		keyInfo.aKey[posKeyIndex].fPosX = Vec.x;
		keyInfo.aKey[posKeyIndex].fPosY = Vec.y;
		keyInfo.aKey[posKeyIndex].fPosZ = Vec.z;

		// 座標キー情報カウントを加算
		posKeyIndex++;
	}
	// "ROT"読み取り時
	else if (param == "ROT")
	{
		// 各パーツの角度に代入
		keyInfo.aKey[rotKeyIndex].fRotX = Vec.x;
		keyInfo.aKey[rotKeyIndex].fRotY = Vec.y;
		keyInfo.aKey[rotKeyIndex].fRotZ = Vec.z;

		// 角度キー情報カウントを加算
		rotKeyIndex++;
	}
}