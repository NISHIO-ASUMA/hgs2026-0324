//=========================================================
// 
// アウトラインシェーダークラス [ outline.h ]
// Author : Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// アウトラインシェーダー適用クラスを定義
//*********************************************************
class COutLine
{
public:

	~COutLine();

	HRESULT Init(const char * pShaderFile);
	void Uninit(void);
	void Begin(void);
	void BeginPass(const int nPass = 0);
	void EndPass(void);
	void End(void);

	/// <summary>
	/// シェーダーパラメーター設定関数
	/// </summary>
	/// <param name="fOutLineWidth">アウトラインのサイズ</param>
	/// <param name="color">アウトラインのカラー</param>
	/// <param name="mtxworld">ワールドマトリックス</param>
	void SetParameter(const float fOutLineWidth, const D3DXVECTOR4& color, const D3DXMATRIX& mtxworld);

	static COutLine* GetInstance(void)
	{
		static COutLine pInstance;
		return &pInstance;
	};

private:

	COutLine();					// 格納コンストラクタ

	LPD3DXEFFECT m_pOutLine;	// シェーダーエフェクトポインタ
};