//=========================================================
//
// オブジェクト基底クラス処理 [ object.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// オブジェクトクラスを定義
//*********************************************************
class CObject
{
public:

	//*************************
	// オブジェクトの種類定義
	//*************************
	enum TYPE
	{
		TYPE_NONE,
		TYPE_ARRAY,		// 仲間
		TYPE_ENEMY,		// 敵
		TYPE_QUEEN,		// 防衛対象
		TYPE_WORKER,	// 司令塔
		TYPE_TOP,		// トップキャラクター
		TYPE_MESH,		// メッシュオブジェクト
		TYPE_BLOCK,		// ブロック
		TYPE_PAUSE,		// ポーズ
		TYPE_INSTANCE,	// インスタンシング
		TYPE_MAX
	};

	//*************************
	// 描画番号定義
	//*************************
	enum PRIORITY
	{
		NONE,			// 初期状態
		MESH,			// メッシュ
		BASENUMBER,		// 基準番号
		MODELOBJECT,	// モデル
		MOVE3D,			// 3Dオブジェクト
		SHADOW,			// 影
		IMPACT,			// 衝撃波
		CHARACTOR,		// プレイヤー
		BILLBOARD,		// ビルボード
		EFFECT,			// エフェクト
		UI,				// 2DUI
		PAUSE,			// ポーズ
		PRIORITY_MAX	// 最大数
	};

	CObject(int nPriority = NONE);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject* GetTop(int nPriority) { return m_pTop[nPriority]; }
	static int GetNumAll(void) { return m_nNumAll; }

	void SetObjType(TYPE type) { m_Type = type; }

	TYPE GetObjType(void) { return m_Type; }
	CObject* GetNext(void) { return m_pNext; }

protected:

	void Release(void); // 解放関数

private:

	static int m_nNumAll;					// オブジェクトの総数
	static CObject* m_pTop[PRIORITY_MAX];	// 先頭オブジェクト
	static CObject* m_pCur[PRIORITY_MAX];	// 最後尾オブジェクト

	bool m_isDeath;			// 死亡フラグ

	TYPE m_Type;			// オブジェクトの種類変数
	CObject* m_pNext;		// 次のオブジェクトのポインタ
	CObject* m_pPrev;		// 前のオブジェクトのポインタ
};