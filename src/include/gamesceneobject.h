//=========================================================
//
// ゲームのオブジェクト管理処理 [ gamesceneobject.h ]
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

//*********************************************************
// 前方宣言
//*********************************************************
class CBlockManager;
class CGameTime;
class CEnemy;
class CScore;
class CBlock;
class CPlayer;
class CGoal;

//*********************************************************
// ゲームシーンで使うオブジェクト管理クラスを定義
//*********************************************************
class CGameSceneObject
{
public:

	~CGameSceneObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CGameTime* GetTime(void) const { return m_pTimer; }
	CScore* GetScore(void) const { return m_pScore; }
	CScore* GetEatNum(void) const { return m_pScoreEatNum; }

	CBlockManager* GetBlockManager(void) const { return m_pBlocks.get(); }
	CPlayer* GetPlayer(void) const { return m_pPlayer; }
	CGoal* GetGoal(void) const { return m_pGoal; }

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static CGameSceneObject* GetInstance(void);

private:
	CGameSceneObject();			// コンストラクタ
	void CreatePointer(void);	// 初期化処理関数分け
private:

	static CGameSceneObject* m_pInstance;		// シングルトン変数
	CScore* m_pScore;							// スコアクラスのポインタ ( ポイントのほう )
	CScore* m_pScoreEatNum;						// スコアクラスのポインタ ( 食べたかずのほう )

	CGameTime* m_pTimer;						// タイマークラスのポインタ
	CPlayer* m_pPlayer;							// プレイヤーポインタ
	CGoal* m_pGoal;								// ゴールクラスポインタ
	std::unique_ptr<CBlockManager>m_pBlocks;	// ブロックマネージャークラスのポインタ

	int m_nIdx = 0; // 検証インデックス
	int m_nParticleCreateCount;					// 生成カウント
};