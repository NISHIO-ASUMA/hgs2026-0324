//=========================================================
//
// カメラ処理 [ camera.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// 前方宣言
//*********************************************************
class CInputMouse;

//*********************************************************
// カメラクラスを定義
//*********************************************************
class CCamera
{
public:

	//*************************
	// カメラモード列挙型
	//*************************
	enum MODE
	{
		MODE_NONE,
		MODE_MOUSE,
		MODE_SHAKE,
		MODE_MAX
	};

	//*************************
	// カメラ構造体を定義
	//*************************
	struct Camera
	{
		D3DXMATRIX mtxprojection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;			// ビューマトリックス
		D3DXVECTOR3 posV, posR;		// 視点,注視点座標
		D3DXVECTOR3 rot;			// 角度
		D3DXVECTOR3 vecU;			// 法線ベクトル
		D3DXVECTOR3 posRDest;		// 目的座標
		float fDistance;			// カメラの距離
		int nMode;					// カメラのモード
	};

	CCamera();
	~CCamera();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void SetResultCamara(void);
	void SetTitleCamara(void);

	void MouseView(CInputMouse* pMouse);
	void WheelMouse(int nDelta);

	void FollowCamera(void);
	void RightStickCamera(void);

	void SetMode(int nMode) { m_pCamera.nMode = nMode; }
	void SetRot(D3DXVECTOR3 rot) { m_pCamera.rot = rot; }

	inline D3DXVECTOR3 GetRot(void) { return m_pCamera.rot; }
	inline D3DXVECTOR3 GetPos(void) { return m_pCamera.posV; }
	inline D3DXVECTOR3 GetPosR(void) { return m_pCamera.posR; }
	inline D3DXMATRIX GetMtxProjection(void) { return m_pCamera.mtxprojection; }
	inline D3DXMATRIX GetView(void) { return m_pCamera.mtxView; }

	int GetMode(void) { return m_pCamera.nMode; }

private:

	Camera ClearDefault(void);	// クリア用関数
	Camera m_pCamera;			// カメラ構造体変数
};