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
#include "block.h"
#include "template.h"

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
	const D3DXVECTOR3 QueenPos		= { 0.0f, 55.0f, 0.0f };		// 女王アリの座標
	constexpr const char* LoadName	= "data/JSON/Gameobject.json";	// 読み込みjsonファイル名
	constexpr const char* WallName	= "data/JSON/GameWall.json";	// 読み込みjsonファイル名

	constexpr int INDEX = 4;

	const D3DXVECTOR3 target[INDEX] =
	{
		{ 300.0f,100.0f,400.0f },
		{ -200.0f,150.0f,50.0f },
		{ 400.0f,60.0f,-300.0f },
		{ 600.0f,0.0,-0.0f }
	};
};

//=========================================================
// コンストラクタ
//=========================================================
CGameSceneObject::CGameSceneObject() : m_pBlocks(nullptr),
m_pTimer(nullptr),
m_pScore(nullptr),
m_pBlock(nullptr),
m_pPlayer(nullptr),
m_nIdx(NULL)
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

	// 各種ポインタクラスの生成
	CreatePointer();

	// プレイヤー生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(-400.0f, 0.0f, -200.0f), VECTOR3_NULL);

	// テストブロック
	m_pBlock = CBlock::Create(VECTOR3_NULL, VECTOR3_NULL, INITSCALE, "STAGEOBJ/block001.x");
	CBlock::Create(D3DXVECTOR3(0.0f,300.0f,0.0f), VECTOR3_NULL, INITSCALE, "STAGEOBJ/block000.x");

	// スコア初期化
	m_pScore->DeleteScore();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CGameSceneObject::Uninit(void)
{
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
#ifdef _DEBUG
	// デバッグキー
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_B) ||
		CManager::GetInstance()->GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_A))
	{
		m_nIdx = Wrap(m_nIdx, 0, GAMEOBJECT::INDEX -1);
		m_pPlayer->ActionSetting(GAMEOBJECT::target[m_nIdx]);
		m_nIdx++;
	}
#endif // _DEBUG
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

}