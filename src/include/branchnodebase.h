//=========================================================
//
// 枝分かれノードクラス基底処理 [ branchnodebase.h ]
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
#include "nodebase.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// 枝分かれノードクラスを定義
//*********************************************************
class CBranchNodeBase : public CNodeBase
{

public:

	//**************************
	// ノードの判別番号定義
	//**************************
	enum class BRANCH_NUM
	{
		BRANCH_NUM_TRUE,
		BRANCH_NUM_FALSE,
		BRANCH_NUM_MAX
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="blackboard">ブラックボードのポインタ</param>
	/// <param name="TrueNode">true結果を返すノード</param>
	/// <param name="FalseNode">false結果を返すノード</param>
	CBranchNodeBase(CBlackBoard* blackboard, CNode* TrueNode, CNode* FalseNode) : CNodeBase(blackboard)
	{
		// 値の初期化をする
		m_nTrueIdx = -1;
		m_pBranchNode[0] = TrueNode;
		m_pBranchNode[1] = FalseNode;
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBranchNodeBase()
	{
		// 配列をクリア
		for (int nCnt = 0; nCnt < static_cast<int>(BRANCH_NUM::BRANCH_NUM_MAX); nCnt++)
		{
			if (m_pBranchNode[nCnt] != nullptr)
			{
				delete m_pBranchNode[nCnt];
				m_pBranchNode[nCnt] = nullptr;
			}
		}
	};

	virtual void Init() override;
	virtual void Update() override;
	virtual void Exit() override;

protected:

	virtual const bool IsCondition() = 0;	// 継承先のクラスに持つ

	int m_nTrueIdx = -1;					// 条件を満たすノードのインデックス

	// true,falseの2つのノードのポインタ
	CNode* m_pBranchNode[static_cast<int>(BRANCH_NUM::BRANCH_NUM_MAX)] = { nullptr,nullptr }; 
};