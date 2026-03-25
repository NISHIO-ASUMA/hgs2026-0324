//=========================================================
//
// ライト処理 [ light.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "light.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CLight::CLight() : m_aLight{}, m_vecDir{}
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CLight::~CLight()
{
	
}
//=========================================================
// ライトの初期化処理
//=========================================================
HRESULT CLight::Init(void)
{
	// デバイスポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ライトのゼロクリア処理
	ZeroMemory(&m_aLight, sizeof(m_aLight));

	for (auto &light : m_aLight)
	{
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse = COLOR_WHITE;
	}

	// ライトの方向設定
	m_vecDir[0] = D3DXVECTOR3(0.25f, -0.50f, -0.23f);
	m_vecDir[1] = D3DXVECTOR3(-0.61f, 0.10f, 0.44f);
	m_vecDir[2] = D3DXVECTOR3(1.0f, -0.52f, 0.21f);

	// 各ライトの設定
	for (int nCnt = 0; nCnt < NUMLIGHT; nCnt++)
	{	
		// 正規化
		D3DXVec3Normalize(&m_vecDir[nCnt], &m_vecDir[nCnt]); 

		m_aLight[nCnt].Direction = m_vecDir[nCnt];

		// ライトの設定
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		// ライトを有効化
		pDevice->LightEnable(nCnt, TRUE);
	}

	return S_OK;
}