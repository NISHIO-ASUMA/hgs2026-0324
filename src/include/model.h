//=========================================================
//
// モデル処理 [ model.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// モデルクラスの定義
//*********************************************************
class CModel
{
public:

	//**********************
	// パーツのタイプ列挙型
	//**********************
	enum PARTTYPE
	{
		PARTTYPE_NONE,		// 何も種類を設定しない時
		PARTTYPE_HEAD,		// 頭
		PARTTYPE_CHEST,		// 体
		PARTTYPE_LEFT_HAND, // 左腕
		PARTTYPE_RIGHT_HAND,// 右腕
		PARTTYPE_LEFT_LEG,	// 左足
		PARTTYPE_RIGHT_LEG,	// 右足
		PARTTYPE_WEAPON,	// 武器
		PARTTYPE_MAX
	};

	CModel();
	~CModel();

	HRESULT Init(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* pFilename,const bool isShadow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void DrawMtxShadow(void);
	void DrawOutLine(const D3DXVECTOR4& color = D3DXVECTOR4(0.0f,0.0f,0.0f,1.0f),const float fOutLinewidth = 0.45f); 
	void SetModelPass(const char* pModelName);
	void SetParent(CModel* pModel);

	inline void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	inline void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	inline void SetScale(const D3DXVECTOR3& scale = INITSCALE) { m_scale = scale; }
	inline void OffSetPos(const D3DXVECTOR3& offpos) { m_offPos = offpos; }
	inline void OffSetRot(const D3DXVECTOR3& offrot) { m_offRot = offrot; }
	inline void SetPartType(PARTTYPE nDestPartType) { m_parttype = nDestPartType; }
	inline void SetMtxShadow(bool isShadow) { m_isShadow = isShadow; }
	inline void SetIsInstansing(const bool IsInstance) { m_Isinstancing = IsInstance; }

	inline D3DXMATRIX GetMtxWorld(void) { return m_mtxworld; }
	inline PARTTYPE GetPartType(void) const { return m_parttype; }
	inline int GetModelIdx(void) const { return m_nModelIdx; }
	inline bool GetIsInstansing(void) const { return m_Isinstancing; }

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="pFilename">ファイル名</param>
	/// <param name="isShadow">影の使用</param>
	/// <returns></returns>
	static CModel* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot, 
		const char* pFilename,
		const bool isShadow
	);

private:

	D3DXVECTOR3 m_pos, m_rot,m_offPos,m_offRot,m_scale; // 座標,角度,大きさ
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス
	CModel* m_pParent;		// 親モデル
	PARTTYPE m_parttype;	// モデルの種類

	int* m_pTexture;		// テクスチャポインタ
	bool m_isShadow;		// 影を設定するかどうか
	bool m_Isinstancing;	// インスタンシングするかどうか
	int m_nModelIdx;		// モデルインデックス
};