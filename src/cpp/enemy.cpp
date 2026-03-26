//=========================================================
//
// 敵の処理 [ enemy.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル	
//*********************************************************
#include "enemy.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "collisionsphere.h"
#include "spherecollider.h"
#include "template.h"
#include "enemymanager.h"
#include "gamesceneobject.h"
#include "score.h"
#include <string>
#include <xfilemanager.h>
#include <manager.h>
#include "sound.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemy::CEnemy(int nPriority) : CObjectX(nPriority),
m_pCollider(nullptr),
m_nLife(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemy::~CEnemy()
{

}
//=========================================================
// 生成処理
//=========================================================
CEnemy* CEnemy::Create
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale,
	const char* pModelName
)
{
	// インスタンス生成
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) return nullptr;

	// オブジェクト設定
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetScale(scale);
	pEnemy->SetFilePass(pModelName);
	pEnemy->SetIsOutLine(true);

	// 初期化失敗時
	if (FAILED(pEnemy->Init())) return nullptr;

	return pEnemy;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemy::Init(void)
{
	// 親クラスの初期化処理
	CObjectX::Init();

	// アウトラインのカラーを設定(黄色)
	SetOutLineColor(D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f));
	m_nLife = Config::LIFE;

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号を取得
	int nModelIdx = GetModelIdx();

	// モデルのパス取得
	std::string str = pXManager->GetInfo(nModelIdx).FilePath;

	// 種類の設定
	if (str == "data/MODEL/ENEMY/Ant_Face.x") // 顔
	{
		SetType(TYPE::ANT);
	}
	else if (str == "data/MODEL/ENEMY/Otama.x") // じゃくし
	{
		SetType(TYPE::OTAMA);
	}
	else if (str == "data/MODEL/ENEMY/butterfly.x") // 蝶
	{
		SetType(TYPE::BUTTERFLY);
	}
	else if (str == "data/MODEL/ENEMY/Spider.x") // 蜘蛛
	{
		SetType(TYPE::SPIDER);
	}

	// コライダー生成
	m_pCollider = CSphereCollider::Create(GetPos(), 40.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemy::Uninit(void)
{
	// 親クラスの終了処理
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemy::Update(void)
{
	// コライダー更新座標を取得
	auto pos = GetPos();

	// 回転を適用する
	auto rot = GetRot();
	rot.y += Config::ROT;
	SetRot(rot);

	if (m_pCollider)
	{
		m_pCollider->SetPos(pos);
	}

	// 親クラスの更新処理
	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CEnemy::Draw(void)
{
	// 親クラスの描画処理
	CObjectX::Draw();
}
//=========================================================
// ダメージ処理
//=========================================================
void CEnemy::DecLife(const int& nDamage)
{
	// 体力減らす
	m_nLife -= nDamage;
	if (m_nLife <= NULL)
	{
		// 体力を0にする
		m_nLife = NULL;

		// サウンド取得と再生
		auto Sound = CManager::GetInstance()->GetSound();
		if (Sound) Sound->Play(CSound::SOUND_LABEL_EAT);

		// 配列内の要素を削除
		CEnemyManager::GetInstance()->Erase(this);

		switch (m_nType)
		{
		case CEnemy::ANT:
			// ゲーム内スコアを加算する
			CGameSceneObject::GetInstance()->GetScore()->AddScore(1);
			break;

		case CEnemy::OTAMA:
			// ゲーム内スコアを加算する
			CGameSceneObject::GetInstance()->GetScore()->AddScore(3);
			break;

		case CEnemy::BUTTERFLY:
			// ゲーム内スコアを加算する
			CGameSceneObject::GetInstance()->GetScore()->AddScore(5);
			break;

		case CEnemy::SPIDER:
			// ゲーム内スコアを加算する
			CGameSceneObject::GetInstance()->GetScore()->AddScore(10);
			break;

		default:
			// ゲーム内スコアを加算する
			CGameSceneObject::GetInstance()->GetScore()->AddScore(0);
			break;
		}

		// 敵の食べた数のスコアを加算する
		CGameSceneObject::GetInstance()->GetEatNum()->AddScore(1);

		// 自身の破棄
		Uninit();

		// 処理終了
		return;
	}
}
//=========================================================
// 当たり判定処理
//=========================================================
bool CEnemy::Collision(CSphereCollider* pOther)
{
	// nullなら
	if (m_pCollider == nullptr) return false;

	return CCollisionSphere::Collision(m_pCollider.get(),pOther);
}