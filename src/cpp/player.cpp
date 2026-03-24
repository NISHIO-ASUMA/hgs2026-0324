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
#include "template.h"
#include "blockmanager.h"

//=========================================================
// コンストラクタ
//=========================================================
CPlayer::CPlayer(int nPriority) : CMoveCharactor(nPriority),
m_isWall(false),
m_isLanding(false),
m_isJump(false),
m_isStayPos(false),
m_pBoxCollder(nullptr),
m_TargetPos(VECTOR3_NULL)
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

	// 初期化失敗時
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
	// 入力クラス取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();

	// 過去座標を取得
	auto oldPos = GetOldPos();

	// まず「現在の移動量」を取得する
	KeyMove();
	KeyPad();

	// キー入力後の「最新の移動量」を取り出す
	D3DXVECTOR3 move = GetMove();

	// 重力の計算 (Y軸の処理)
	if (!m_isLanding && !m_isWall)
	{
		move.y -= 0.7f; // 滞空時のみ重力
	}
	else
	{
		move.y = 0.0f;  // 接地時はリセット
	}

	// ジャンプ入力
	if ((pKey->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CJoyPad::JOYKEY_A)) && m_isLanding)
	{
		move.y = Config::JUMP;
		m_isLanding = false;
		m_isJump = true;
	}

	// 最終的な移動量を反映
	SetMove(move);

	// アクションキー情報
	Action();

	// 位置更新
	UpdatePosition();

	// 移動後の最新座標を取得
	D3DXVECTOR3 updatePos = GetPos();
	D3DXVECTOR3 updateposold = GetOldPos();

	// 地面との衝突判定
	if (updatePos.y <= 0.0f)
	{
		updatePos.y = 0.0f;
		m_isLanding = true;
		m_isJump = false;
		m_isWall = false;
	}

	// コライダーの更新
	if (m_pBoxCollder)
	{
		m_pBoxCollder->SetPos(updatePos);
		m_pBoxCollder->SetPosOld(updateposold);
	}

	// ブロックとの衝突判定
	auto BlockManager = CGameSceneObject::GetInstance()->GetBlockManager();

	// ヒット関係のフラグ
	bool isAnyHit = (updatePos.y <= 0.0f);

	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		// 配列のブロック取得
		auto childblock = BlockManager->GetBlock(nCnt)->GetCollider();

		if (CollisionBox(childblock, &updatePos))
		{
			// 押し戻し適用
			CMoveCharactor::SetPos(updatePos);

			// コライダー座標更新
			m_pBoxCollder->SetPos(updatePos);
			//m_pBoxCollder->SetPosOld(updatePos);
			isAnyHit = true;
			break;
		}
	}

	// 全ブロックのループが終わった後に、最終的な接地状態を反映させる
	m_isLanding = isAnyHit;
	if (m_isLanding)
	{
		m_isJump = false;
		m_isWall = false;
	}

	// 親クラスの更新
	CMoveCharactor::Update();

#else
	// 入力クラス取得
	CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();
	CJoyPad* pPad = CManager::GetInstance()->GetJoyPad();

	// 過去座標を取得
	auto oldPos = GetOldPos();

	// 移動入力の制御
	if (!m_isStayPos)
	{
		// 通常時のみキー入力を受け付ける
		KeyMove();
		KeyPad();
	}
	else 
	{
		// アクション中は入力をリセット
		SetMove(VECTOR3_NULL);
	}

	// 最新の移動量を取り出す
	D3DXVECTOR3 move = GetMove();

	// --- 2. 重力の計算 ---
	if (!m_isLanding && !m_isStayPos)
	{
		move.y -= 0.7f; // 通常の落下
	}
	else
	{
		move.y = 0.0f;	// 接地、アクション中、または滞空停止時はリセット
	}

	// --- 入力があったら滞空を解除する ---
	if (pKey->GetTrigger(DIK_M)) 
	{
		m_isStayPos = false;
	}

	// ジャンプ入力 (アクション中は不可にするのが一般的)
	if (!m_isWall && (pKey->GetTrigger(DIK_SPACE) || pPad->GetTrigger(CJoyPad::JOYKEY_A)) && m_isLanding)
	{
		move.y = Config::JUMP;
		m_isLanding = false;
		m_isJump = true;
	}

	SetMove(move);

	// --- 3. アクションの実行 ---
	// ここで SetMove(moveVec) が呼ばれる
	Action();

	// 位置更新 (UpdatePosition内部で pos += move される)
	UpdatePosition();

	D3DXVECTOR3 updatePos = GetPos();
	D3DXVECTOR3 updateposold = GetOldPos();

	// 地面(Y=0)との衝突判定
	if (updatePos.y <= 0.0f)
	{
		updatePos.y = 0.0f;
		m_isLanding = true;
		m_isJump = false;
		m_isStayPos = false;
	}

	// コライダー更新
	if (m_pBoxCollder)
	{
		m_pBoxCollder->SetPos(updatePos);
		m_pBoxCollder->SetPosOld(updateposold);
	}

	// --- 4. ブロックとの衝突判定 ---
	auto BlockManager = CGameSceneObject::GetInstance()->GetBlockManager();
	bool isAnyHit = (updatePos.y <= 0.0f);

	for (int nCnt = 0; nCnt < BlockManager->GetAll(); nCnt++)
	{
		auto childblock = BlockManager->GetBlock(nCnt)->GetCollider();
		if (CollisionBox(childblock, &updatePos))
		{
			CMoveCharactor::SetPos(updatePos);
			m_pBoxCollder->SetPos(updatePos);
			isAnyHit = true;
			break; // 他の壁判定も行うため、できれば外す
		}
	}

	// --- 5. フラグの最終確定 ---
	m_isLanding = isAnyHit;
	if (m_isLanding)
	{
		m_isJump = false;
	}

	// 親クラスの更新
	CMoveCharactor::Update();
#endif
}
//=========================================================
// 描画処理
//=========================================================
void CPlayer::Draw(void)
{
	// 親クラスの描画処理
	CMoveCharactor::Draw();

#ifdef _DEBUG
	// デバッグフォント
	CDebugproc::Print("着地 : %d", m_isLanding);
	CDebugproc::Draw(0, 180);

	CDebugproc::Print("座標 : [ %.2f,%.2f,%.2f ]", GetPos().x, GetPos().y, GetPos().z);
	CDebugproc::Draw(0, 200);
#endif // _DEBUG
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
//=========================================================
// アクション操作
//=========================================================
void CPlayer::Action(void)
{
	// 自動移動フラグが立っている間の処理
	if (m_isWall)
	{
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 followVec = m_TargetPos - pos;
		float fDistance = D3DXVec3Length(&followVec);

		if (fDistance > Action::CheckDistance)
		{
			// 移動継続
			D3DXVec3Normalize(&followVec, &followVec);
			D3DXVECTOR3 moveVec = followVec * Action::AUTOSPEED;

			float angleY = atan2(-moveVec.x, -moveVec.z);
			D3DXVECTOR3 rotDest = GetRotDest();
			rotDest.y = NormalAngle(angleY);
			SetRotDest(rotDest);

			SetMove(moveVec);
		}
		else
		{
			// 到着した時だけフラグ
			SetPos(m_TargetPos);
			SetMove(VECTOR3_NULL);
			m_isWall = false; // 終了！
			m_isStayPos = true;   // その場に留まるフラグをON
		}
	}
}
//=========================================================
// wallアクションを設定する
//=========================================================
void CPlayer::ActionSetting(const D3DXVECTOR3& pos)
{
	// ターゲット座標を設定
	m_TargetPos = pos;

	// 壁移動フラグを起動
	m_isWall = true;
}