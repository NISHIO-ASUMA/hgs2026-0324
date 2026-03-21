//=========================================================
//
// モーション処理 [ motion.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "motion.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "template.h"
#include "debugproc.h"
#include "motionmanager.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CMotion::CMotion() : m_nCounterMotion(NULL),
m_nKey(NULL),
m_motiontype(NULL),
m_nNextKey(NULL),
m_nAllFrameCount(NULL),
m_nNumAllFrame(NULL),
m_motiontypeBlend(NULL),
m_nFrameBlend(NULL),
m_nKeyBlend(NULL),
m_nNextKeyBlend(NULL),
m_nMotionIdx(-1),
m_isFinishMotion(false),
m_isFirstMotion(false),
m_isBlendMotion(false)
{
}
//=========================================================
// デストラクタ
//=========================================================
CMotion::~CMotion()
{
	
}
//=========================================================
// モーション読み込み関数
//=========================================================
std::unique_ptr<CMotion> CMotion::Load
(
	const char* pFilename, 
	std::vector<CModel*>& pModel, 
	int nDestMotions,
	const bool isShadow
)
{
	// モーションのユニークポインタ生成
	auto pMotion = std::make_unique<CMotion>();

	// 登録処理
	pMotion->RegisterPath(pFilename, pModel, nDestMotions, isShadow);

	// 生成されたポインタを返す
	return pMotion;
}
//=========================================================
// モーションインデックス登録関数
//=========================================================
void CMotion::RegisterPath(const char* pMotionName, std::vector<CModel*>& pModel, int nDestMotions, const bool isShadow)
{
	// モーションマネージャーを取得する
	auto MotionManager = CManager::GetInstance()->GetMotionManager();
	if (MotionManager == nullptr) return;

	// インデックスに登録し,その情報を取得する
	m_nMotionIdx = MotionManager->Register(pMotionName, pModel, nDestMotions, isShadow);
}
//============================================================
// モーションセット
//============================================================
void CMotion::SetMotion(int motiontype)
{
	// 一致していたら
	if (m_motiontype == motiontype) return;

	// 代入
	m_motiontype = motiontype;
	m_nKey = 0;
	m_nCounterMotion = 0;
	m_nAllFrameCount = 0;
	m_isFinishMotion = false;
}
//=================================================================
// ブレンドモーションセット情報
//=================================================================
void CMotion::SetMotion(int nMotionType, bool isBlend, int nBlendFrame)
{
	// 一致していたら
	if (m_motiontype == nMotionType) return;

	// ブレンドが有効化
	if (isBlend == true)
	{
		// 最初のモーションブレンドが終わってたら
		if (!m_isFirstMotion)
		{
			m_isFirstMotion = true;
			m_nKeyBlend = 0;				// 0から始める
			m_nCounterBlend = 0;			// 0から始める
		}

		m_isBlendMotion = isBlend;			// ブレンドがあるかどうか
		m_nFrameBlend = nBlendFrame;		// ブレンドのフレームを代入
		m_motiontypeBlend = nMotionType;	// ブレンドするモーションのタイプを代入
	}
	// モーションブレンドがない
	else
	{
		// 一致していたら
		if (m_motiontype == nMotionType) return;

		// 代入
		m_motiontype = nMotionType;
		m_nKey = 0;
		m_nCounterMotion = 0;
		m_nAllFrameCount = 0;
		m_isFinishMotion = false;
	}
}
//=========================================================
// モーション全体更新処理
//=========================================================
void CMotion::Update(std::vector<CModel*> pModel)
{
	// モーションマネージャーから情報を取得
	auto Manager = CManager::GetInstance()->GetMotionManager();
	const auto& Info = Manager->GetFileDataIdx(m_nMotionIdx);

	// 最大数情報
	int nMotionNum = Info.nNumMotion;
	int nModelNum = Info.nNumModel;

	// 例外処理
	if (nModelNum <= 0)
	{
		// 警告表示
		MessageBox(GetActiveWindow(), "モデルが存在しません", "キャラクターエラー", MB_OK);
		return;
	}

	// 現在モーションキー計算
	m_motiontype = Clump(m_motiontype, 0, nMotionNum);
	m_nNextKey = Wrap(m_nKey + 1, 0, Info.m_aMotionInfo[m_motiontype].nNumKey - 1);

	// 最後のキーとブレンドのキーを計算
	int nLastKey = Info.m_aMotionInfo[m_motiontype].nNumKey - 1;

	// 最大モデル数で回す
	for (int nCnt = 0; nCnt < nModelNum; nCnt++)
	{
		// ブレンド開始なら
		if (m_isFirstMotion)
		{
			// ブレンドモーションの更新
			UpdateBlend(Manager,pModel.data(), nCnt);
		}
		else
		{
			// 現在のモーション更新
			UpdateCurrentMotion(Manager,pModel.data(), nCnt);
		}

		// モデル更新
		pModel[nCnt]->Update();
	}

	// フレーム進行処理
	if (m_nCounterMotion >= Info.m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame)
	{
		// カウンターをリセット
		m_nCounterMotion = 0;

		// キー数が上限より一個下
		if (m_nKey < Info.m_aMotionInfo[m_motiontype].nNumKey - 1)
		{
			// キー数加算
			m_nKey++;
		}
		else if (m_nKey >= Info.m_aMotionInfo[m_motiontype].nNumKey)
		{
			// キーをリセット
			m_nKey = 0;

			// フレームをリセット
			m_nCounterMotion = 0;
		}
		else
		{
			// 通常ループ
			m_nKey = Wrap(m_nKey + 1, 0, Info.m_aMotionInfo[m_motiontype].nNumKey - 1);
		}
	}
	else
	{
		// ブレンドが開始していなかったら
		if (!m_isFirstMotion)
		{
			// カウンターを加算
			m_nCounterMotion++;
		}

		// 全体フレームカウント
		m_nAllFrameCount++;
	}

	// ブレンドモーションが始まったら
	if (m_isFirstMotion == true)
	{
		// ブレンドモーションカウントをインクリメント
		m_nCounterBlend++;
	}

	// モーションの出だしのブレンドが終了した
	if (m_isFirstMotion == true && m_nCounterBlend >= m_nFrameBlend)
	{
		m_nFrameBlend = 0;

		// フラグをもとに戻す
		m_isFirstMotion = false;

		// モーションをブレンドしたモーションにする
		m_motiontype = m_motiontypeBlend;

		// キーをリセット
		m_nKey = 0;

		// ブレンドしたフレームから開始
		m_nCounterMotion = 0;

		// ブレンドフレーム初期化
		m_nCounterBlend = 0;
	}

	// 着地モーションの終了判定
	if (m_nKey >= nLastKey -1 && m_nCounterMotion >= Info.m_aMotionInfo[m_motiontype].aKeyInfo[m_nKey].nFrame)
	{// 最後のキーに達していて、カウンターも終了フレームを超えていたら
		m_isFinishMotion = true;
	}

	// 全体フレームカウント計算用
	int nFrame = 0;

	// キーごとのフレームで回す
	for (int nCnt = 0; nCnt < Info.m_aMotionInfo[m_motiontype].nNumKey; nCnt++)
	{
		// 全体計算用に加算
		nFrame += Info.m_aMotionInfo[m_motiontype].aKeyInfo[nCnt].nFrame;
	}

	// 最大値よりもカウントがオーバーしたら
	if (m_nAllFrameCount >= m_nNumAllFrame)
	{
		m_nAllFrameCount = 0;
	}

	// 全体フレーム計算
	m_nNumAllFrame = nFrame;
}
//=================================================================
// 現在のモーションの更新関数
//=================================================================
void CMotion::UpdateCurrentMotion(CMotionManager* pMption,CModel** ppModel, int nModelCount)
{
	// モーションリスト取得
	const auto& motionList = pMption->GetFileDataIdx(m_nMotionIdx);

	// 現在のモーション情報取得
	const CMotionManager::INFO& motionInfo = motionList.m_aMotionInfo[m_motiontype];

	// 現在と次のキー情報取得
	const CMotionManager::KEY_INFO& keyInfoNow = motionInfo.aKeyInfo[m_nKey];
	const CMotionManager::KEY_INFO& keyInfoNext = motionInfo.aKeyInfo[m_nNextKey];

	// 現在の KEY
	const CMotionManager::KEY& NowKey = keyInfoNow.aKey[nModelCount];
	const CMotionManager::KEY& NextKey = keyInfoNext.aKey[nModelCount];

	// キー情報から位置と向きを算出
	D3DXVECTOR3 posMotion, rotMotion;

	// 角度と座標の差分を計算
	posMotion.x = NextKey.fPosX - NowKey.fPosX;
	posMotion.y = NextKey.fPosY - NowKey.fPosY;
	posMotion.z = NextKey.fPosZ - NowKey.fPosZ;

	rotMotion.x = NextKey.fRotX - NowKey.fRotX;
	rotMotion.y = NextKey.fRotY - NowKey.fRotY; 
	rotMotion.z = NextKey.fRotZ - NowKey.fRotZ;

	// 正規化
	rotMotion.x = NormalAngle(rotMotion.x);
	rotMotion.y = NormalAngle(rotMotion.y);
	rotMotion.z = NormalAngle(rotMotion.z);

	// 求める値を保存する変数を宣言
	D3DXVECTOR3 Pos, Rot;

	// 補間係数を計算
	float fDis = static_cast<float>(m_nCounterMotion) / keyInfoNow.nFrame;

	// 補間結果を算出
	Pos.x = NowKey.fPosX + posMotion.x * fDis;
	Pos.y = NowKey.fPosY + posMotion.y * fDis;
	Pos.z = NowKey.fPosZ + posMotion.z * fDis;

	Rot.x = NowKey.fRotX + rotMotion.x * fDis;
	Rot.y = NowKey.fRotY + rotMotion.y * fDis;
	Rot.z = NowKey.fRotZ + rotMotion.z * fDis;

	// モデルのパーツに設定
	ppModel[nModelCount]->SetPos(Pos);
	ppModel[nModelCount]->SetRot(Rot);
}

//=================================================================
// ブレンドモーションの更新関数
//=================================================================
void CMotion::UpdateBlend(CMotionManager* pMption,CModel** ppModel, int nModelCount)
{
	// モーション情報を取得
	const auto& motionList = pMption->GetFileDataIdx(m_nMotionIdx);
	
	// 現在のモーション情報取得
	const CMotionManager::INFO& motionInfo = motionList.m_aMotionInfo[m_motiontype];
	const CMotionManager::INFO& BlendInfo = motionList.m_aMotionInfo[m_motiontypeBlend];

	// ブレンド係数を計算
	float fBlendFrame = static_cast<float>(m_nCounterBlend) / static_cast<float>(m_nFrameBlend);
	float fRateMotion = static_cast<float>(m_nCounterMotion) / static_cast<float>(motionInfo.aKeyInfo[m_nKey].nFrame);

	// 現在のキーと次のキーを取得
	const CMotionManager::KEY& nowKey = motionInfo.aKeyInfo[m_nKey].aKey[nModelCount];
	const CMotionManager::KEY& nextKey = motionInfo.aKeyInfo[m_nNextKey].aKey[nModelCount];

	const CMotionManager::KEY& nowKeyBlend = BlendInfo.aKeyInfo[m_nKeyBlend].aKey[nModelCount];
	const CMotionManager::KEY& nextKeyBlend = BlendInfo.aKeyInfo[m_nNextKeyBlend].aKey[nModelCount];

	//==========================
	// 現在モーションの補間計算
	//==========================
	D3DXVECTOR3 DiffRot = VECTOR3_NULL; // 角度
	D3DXVECTOR3	CurrentRot = VECTOR3_NULL; // 現在角度

	// 角度を計算する
	DiffRot.x = nextKey.fRotX - nowKey.fRotX;
	DiffRot.y = nextKey.fRotY - nowKey.fRotY;
	DiffRot.z = nextKey.fRotZ - nowKey.fRotZ;

	// 現在角度に適用
	CurrentRot.x = nowKey.fRotX + DiffRot.x * fRateMotion;
	CurrentRot.y = nowKey.fRotY + DiffRot.y * fRateMotion;
	CurrentRot.z = nowKey.fRotZ + DiffRot.z * fRateMotion;

	// 座標を計算
	D3DXVECTOR3 DiffPos = VECTOR3_NULL; // 座標
	D3DXVECTOR3	CurrentPos = VECTOR3_NULL; // 現在座標

	// 座標計算
	DiffPos.x = nextKey.fPosX - nowKey.fPosX;
	DiffPos.y = nextKey.fPosY - nowKey.fPosY;
	DiffPos.z = nextKey.fPosZ - nowKey.fPosZ;

	// 現在座標に適用
	CurrentPos.x = nowKey.fPosX + DiffPos.x * fRateMotion;
	CurrentPos.y = nowKey.fPosY + DiffPos.y * fRateMotion;
	CurrentPos.z = nowKey.fPosZ + DiffPos.z * fRateMotion;

	//==========================================================
	// ブレンドモーションの補間計算
	//==========================================================
	D3DXVECTOR3 DiffBlendRot = VECTOR3_NULL; // 角度
	D3DXVECTOR3	BlendRot = VECTOR3_NULL;	 // ブレンド角度

	// ブレンド角度を計算
	DiffBlendRot.x = nextKeyBlend.fRotX - nowKeyBlend.fRotX;
	DiffBlendRot.y = nextKeyBlend.fRotY - nowKeyBlend.fRotY;
	DiffBlendRot.z = nextKeyBlend.fRotZ - nowKeyBlend.fRotZ;

	// ブレンドの角度を適用
	BlendRot.x = nowKeyBlend.fRotX + DiffBlendRot.x * fBlendFrame;
	BlendRot.y = nowKeyBlend.fRotY + DiffBlendRot.y * fBlendFrame;
	BlendRot.z = nowKeyBlend.fRotZ + DiffBlendRot.z * fBlendFrame;

	D3DXVECTOR3 DiffBlendPos = VECTOR3_NULL; // 座標
	D3DXVECTOR3	BlendPos = VECTOR3_NULL;	 // ブレンド座標

	// ブレンド座標を計算
	DiffBlendPos.x = nextKeyBlend.fPosX - nowKeyBlend.fPosX;
	DiffBlendPos.y = nextKeyBlend.fPosY - nowKeyBlend.fPosY;
	DiffBlendPos.z = nextKeyBlend.fPosZ - nowKeyBlend.fPosZ;

	// ブレンドの座標を適用
	BlendPos.x = nowKeyBlend.fPosX + DiffBlendPos.x * fBlendFrame;
	BlendPos.y = nowKeyBlend.fPosY + DiffBlendPos.y * fBlendFrame;
	BlendPos.z = nowKeyBlend.fPosZ + DiffBlendPos.z * fBlendFrame;

	//==========================================================
	// モデルの座標,角度に適用
	//==========================================================

	// 最終角度,座標
	D3DXVECTOR3 LastRot = VECTOR3_NULL;
	D3DXVECTOR3 LastPos = VECTOR3_NULL;

	// 角度計算
	LastRot.x = CurrentRot.x + (BlendRot.x - CurrentRot.x) * fBlendFrame;
	LastRot.y = CurrentRot.y + (BlendRot.y - CurrentRot.y) * fBlendFrame;
	LastRot.z = CurrentRot.z + (BlendRot.z - CurrentRot.z) * fBlendFrame;

	// 座標計算
	LastPos.x = CurrentPos.x + (BlendPos.x - CurrentPos.x) * fBlendFrame;
	LastPos.y = CurrentPos.y + (BlendPos.y - CurrentPos.y) * fBlendFrame;
	LastPos.z = CurrentPos.z + (BlendPos.z - CurrentPos.z) * fBlendFrame;

	// 正規化
	LastRot.x = NormalAngle(LastRot.x);
	LastRot.y = NormalAngle(LastRot.y);
	LastRot.z = NormalAngle(LastRot.z);

	//==========================================================
	// モデルにセット
	//==========================================================
	ppModel[nModelCount]->SetPos(LastPos);
	ppModel[nModelCount]->SetRot(LastRot);
}

//=================================================================
// デバッグフォント関数
//=================================================================
void CMotion::Debug(void)
{
#ifdef _DEBUG
	CDebugproc::Print("[現在フレームカウント] %d /  [ 最大モーションフレーム ] %d", m_nAllFrameCount, m_nNumAllFrame);
	CDebugproc::Draw(800, 320);

	CDebugproc::Print("[ブレンドフレーム] %d / [ブレンドカウント] %d", m_nFrameBlend, m_nCounterBlend);
	CDebugproc::Draw(800, 340);
#endif // _DEBUG
}
//=================================================================
// モーションフレーム判定
//=================================================================
bool CMotion::CheckFrame(int nStartMotion, int nEndMotion, int nMotionType)
{
	// StartとEndの範囲内なら
	if (m_nAllFrameCount >= nStartMotion && m_nAllFrameCount <= nEndMotion && m_motiontype == nMotionType)
		return true;

	// それ以外の時
	return false;
}