//=========================================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "player.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "boxcollider.h"
#include "collisionbox.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "gamesceneobject.h"
#include "block.h"
#include "debugproc.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_isWall(false),
m_isLanding(false),
m_isJump(false),
m_pBoxCollder(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CPlayer::~CPlayer()
{

}
//=========================================================
// 生成処理
//=========================================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// インスタンス生成
	CPlayer* pPlayer = new CPlayer;
	if (pPlayer == nullptr) return nullptr;

	// オブジェクト設定
	pPlayer->SetPos(pos);
	pPlayer->SetOldPos(pos);
	pPlayer->SetRot(rot);

	// 初期化
	if (FAILED(pPlayer->Init())) return nullptr;

	return pPlayer;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPlayer::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// モーションロード
	CMoveCharactor::MotionLoad(Config::SCRIPT,MOTION::MAX,false);

	// コライダー生成
	m_pBoxCollder = CBoxCollider::Create
	(
		GetPos(), 
		GetOldPos(), 
		D3DXVECTOR3(Config::COLLISION, Config::COLLISION, Config::COLLISION)
	);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPlayer::Uninit(void)
{
	// コライダー破棄
	m_pBoxCollder.reset();

	// 親クラスの終了処理
	CMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CPlayer::Update(void)
{
#if 0
	// 現在の座標取得
	auto pos = GetPos();
	auto oldPos = GetOldPos();

	// キー入力
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	// キー入力移動
	KeyMove();
	KeyPad();

	// 更新座標を取得
	auto Updatepos = GetPos();
	auto Move = GetMove();

	// 矩形コライダーの位置更新
	if (m_pBoxCollder)
	{
		m_pBoxCollder->SetPos(Updatepos);
		m_pBoxCollder->SetPosOld(oldPos);
	}

	// ジャンプキー
	if (pKey->GetTrigger(DIK_SPACE) && m_isLanding)
	{
		m_isJump = true;
		m_isLanding = false;
		Move.y += 15.0f;
		SetMove(Move);
	}

	// ジャンプ  かつ 着地
	if (m_isJump && m_isLanding)
	{
		m_isJump = false;
	}

	//================================
	// 重力落下
	//================================
	auto MoveDown = GetMove();
	if (!m_isLanding) 
	{
		MoveDown.y -= 0.7f; // 空中にいる時だけ重力をかける
	}
	else 
	{
		MoveDown.y = 0.0f;	// 接地中は下方向への移動量をリセット
	}
	// 最終重力値を設定
	SetMove(MoveDown);

#if 1
	if (0.0f >= Updatepos.y)
	{// 地面より下
		Updatepos.y = 0.0f;
		SetPos(Updatepos);
		//m_isLanding = true;
	}
#endif

	// 座標更新
	UpdatePosition();

	// ブロックとのコリジョン
	auto block = CGameSceneObject::GetInstance()->GetBlock();

	// 当たっていたら
	if (CollisionBox(block->GetCollider(), &Updatepos))
	{
		// 座標セット
		SetPos(Updatepos);

		// コライダー座標の更新
		m_pBoxCollder->SetPos(Updatepos);
	}

	// 親クラスの更新処理
	CMoveCharactor::Update();
#else
	// 入力クラスと現在の座標情報を取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();

	auto oldPos = GetOldPos();

	// まず「現在の移動量」を取得する
	KeyMove();
	KeyPad();

	// キー入力後の「最新の移動量」を取り出す (XZ軸の入力が反映済み)
	D3DXVECTOR3 move = GetMove();

	// 重力の計算 (Y軸の処理)
	if (!m_isLanding)
	{
		move.y -= 0.7f; // 滞空時のみ重力
	}
	else
	{
		move.y = 0.0f;  // 接地時はリセット
	}

	// ジャンプ入力
	if ((pKey->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CJoyPad::JOYKEY_A))&& m_isLanding)
	{
		move.y = 15.0f;
		m_isLanding = false;
		m_isJump = true;
	}

	// 最終的な移動量を反映
	SetMove(move);

	// 物理的な位置更新 (UpdatePosition内部で座標 = 座標 + move される)
	UpdatePosition();

	// 移動後の最新座標を取得
	D3DXVECTOR3 updatePos = GetPos();

	// 地面(y=0)との衝突判定
	if (updatePos.y <= 0.0f)
	{
		updatePos.y = 0.0f;
		m_isLanding = true;
		m_isJump = false;
	}

	// コライダーの更新 (判定を行うための最新状態にする)
	if (m_pBoxCollder)
	{
		m_pBoxCollder->SetPos(updatePos);
		m_pBoxCollder->SetPosOld(oldPos);
	}

	// ブロックとの衝突判定
	auto block = CGameSceneObject::GetInstance()->GetBlock();
	if (CollisionBox(block->GetCollider(), &updatePos))
	{
		SetPos(updatePos);	// 押し戻し適用
		m_isLanding = true; // ここで着地フラグを立てる
	}
	else
	{
		// 地面(y=0)にもいなければ、空中とする
		if (updatePos.y > 0.0f) { m_isLanding = false;}
	}

	// 最後に座標を確定
	SetPos(updatePos);

	// 親クラスの更新
	CMoveCharactor::Update();
#endif
}
//=========================================================
// 矩形コライダー判定処理
//=========================================================
bool CPlayer::CollisionBox(CBoxCollider* pOther, D3DXVECTOR3* pOutPos)
{
	// y軸の移動量を取得
	float moveY = GetMove().y;

	// 先に結果を受け取る
	bool isHit = CCollisionBox::Collision(m_pBoxCollder.get(), pOther, pOutPos, m_isLanding, moveY);

	// 移動量を設定
	SetMove(D3DXVECTOR3(GetMove().x, moveY, GetMove().z));

	// 結果を返す
	return isHit;
}
//=========================================================
// 描画処理
//=========================================================
void CPlayer::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();

	// デバッグフォント
	CDebugproc::Print("着地 : %d", m_isLanding);
	CDebugproc::Draw(0, 180);

	CDebugproc::Print("座標 : [ %.2f,%.2f,%.2f ]", GetPos().x, GetPos().y, GetPos().z);
	CDebugproc::Draw(0, 200);
}
//=========================================================
// キー入力移動
//=========================================================
void CPlayer::KeyMove(void)
{
	// 入力デバイス取得
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	// 移動量を取得
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 rotdest = GetRotDest();

	// パッドのスティック入力があったら
	if (pPad->GetLeftStick()) return;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera == nullptr) return;

	// 移動フラグ
	bool isMove = false;

	if (pKey->GetPress(DIK_A) || pPad->GetPress(CJoyPad::JOYKEY_LEFT))
	{// Aキー

		if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_RIGHT))
		{// 左斜め上

			move.x += sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::MOVESPEED;
			move.z += cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
			isMove = true;
		}
		else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
		{// 右斜め下

			move.x -= sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::MOVESPEED;
			move.z -= cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
			isMove = true;

		}
		else
		{// 単体
			move.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::MOVESPEED;
			move.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
			isMove = true;
		}

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_D) || pPad->GetPress(CJoyPad::JOYKEY_RIGHT))
	{// Dキーを押した

		if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_UP))
		{// Wキーを押した
			move.x += sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::MOVESPEED;
			move.z += cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.75f);
			isMove = true;
		}
		else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
		{// Sキーを押した
			move.x -= sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::MOVESPEED;
			move.z -= cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.25f);
			isMove = true;
		}
		else
		{// Dキーのみ押した
			move.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::MOVESPEED;
			move.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * Config::MOVESPEED;
			rotdest.y = pCamera->GetRot().y - (D3DX_PI * 0.5f);
			isMove = true;
		}

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_W) || pPad->GetPress(CJoyPad::JOYKEY_UP))
	{// Wキーを押した

		move.x += sinf(pCamera->GetRot().y) * Config::MOVESPEED;
		move.z += cosf(pCamera->GetRot().y) * Config::MOVESPEED;
		rotdest.y = pCamera->GetRot().y - D3DX_PI;
		isMove = true;

		// 角度を正規化
		if (rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			rot.y += Config::NorRot;
		}
	}
	else if (pKey->GetPress(DIK_S) || pPad->GetPress(CJoyPad::JOYKEY_DOWN))
	{// Sキーを押した

		move.x -= sinf(pCamera->GetRot().y) * Config::MOVESPEED;
		move.z -= cosf(pCamera->GetRot().y) * Config::MOVESPEED;
		rotdest.y = pCamera->GetRot().y;
		isMove = true;

		// 角度の正規化
		if (rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			rot.y -= Config::NorRot;
		}
	}

	if (isMove)
	{
		//// MOVEじゃなかったら切り替え
		//if (GetMotion()->GetMotionType() != MOVE)
		//{
		//	GetMotion()->SetMotion(MOVE);
		//}
	}
	else
	{
		//// NEUTRALに遷移する
		//if (GetMotion()->GetMotionType() == MOVE)
		//{
		//	//　タイプ切り替え
		//	GetMotion()->SetMotion(NEUTRAL, true, 10);
		//}
	}

	if (rotdest.y - rot.y > D3DX_PI)
	{// 左回転
		// 角度
		rot.y = rot.y + Config::NorRot;
	}
	else if (rot.y - rotdest.y > D3DX_PI)
	{// 右回転
		// 角度
		rot.y = rot.y - Config::NorRot;
	}

	// 現在の角度
	rot.y += (rotdest.y - rot.y) * 0.25f;

	// キャラクターに適用する
	SetRot(rot);
	SetRotDest(rotdest);
	SetMove(move);
}
//=========================================================
// パッド移動
//=========================================================
void CPlayer::KeyPad(void)
{
	// パッドの取得
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();

	// 移動量を取得
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 rotdest = GetRotDest();

	// 入力デバイス取得
	XINPUT_STATE* pStick = pPad->GetStickAngle();

	// 入力受け取り失敗時
	if (!pPad->GetLeftStick()) return;

	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera == nullptr) return;

	// フラグ
	bool isMoving = false;
	static bool wasStick = false;

	// 取得できたら
	if (pPad->GetLeftStick())
	{
		// 左スティックの角度
		float LStickAngleY = pStick->Gamepad.sThumbLY;
		float LStickAngleX = pStick->Gamepad.sThumbLX;

		// デッドゾーンを設定
		float DeadZone = 32767.0f * 0.25f;
		float fMag = sqrtf((LStickAngleX * LStickAngleX) + (LStickAngleY * LStickAngleY));

		if (fMag > DeadZone)
		{
			// 正規化
			float normalizeX = (LStickAngleX / fMag);
			float normalizeY = (LStickAngleY / fMag);

			// 移動量を計算する
			float MoveX = normalizeX * cosf(-pCamera->GetRot().y) - normalizeY * sinf(-pCamera->GetRot().y);
			float MoveZ = normalizeX * sinf(-pCamera->GetRot().y) + normalizeY * cosf(-pCamera->GetRot().y);

			// 移動量を設定
			move.x += MoveX * Config::MOVESPEED;
			move.z += MoveZ * Config::MOVESPEED;
			rotdest.y = atan2f(-MoveX, -MoveZ);
			isMoving = true;
		}
	}

	if (isMoving)
	{
		//// MOVEじゃなかったら
		//if (GetMotion()->GetMotionType() != MOVE)
		//{
		//	// モーション変更
		//	GetMotion()->SetMotion(MOVE);
		//}
	}
	else if (!isMoving && wasStick)
	{
		//// 離した瞬間の判定
		//if (GetMotion()->GetMotionType() == MOVE)
		//	GetMotion()->SetMotion(NEUTRAL, true, 10);
	}

	// フラグを変更する
	wasStick = isMoving;

	// 適用
	SetMove(move);
	SetRotDest(rotdest);
}
