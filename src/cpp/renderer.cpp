//=========================================================
//
// レンダリング処理 [ renderer.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// クラス定義ヘッダーファイル
//*********************************************************
#include "renderer.h"

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "fade.h"
#include "modelmanager.h"
#include "model.h"
#include "texture.h"

//*********************************************************
// 定数名前空間宣言
//*********************************************************
namespace RENDERINFO
{
	const UINT MAX_INSTANCE = 2048; // 最大インスタンス数
};

//*********************************************************
// 静的メンバ変数宣言
//*********************************************************
CDebugproc* CRenderer::m_pDebug = nullptr;	// デバッグプロセスへのポインタ

//=========================================================
// コンストラクタ
//=========================================================
CRenderer::CRenderer() : m_pD3D(nullptr),
m_pD3DDevice(nullptr),
m_fps(NULL)
{
	
}
//=========================================================
// デストラクタ
//=========================================================
CRenderer::~CRenderer()
{
	
}
//=========================================================
// レンダラー初期化処理
//=========================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// ディスプレイモード
	D3DDISPLAYMODE d3ddm = {};

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							//パラメーターの0クリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;									// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして16bitを使用
	m_d3dpp.Windowed = bWindow;										// ウインドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成 (描画処理と頂点処理をハードウェアで行う)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成 (描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成 (描画処理,頂点処理をCPUで行う)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// デバッグフォント初期化
	m_pDebug = new CDebugproc;
	m_pDebug->Init();

	return S_OK;
}
//=========================================================
// レンダラー終了処理
//=========================================================
void CRenderer::Uninit(void)
{
	// デバッグプロセスの終了処理
	if (m_pDebug != nullptr)
	{
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	// 全オブジェクト破棄
	CObject::ReleaseAll();

	// インスタンシング頂点バッファの破棄
	if (m_instanceVB)
	{
		m_instanceVB->Release();
		m_instanceVB = nullptr;
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}
//=========================================================
// レンダラー更新処理
//=========================================================
void CRenderer::Update(void)
{
	// デバッグ情報の更新処理
	m_pDebug->Update();

	// 全オブジェクト更新処理
	CObject::UpdateAll();

#ifdef _DEBUG

	// キー入力情報取得
	CInputKeyboard* pInput = CManager::GetInstance()->GetInputKeyboard();

	// ワイヤーフレーム設定
	if (pInput->GetTrigger(DIK_F3))
	{
		// ワイヤーフレームON
		OnWireFrame();
	}
	else if (pInput->GetTrigger(DIK_F4))
	{
		// ワイヤーフレームOFF
		OffWireFrame();
	}

#endif // _DEBUG
}
//=========================================================
// レンダラー描画処理
//=========================================================
void CRenderer::Draw(void)
{
	// 画面クリア (バックバッファ&Zバッファ&ステンシルバッファのクリア)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画成功時

		// 全オブジェクト描画
		CObject::DrawAll();

		// シーン取得
		CScene*pScene = CManager::GetInstance()->GetSceneRaw();

		// シーンの描画
		if (pScene != nullptr) pScene->Draw();

		// フォントセット
		m_pDebug->Print("FPS : %d\n", m_fps);

		// デバッグフォントの描画
		m_pDebug->Draw(0, 0);

		// フェード描画
		CManager::GetInstance()->GetFade()->Draw();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
#if 0
//=========================================================
// インスタンシングオブジェクト登録関数
//=========================================================
void CRenderer::AddInstanceObject(const int nIdxModel,CInstanceModel* pModel)
{
	// 対象オブジェクトをインスタンシング配列に追加
	m_RegisterInstObject[nIdxModel].push_back(pModel);
}

//=========================================================
// インスタンシング描画関数
//=========================================================
void CRenderer::DrawInstancingAll(void)
{
	// モデルマネージャー取得
	auto* modelMgr = CManager::GetInstance()->GetInstanceModelManager();
	auto& models = modelMgr->GetList();

	for (auto& pair : m_RegisterInstObject)
	{
		// 配列の最初と次を取得
		int nModelIdx = pair.first;
		auto& instances = pair.second;

		// 情報が無かったら
		if (instances.empty())
			continue;

		// 対象オブジェクトの情報を格納
		auto& modelInfo = models[nModelIdx];
		auto& modelData = modelInfo.InstanceData;

		//===================================
		// インスタンシング頂点バッファ更新
		//===================================
		InstanceData* pInst = nullptr;

		// 頂点バッファをロック
		m_instanceVB->Lock(0, 0, (void**)&pInst, D3DLOCK_DISCARD);

		for (int nCnt = 0; nCnt < static_cast<int>(instances.size()); nCnt++)
		{
			// ワールドマトリックス設定
			pInst[nCnt].mtxworld = instances[nCnt]->GetMtxWorld();
		}

		// 頂点バッファをアンロック
		m_instanceVB->Unlock();

		//=============================
		// マテリアル単位ループ
		//=============================
		for (DWORD matID = 0; matID < modelInfo.dwNumMat; matID++)
		{
			//=========================
			// マテリアルカラー取得
			//=========================
			D3DXVECTOR4 matColor = VECTOR4_NULL;

			if (modelInfo.pBuffMat)
			{
				// モデルのマテリアルを取得する
				auto& mat = modelInfo.Materials[matID];

				// D3DXVECTOR4に変換する
				matColor = D3DXVECTOR4
				(
					mat.Diffuse.r,
					mat.Diffuse.g,
					mat.Diffuse.b,
					mat.Diffuse.a
				);
			}

			//=========================
			// インスタンシング描画
			//=========================
			CInstancing::GetInstance()->Begin();
			CInstancing::GetInstance()->BeginPass();

			// インスタンシング開始関数
			CInstancing::GetInstance()->BeginInstancing
			(
				instances.size(),
				modelData.VtxBuffer,
				sizeof(MODEL_3D),
				modelData.IndexBuffer,
				m_instanceVB,
				sizeof(InstanceData)
			);

			// シェーダーパラメーターセット
			CInstancing::GetInstance()->SetInstancingParam(nullptr, matColor);

			// ポリゴン描画
			m_pD3DDevice->DrawIndexedPrimitive
			(
				D3DPT_TRIANGLELIST,
				0,
				0,
				modelData.vtxCount,
				0,
				modelData.PrimCount
			);

			// インスタンシングパラメーターリセット
			CInstancing::GetInstance()->EndInstancing();

			// インスタンシング終了
			CInstancing::GetInstance()->EndPass();
			CInstancing::GetInstance()->End();
		}
	}

	// セカンドの配列クリア
	for (auto& pair : m_RegisterInstObject)
	{
		pair.second.clear();
	}
}
#endif
//=========================================================
// ワイヤーフレーム起動
//=========================================================
void CRenderer::OnWireFrame(void)
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=========================================================
// ワイヤーフレーム終了
//=========================================================
void CRenderer::OffWireFrame(void)
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}