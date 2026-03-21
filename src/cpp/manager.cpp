//=========================================================
//
// マネージャー処理 [ manager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "manager.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "result.h"
#include "ranking.h"
#include "object.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "fade.h"
#include "xfilemanager.h"
#include "motionmanager.h"
#include "modelmanager.h"
#include "outline.h"
#include "jsonmanager.h"

//*********************************************************
// 定数名前空間宣言
//*********************************************************
namespace MANAGERBASE
{
	constexpr const char* OUTLINEFILE = "data/SHADER/Out_Line.hlsl";		// アウトラインシェーダーファイル
	constexpr const char* INSTANCINGNAME = "data/SHADER/Instancing.hlsl";	// インスタンシングシェーダーファイル
};

//=========================================================
// コンストラクタ
//=========================================================
CManager::CManager() : m_pCamera(nullptr),
m_pFade(nullptr),
m_pInputKeyboard(nullptr),
m_pInputMouse(nullptr),
m_pJoyPad(nullptr),
m_pLight(nullptr),
m_pRenderer(nullptr),
m_pScene(nullptr),
m_pSound(nullptr),
m_pTexture(nullptr),
m_pXfileManager(nullptr),
m_pMotionManager(nullptr),
m_pModelManager(nullptr),
m_pJsonManager(nullptr)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CManager::~CManager()
{
	
}
//=========================================================
// インスタンス取得処理
//=========================================================
CManager* CManager::GetInstance(void)
{
	static CManager pManeger;
	return &pManeger;
};
//=========================================================
// マネージャーの初期化処理
//=========================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// キーボードオブジェクトの生成処理
	m_pInputKeyboard = std::make_unique<CInputKeyboard>();
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd))) return E_FAIL;

	// ジョイパッドオブジェクトの生成処理
	m_pJoyPad = std::make_unique<CJoyPad>();
	if (FAILED(m_pJoyPad->Init(hInstance, hWnd))) return E_FAIL;

	// マウスオブジェクトの生成処理
	m_pInputMouse = std::make_unique<CInputMouse>();
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd))) return E_FAIL;

	// サウンドの生成処理
	m_pSound = std::make_unique<CSound>();
	if (FAILED(m_pSound->Init(hWnd))) return E_FAIL;

	// カメラの生成処理
	m_pCamera = std::make_unique<CCamera>();
	if (FAILED(m_pCamera->Init())) return E_FAIL;

	// レンダラーの生成処理
	m_pRenderer = std::make_unique <CRenderer>();
	if (FAILED(m_pRenderer->Init(hWnd, bWindow))) return E_FAIL;

	// ライトの生成処理
	m_pLight = std::make_unique <CLight>();
	if (FAILED(m_pLight->Init())) return E_FAIL;

	// テクスチャの生成処理
	m_pTexture = std::make_unique <CTexture>();
	if (FAILED(m_pTexture->Load())) return E_FAIL;

	// Xファイルオブジェクトマネージャーの生成処理
	m_pXfileManager = std::make_unique <CXfileManager>();
	if (FAILED(m_pXfileManager->Load())) return E_FAIL;

	// キャラクターモデル管理クラスの生成処理
	m_pModelManager = std::make_unique<CModelManager>();
	if (FAILED(m_pModelManager->Load())) return E_FAIL;

	// モーションマネジャーの生成処理
	m_pMotionManager = std::make_unique<CMotionManager>();
	if (FAILED(m_pMotionManager->Load())) return E_FAIL;

	// jsonマネージャークラスの生成
	m_pJsonManager = std::make_unique<CJsonManager>();
	if (FAILED(m_pJsonManager->Init())) return E_FAIL;

	// アウトラインクラス生成
	COutLine::GetInstance()->Init(MANAGERBASE::OUTLINEFILE);

	// フェードの生成処理
	m_pFade = std::make_unique <CFade>();
	if (FAILED(m_pFade->Init())) return E_FAIL;

#ifdef _DEBUG
	// デバッグ用シーンセット
	m_pFade->SetFade(std::make_unique<CTitle>());
#else
	// シーンセット
	m_pFade->SetFade(std::make_unique<CResult>());
#endif // _DEBUG

	// 乱数の種を一度宣言する
	srand((unsigned int)time(nullptr));

	return S_OK;
}
//=========================================================
// マネージャーの終了処理
//=========================================================
void CManager::Uninit(void)
{
	// キーボードインスタンスの破棄
	m_pInputKeyboard.reset();

	// ゲームパッドインスタンスの破棄
	m_pJoyPad.reset();

	// マウスインスタンスの破棄
	m_pInputMouse.reset();

	// サウンドインスタンスの破棄
	m_pSound.reset();

	// カメラインスタンスの破棄
	m_pCamera.reset();

	// ライトインスタンスの破棄
	m_pLight.reset();

	// テクスチャインスタンスの破棄
	m_pTexture.reset();

	// モデルインスタンスの破棄
	m_pXfileManager.reset();

	// キャラクターモデルの破棄
	m_pModelManager.reset();

	// モーションマネージャーの破棄
	m_pMotionManager.reset();

	// jsonマネージャーの破棄
	m_pJsonManager.reset();

	// アウトラインの破棄
	COutLine::GetInstance()->Uninit();

	// シーンの破棄
	if (m_pScene)
	{
		m_pScene->Uninit();
		m_pScene.reset();
	}

	// フェードの破棄
	if (m_pFade)
	{
		m_pFade->Uninit();
		m_pFade.reset();
	}

	// レンダラーの破棄
	if (m_pRenderer)
	{
		// レンダラーの終了処理
		m_pRenderer->Uninit();
		m_pRenderer.reset();
	}
}
//=========================================================
// マネージャーの更新処理
//=========================================================
void CManager::Update()
{
	// キーボードの更新処理
	if (m_pInputKeyboard) m_pInputKeyboard->Update();

	// ジョイパッドの更新処理
	if (m_pJoyPad) m_pJoyPad->Update();

	// マウスの更新処理
	if (m_pInputMouse) m_pInputMouse->Update();

	// ジョイパッド振動の更新処理
	m_pJoyPad->UpdateVibration();

	// カメラ更新
	if (m_pCamera) m_pCamera->Update();

	// フェードの更新
	if (m_pFade) m_pFade->Update();

	// シーンの更新
	if (m_pScene) m_pScene->Update();

	// レンダラーの更新処理
	if (m_pRenderer) m_pRenderer->Update();
}
//=========================================================
// マネージャーの描画処理
//=========================================================
void CManager::Draw(void)
{
	// レンダラーの描画処理
	if (m_pRenderer) m_pRenderer->Draw();
}
//=========================================================
// シーンのセット
//=========================================================
void CManager::SetScene(std::unique_ptr<CScene> pNewScene)
{
	// nullptrじゃない
	if (m_pScene != nullptr)
	{
		// 終了処理
		m_pScene->Uninit();

		// nullじゃない
		if (m_pSound)
		{
			// サウンドの停止
			m_pSound->StopSound();
		}

		// 全オブジェクト破棄
		CObject::ReleaseAll();

		// インスタンシング破棄
		m_pRenderer->ClearDrawInstance();

		// 古いシーンを破棄
		m_pScene.reset();
	}

	// もしシーンが無かったら
	if (pNewScene)
	{
		// シーンの初期化
		if (FAILED(pNewScene->Init()))
		{
			// 失敗時
			MessageBox(GetActiveWindow(), "シーン初期化失敗", "例外スロー", MB_OK);
			pNewScene.reset();
			return;
		}

		// 所有権をマネージャに移動
		m_pScene = std::move(pNewScene);
	}
}
//=========================================================
// 現在シーン取得
//=========================================================
CScene::MODE CManager::GetScene(void)
{
	// nullptrじゃない
	if (m_pScene)
		return m_pScene->GetScene(); // 現在シーン
	else
		return CScene::MODE_NONE;	 // 何もないシーン
}