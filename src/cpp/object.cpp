//=========================================================
//
// オブジェクト処理 [ object.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "object.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "manager.h"
#include "debugproc.h"
#include "pausemanager.h"
#include "camera.h"

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
int CObject::m_nNumAll = NULL;					// オブジェクト総数
CObject* CObject::m_pTop[PRIORITY_MAX] = {};	// 先頭アドレス
CObject* CObject::m_pCur[PRIORITY_MAX] = {};	// 最後尾アドレス

//=========================================================
// コンストラクタ
//=========================================================
CObject::CObject(int nPriority)
{
	m_Type = TYPE_NONE;					// オブジェクト種類を初期化
	m_isDeath = false;					// 死亡フラグを無効化

	m_pNext = nullptr;					// 次のポインタをnullptrに初期化
	m_pPrev = nullptr;					// 前のポインタをnullptrに初期化

	m_pPrev = m_pCur[nPriority];		// 現在の最後尾を前ポインタに設定

	if (m_pCur[nPriority])
	{// すでに最後尾があるなら

		m_pCur[nPriority]->m_pNext = this; // そこから自分につなげる
	}
	else
	{// 先頭がいなければ自分が先頭になる
		
		m_pTop[nPriority] = this;
	}

	m_pCur[nPriority] = this;	// 自分を最後尾として記録
	m_nNumAll++;				// 総数をカウントアップ
}
//=========================================================
// デストラクタ
//=========================================================
CObject::~CObject()
{
	
}
//=========================================================
// 全解放処理
//=========================================================
void CObject::ReleaseAll(void)
{
	// 優先番号分回す
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
	{
		// 先頭のアドレスを取得する
		CObject* pObj = m_pTop[nPri];

		// 取得できたら
		while (pObj)
		{
			// 次のオブジェクトを保存する
			CObject* pNext = pObj->m_pNext;

			// オブジェクト終了処理
			pObj->Uninit();

			// 前後を初期化
			pObj->m_pNext = nullptr;
			pObj->m_pPrev = nullptr;

			// オブジェクト自体をここで削除する
			delete pObj;

			// nullptrにする
			pObj = nullptr;

			// 次のオブジェクトに代入
			pObj = pNext;
		}

		// 先頭のポインタをnullptrにする
		m_pTop[nPri] = nullptr;

		// 最後尾のポインタをnullptrにする
		m_pCur[nPri] = nullptr;
	}

	// 総数をリセットする
	m_nNumAll = NULL;
}
//=========================================================
// オブジェクトの破棄
//=========================================================
void CObject::Release(void)
{
	// 死亡フラグを有効化するのみ
	m_isDeath = true;
}
//=========================================================
//全更新処理
//=========================================================
void CObject::UpdateAll(void)
{
	// 優先番号分回す
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		// 先頭アドレス,優先番号を取得
		CObject* pObject = m_pTop[nPrio];

		// 取得できたら
		while (pObject)
		{
			// 次のオブジェクト保存
			CObject* pObjeNext = pObject->m_pNext;

			// 死んでたら
			if (pObject->m_isDeath)
			{
				// 次のオブジェクトを代入
				pObject = pObjeNext;
				continue;
			}

			// ポーズオブジェクトのみを更新
			if (CPauseManager::GetPause())
			{
				if (pObject->GetObjType() == TYPE_PAUSE)
				{
					// 対象更新
					pObject->Update();
				}
			}
			else
			{
				// 全部のオブジェクト継承更新
				pObject->Update();
			}
			
			// 次のオブジェクトを代入
			pObject = pObjeNext;
		}
	}

	// 死亡しているかフラグで判別し対象のオブジェクトを削除
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// 先頭アドレス,優先番号を取得
		CObject* pObject = m_pTop[nCnt];

		// 取得できたら
		while (pObject)
		{
			// 次のオブジェクト保存
			CObject* pObjeNext = pObject->m_pNext;

			// フラグが有効の物を破棄する
			if (pObject->m_isDeath)
			{
				// 前のオブジェクトが存在する場合は、自分の次を前のオブジェクトに接続
				if (pObject->m_pPrev)
				{
					pObject->m_pPrev->m_pNext = pObject->m_pNext;
				}
				else
				{
					// 自分がリストの先頭なら、トップを次のオブジェクトに更新
					m_pTop[nCnt] = pObject->m_pNext;
				}

				// 次のオブジェクトが存在する場合は、自分の前を次のオブジェクトに接続
				if (pObject->m_pNext)
				{
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;
				}
				else
				{
					// 自分がリストの最後尾なら、最後尾を前のオブジェクトに更新
					m_pCur[nCnt] = pObject->m_pPrev;
				}

				// 前後を初期化
				pObject->m_pNext = nullptr;
				pObject->m_pPrev = nullptr;

				// 総数減算
				m_nNumAll--;

				// オブジェクト自身破棄
				delete pObject;
			}

			// 次のオブジェクトを代入
			pObject = pObjeNext;
		}
	}
}
//=========================================================
// 全描画処理
//=========================================================
void CObject::DrawAll(void)
{
#if 1
	// カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// カメラのセット
	pCamera->SetCamera();

	// 優先番号分回す
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		// 先頭アドレス,優先番号を取得
		CObject* pObject = m_pTop[nPrio];

		// 取得できたら
		while (pObject)
		{
			// 次のオブジェクト保存
			CObject* pObjeNext = pObject->m_pNext;

			if (!pObject->m_isDeath)
			{
				// オブジェクト描画
				pObject->Draw();
			}

			// 次のオブジェクトを代入
			pObject = pObjeNext;
		}
	}

#ifdef _DEBUG
	// デバッグフォント
	CDebugproc::Print("現在の総オブジェクト数 : %d", m_nNumAll);
	CDebugproc::Draw(0, 60);
#endif // _DEBUG
#endif
}