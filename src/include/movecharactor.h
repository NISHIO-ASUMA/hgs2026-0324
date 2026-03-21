//=========================================================
//
// 動くキャラクタークラス処理 [ movecharactor.h ]
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
#include <vector>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"
#include "motion.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CShadowStencil;
class CModel;
class CBlackBoard;
class CNode;

//*********************************************************
// 動くキャラクタークラスを定義
//*********************************************************
class CMoveCharactor : public CObject
{
public:

	CMoveCharactor(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CMoveCharactor();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	void MotionLoad(const char* pScriptName, int nDestMotions,const bool isShadow);
	void UpdatePosition(void);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetOldPos(const D3DXVECTOR3& posOld) { m_posOld = posOld; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetRotDest(const D3DXVECTOR3& rotdest) { m_rotDest = rotdest; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_scale = scale; }
	inline void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	inline void SetMoveValue(const float fValue) { m_fMoveValue = fValue; }
	inline void SetUseStencil(const bool isUse = false) { m_isStencilUse = isUse; }
	inline void SetUseOutLine(const bool isUse = false) { m_isOutLine = isUse; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline D3DXVECTOR3 GetOldPos(void) const { return m_posOld; }
	inline D3DXVECTOR3 GetRot(void) const { return m_rot; }
	inline D3DXVECTOR3 GetMove(void) const { return m_move; }
	inline D3DXVECTOR3 GetRotDest(void) const { return m_rotDest; }

	/// <summary>
	/// モーションのポインタを返す
	/// </summary>
	/// <param name=""></param>
	/// <returns>
	inline CMotion* GetMotion(void) { return m_pMotion.get(); }

protected:

	CBlackBoard* m_pBlackBoard = nullptr; // 共通ブラックボード
	CNode* m_pBehaviorTree = nullptr;	  // ツリーノード

private:

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_posOld;	// 過去座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_rotDest;  // 目的角
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_scale;	// 拡大率
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス

	std::unique_ptr<CMotion>m_pMotion;	// モーションクラスのユニークポインタ
	std::vector<CModel*>m_pModel;		// モデルクラスの動的配列
	CShadowStencil* m_pShadowS;			// ステンシルシャドウクラスのポインタ

	float m_fMoveValue;		// 移動加算値
	bool m_isStencilUse;	// ステンシルシャドウを使うか
	bool m_isOutLine;		// アウトラインを使うかどうか
	bool m_isInstancing;	// 適用させるかどうか
};