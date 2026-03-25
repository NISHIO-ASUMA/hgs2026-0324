//=========================================================
//
// カメラ処理 [ camera.cpp ]
// Author: Asuma Nishio
// NOTE : 基本の操作はゲームパッド前提で作成する
// 
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "camera.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "template.h"
#include "player.h"
#include "gamesceneobject.h"

//*********************************************************
// 定数宣言
//*********************************************************
namespace CAMERAINFO
{
	constexpr float MAX_VIEWUP = 3.0f;			// カメラの角度制限値
	constexpr float MAX_VIEWDOWN = 0.1f;		// カメラの角度制限値
	constexpr float NorRot = D3DX_PI * 2.0f;	// 正規化値

	const D3DXVECTOR3 InitPos = { 0.0f, 1110.0f, -350.0f };		 // カメラ初期座標
	const D3DXVECTOR3 InitRot = { D3DX_PI * 0.55f, 0.0f, 0.0f }; // カメラ初期角度
	const D3DXVECTOR3 InitVecU = { 0.0f, 1.0f, 0.0f };			 // 初期ベクトル
}

//=========================================================
// コンストラクタ
//=========================================================
CCamera::CCamera() : m_pCamera(ClearDefault())
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CCamera::~CCamera()
{
	
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CCamera::Init(void)
{
	m_pCamera.posV = CAMERAINFO::InitPos;	// カメラの位置
	m_pCamera.posR = VECTOR3_NULL;			// カメラの見ている位置
	m_pCamera.vecU = CAMERAINFO::InitVecU;	// 上方向ベクトル
	m_pCamera.rot = CAMERAINFO::InitRot;	// 角度

	// 距離を計算
	float fRotx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fRoty = m_pCamera.posV.y - m_pCamera.posR.y;
	float fRotz = m_pCamera.posV.z - m_pCamera.posR.z;

	// 視点から注視点までの距離
	m_pCamera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CCamera::Uninit(void)
{

}
//=========================================================
// 更新処理
//=========================================================
void CCamera::Update(void)
{
	// 右スティックのカメラ
	RightStickCamera();
	//SlidMouse();
#ifdef _DEBUG
	// マウスクリックカメラ更新
	MouseView(CManager::GetInstance()->GetMouse());

	// 追従カメラ
	FollowCamera();

#else
	// マウスのスライド移動を有効化
	SlidMouse();

#endif // _DEBUG

#ifdef _DEBUG
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_TAB))
	{
		// 初期化
		Init();
	}
#endif // _DEBUG

	// 角度の正規化
	if (m_pCamera.rot.y > D3DX_PI)
	{// D3DX_PIより大きくなったら
		m_pCamera.rot.y -= CAMERAINFO::NorRot;
	}

	// 角度の正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{// D3DX_PIより小さくなったら
		m_pCamera.rot.y += CAMERAINFO::NorRot;
	}
}
//=========================================================
// カメラをセット
//=========================================================
void CCamera::SetCamera(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_pCamera.mtxView,
		&m_pCamera.posV,
		&m_pCamera.posR,
		&m_pCamera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_pCamera.mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_pCamera.mtxprojection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_pCamera.mtxprojection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		6000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_pCamera.mtxprojection);

#ifdef _DEBUG
	// フォントセット
	CDebugproc::Print("Camera : PosV [ %.2f, %.2f, %.2f ]\n", m_pCamera.posV.x, m_pCamera.posV.y, m_pCamera.posV.z);
	CDebugproc::Draw(0, 20);

	CDebugproc::Print("Camera : PosR [ %.2f, %.2f, %.2f ]\n", m_pCamera.posR.x, m_pCamera.posR.y, m_pCamera.posR.z);
	CDebugproc::Draw(0, 40);

	CDebugproc::Print("Camera : Rot [ %.2f, %.2f, %.2f ]\n", m_pCamera.rot.x, m_pCamera.rot.y, m_pCamera.rot.z);
	CDebugproc::Draw(0, 80);
#endif // _DEBUG
}
//==============================================================
// タイトルカメラ
//==============================================================
void CCamera::SetTitleCamara(void)
{
	m_pCamera.posV = D3DXVECTOR3(0.0f, 1050.0f, -600.0f);		// カメラの位置
	m_pCamera.posR = VECTOR3_NULL;								// カメラの見ている位置
	m_pCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	m_pCamera.rot = D3DXVECTOR3(D3DX_PI * 0.6f, 0.0f, 0.0f);	// 角度

	// 距離を計算
	float fRotx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fRoty = m_pCamera.posV.y - m_pCamera.posR.y;
	float fRotz = m_pCamera.posV.z - m_pCamera.posR.z;

	// 視点から注視点までの距離
	m_pCamera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));
}
//==============================================================
// リザルトカメラ
//==============================================================
void CCamera::SetResultCamara(void)
{
	// カメラ位置
	m_pCamera.posV = D3DXVECTOR3(0.0f, 5.0f, -190.0f);
	m_pCamera.posR = D3DXVECTOR3(80.0f, 50.0f, 0.0f);
	m_pCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 回転
	m_pCamera.rot = D3DXVECTOR3(D3DX_PI * 0.3f,0.0f,0.0f);

	float fPosx = m_pCamera.posV.x - m_pCamera.posR.x;
	float fPosy = m_pCamera.posV.y - m_pCamera.posR.y;
	float fPosz = m_pCamera.posV.z - m_pCamera.posR.z;

	m_pCamera.fDistance = sqrtf(fPosx * fPosx + fPosy * fPosy + fPosz * fPosz);
}
//==============================================================
// マウス操作の視点移動
//==============================================================
void CCamera::MouseView(CInputMouse * pMouse)
{
	// 左クリック
	if (pMouse->GetPress(CInputMouse::MOUSE_LEFT))
	{
		// マウスの移動量取得
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		// 現在の角度を計算
		D3DXVECTOR2 fAngle = Move - MoveOld;

		// 回転量を更新
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// 回転量を制限
		if (m_pCamera.rot.x > CAMERAINFO::MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < CAMERAINFO::MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラの視点の情報
		m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}
	// 右クリック
	else if (pMouse->GetPress(CInputMouse::MOUSE_RIGHT))
	{
		D3DXVECTOR2 Move = pMouse->GetMouseVelocity();
		D3DXVECTOR2 MoveOld = pMouse->GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		// 回転量を更新
		m_pCamera.rot.y += fAngle.x * 0.01f;
		m_pCamera.rot.x += fAngle.y * 0.01f;

		// 回転量を制限
		if (m_pCamera.rot.x > CAMERAINFO::MAX_VIEWUP)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}
		else if (m_pCamera.rot.x < CAMERAINFO::MAX_VIEWDOWN)
		{
			m_pCamera.rot.x -= fAngle.y * 0.01f;
		}

		// カメラ座標を更新
		m_pCamera.posR.x = m_pCamera.posV.x + sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		m_pCamera.posR.y = m_pCamera.posV.y + cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		m_pCamera.posR.z = m_pCamera.posV.z + sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}

	// 正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{
		m_pCamera.rot.y += CAMERAINFO::NorRot;
	}
	else if (m_pCamera.rot.y > D3DX_PI)
	{
		m_pCamera.rot.y += -CAMERAINFO::NorRot;
	}
	if (m_pCamera.rot.x < -D3DX_PI)
	{
		m_pCamera.rot.x += CAMERAINFO::NorRot;
	}
	else if (m_pCamera.rot.x > D3DX_PI)
	{
		m_pCamera.rot.x += -CAMERAINFO::NorRot;
	}
}
//==============================================================
// マウスホイール処理
//==============================================================
void CCamera::WheelMouse(int nDelta)
{
	if (nDelta > 0)
	{// マウスの値が正
		m_pCamera.fDistance -= 20.0f;
	}
	else if (nDelta < 0)
	{// マウスの値が負
		m_pCamera.fDistance += 20.0f;
	}

	if (m_pCamera.fDistance <= 50.0f)
	{// 50.0f以下なら
		m_pCamera.fDistance = 50.0f;
	}

	// カメラの視点の情報
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
}
//==============================================================
// ターゲット追従カメラ処理
//==============================================================
void CCamera::FollowCamera(void)
{
	// 追従先のターゲットを設定
	const auto& instance = CGameSceneObject::GetInstance();
	if (instance == nullptr) return;

	// プレイヤー取得
	auto player = instance->GetPlayer();
	if (player == nullptr) return;

	// 注視点設定
	D3DXVECTOR3 targetPos = player->GetPos();
	targetPos.y += 60.0f;

	// カメラからの距離を固定化する
	m_pCamera.fDistance = 300.0f;

	// 滑らかに追従させる（線形補間）
	m_pCamera.posR += (targetPos - m_pCamera.posR) * 0.3f;

	// 計算
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
}
//==============================================================
// マウスのフリック移動によるカメラ移動
//==============================================================
void CCamera::SlidMouse(void)
{
	DIMOUSESTATE mouseState;

	if (CManager::GetInstance()->GetMouse()->GetState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00045f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		m_pCamera.rot.y += X;
		m_pCamera.rot.x += Y;

		if (m_pCamera.rot.y < -D3DX_PI)
		{
			m_pCamera.rot.y += D3DX_PI * 2.0f;
		}
		else if (m_pCamera.rot.y > D3DX_PI)
		{
			m_pCamera.rot.y += -D3DX_PI * 2.0f;
		}

		if (m_pCamera.rot.x < -D3DX_PI)
		{
			m_pCamera.rot.x += D3DX_PI * 2.0f;
		}
		else if (m_pCamera.rot.x > D3DX_PI)
		{
			m_pCamera.rot.x += -D3DX_PI * 2.0f;
		}

		if (m_pCamera.rot.x > 3.00f)
		{
			m_pCamera.rot.x -= Y;
		}
		else if (m_pCamera.rot.x < 0.1f)
		{
			m_pCamera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		//m_pCamera.posR.x = m_pCamera.posV.x + sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
		//m_pCamera.posR.y = m_pCamera.posV.y + cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
		//m_pCamera.posR.z = m_pCamera.posV.z + sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;
	}
}
//==============================================================
// 右スティックのカメラ処理
//==============================================================
void CCamera::RightStickCamera(void)
{
	// スティック
	auto pad = CManager::GetInstance()->GetJoyPad();
	XINPUT_STATE* pStick = pad->GetStickAngle();

	if (pad->GetRightStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag >= DeadZone)
		{
			float NormalizeX = RStickAngleX / fMag;
			float NormalizeY = RStickAngleY / fMag;

			float fAngle = fMag * 0.000003f;
			m_pCamera.rot.y += NormalizeX * 0.5f * fAngle;
			m_pCamera.rot.x -= NormalizeY * 0.5f * fAngle;
		}
	}

	// 角度の正規化
	if (m_pCamera.rot.y > D3DX_PI)
	{// D3DX_PIより大きくなったら
		m_pCamera.rot.y -= D3DX_PI * 2.0f;
	}

	// 角度の正規化
	if (m_pCamera.rot.y < -D3DX_PI)
	{// D3DX_PIより小さくなったら
		m_pCamera.rot.y += D3DX_PI * 2.0f;
	}

	if (m_pCamera.rot.x <= D3DX_PI * 0.55f)
	{// カメラの下限
		m_pCamera.rot.x = D3DX_PI * 0.55f;
	}

	if (m_pCamera.rot.x >= D3DX_PI * 0.9f)
	{// カメラの上限
		m_pCamera.rot.x = D3DX_PI * 0.9f;
	}
	// カメラの視点の情報
	m_pCamera.posV.x = m_pCamera.posR.x - sinf(m_pCamera.rot.x) * sinf(m_pCamera.rot.y) * m_pCamera.fDistance;
	m_pCamera.posV.y = m_pCamera.posR.y - cosf(m_pCamera.rot.x) * m_pCamera.fDistance;
	m_pCamera.posV.z = m_pCamera.posR.z - sinf(m_pCamera.rot.x) * cosf(m_pCamera.rot.y) * m_pCamera.fDistance;

}
//==============================================================
// 値のクリア関数
//==============================================================
CCamera::Camera CCamera::ClearDefault(void)
{
	// 要素のクリア
	m_pCamera.fDistance = NULL;
	m_pCamera.mtxprojection = {};
	m_pCamera.mtxView = {};
	m_pCamera.nMode = MODE_NONE;
	m_pCamera.posR = VECTOR3_NULL;
	m_pCamera.posRDest = VECTOR3_NULL;
	m_pCamera.posV = VECTOR3_NULL;
	m_pCamera.rot = VECTOR3_NULL;
	m_pCamera.vecU = VECTOR3_NULL;

	return m_pCamera;
}