//=========================================================
//
// ゲームのオブジェクト管理処理 [ gamesceneobject.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "gamesceneobject.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "blockmanager.h"
#include "gametime.h"
#include "score.h"
#include "gamemanager.h"
#include "jsonmanager.h"
#include "player.h"
#include "template.h"
#include "enemymanager.h"
#include "walltargetmanager.h"
#include "goal.h"
#include "simplemeshcylinder.h"
#include "particle.h"
#include "enemy.h"

//*********************************************************
// 静的メンバ変数
//*********************************************************
CGameSceneObject* CGameSceneObject::m_pInstance = nullptr;			// シングルトン変数

//*********************************************************
// 定数名前空間
//*********************************************************
namespace GAMEOBJECT
{
	const D3DXVECTOR3 TimerPos		= { 1020.0f,60.0f,0.0f };		// タイマーの座標
	const D3DXVECTOR3 TopAntPos		= { 0.0f, 0.0f, -450.0f };		// 操作アリの座標
	const D3DXVECTOR3 PlayerPos		= { -2635.0f,0.0f,-3870.0f };		// プレイヤー座標
	const D3DXVECTOR3 EPos = { -2635.0f,50.0f,-3870.0f };		// プレイヤー座標

	const D3DXVECTOR3 QueenPos		= { 0.0f, 55.0f, 0.0f };		// 女王アリの座標
	constexpr const char* LoadName	= "data/JSON/Gameobject.json";	// 読み込みjsonファイル名
	constexpr const char* WallName	= "data/JSON/GameWall.json";	// 読み込みjsonファイル名
	constexpr const char* ENEMYLOAD = "data/JSON/GameEnemy.json";	// 敵ファイル名
	constexpr const char* POINTLOAD = "data/JSON/GameMaker.json";	// ワイヤーポイントファイル名

	constexpr int INDEX = 4;
	constexpr int PARTICLE = 60;
	const D3DXVECTOR3 target[INDEX] =
	{
		{ 300.0f,100.0f,400.0f },
		{ -200.0f,150.0f,50.0f },
		{ 400.0f,60.0f,-300.0f },
		{ 43.0f, 150.0f, 480.0f }
	};
};

//=========================================================
// コンストラクタ
//=========================================================
CGameSceneObject::CGameSceneObject() : m_pBlocks(nullptr),
m_pTimer(nullptr),
m_pScore(nullptr),
m_pPlayer(nullptr),
m_pGoal(nullptr),
m_pScoreEatNum(nullptr),
m_nIdx(NULL),
m_nParticleCreateCount(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CGameSceneObject::~CGameSceneObject()
{

}
//=========================================================
// インスタンス取得処理
//=========================================================
CGameSceneObject* CGameSceneObject::GetInstance(void)
{
	// nullなら
	if (m_pInstance == nullptr) m_pInstance = new CGameSceneObject;

	// インスタンスを返す
	return m_pInstance;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CGameSceneObject::Init(void)
{
	// ゲームで使うオブジェクトの読み込み
	auto jsonmanager = CManager::GetInstance()->GetJsonManager();
	jsonmanager->Load(GAMEOBJECT::LoadName);

	// タイマー生成
	m_pTimer = CGameTime::Create(GAMEOBJECT::TimerPos);
	m_pGoal = CGoal::Create(D3DXVECTOR3(255.0f, 1570.0f, -600.0f));
	// 各種ポインタクラスの生成
	CreatePointer();

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(GAMEOBJECT::PlayerPos, VECTOR3_NULL);

	// ゴール生成
	m_pGoal = CGoal::Create(D3DXVECTOR3(255.0f, 1570.0f,-600.0f));
	CSimpleMeshCylinder::Create(D3DXVECTOR3(255.0f, 1480.0f, -600.0f), 80.0f);

	// スコア初期化
	m_pScore->DeleteScore();
	m_pScoreEatNum->DeleteScore();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
	// 敵管理クラスの終了
	CEnemyManager::GetInstance()->Uninit();

	// ターゲットポイント管理クラスの終了
	CWallTargetManager::GetInstance()->Uninit();

	// ブロック管理クラスの破棄
	m_pBlocks.reset();

	// インスタンスの破棄
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//=========================================================
// 更新処理
//=========================================================
void CGameSceneObject::Update(void)
{
#if 1
	// カウント加算
	m_nParticleCreateCount++;

	if (m_nParticleCreateCount >= GAMEOBJECT::PARTICLE)
	{
		// 生成
		auto pos = D3DXVECTOR3(255.0f, 1580.0f, -600.0f);

		// パーティクル生成
		CParticle::Create(pos, COLOR_YERROW, 70, 120.0f, 120, 160);

		// カウント初期化
		m_nParticleCreateCount = 0;
	}

#ifdef _DEBUG
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F7))
	{
		// 仮ですこあ書き出しを実行する
		//m_pScore->SaveScore();
	}
#endif // _DEBUG
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CGameSceneObject::Draw(void)
{
#ifdef _DEBUG
	// デバッグ表示
#endif // _DEBUG
}
//=========================================================
// ポインタの生成を行う関数
//=========================================================
void CGameSceneObject::CreatePointer(void)
{
	// ブロックマネージャー生成
	m_pBlocks = std::make_unique<CBlockManager>();
	auto jsonManager = CManager::GetInstance()->GetJsonManager();
	jsonManager->SetBlockManager(m_pBlocks.get());
	m_pBlocks->Init();

	// タイマー生成
	m_pTimer = CGameTime::Create(GAMEOBJECT::TimerPos);

	// スコア生成
	m_pScore = CScore::Create(VECTOR3_NULL);
	m_pScoreEatNum = CScore::Create(VECTOR3_NULL);

	// ターゲットポイントの初期化
	 CWallTargetManager::GetInstance()->Init(GAMEOBJECT::POINTLOAD);

	// 敵管理クラスの初期化
	CEnemyManager::GetInstance()->Init(GAMEOBJECT::ENEMYLOAD);
}