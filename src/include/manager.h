//=========================================================
//
// マネージャー処理 [ manager.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// システムインクルードファイル
//*********************************************************
#include <memory>

//*********************************************************
// インクルードファイル
//*********************************************************
#include "scene.h"
#include "renderer.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CInputKeyboard;
class CJoyPad;
class CInputMouse;
class CSound;
class CTexture;
class CCamera;
class CLight;
class CFade;
class CXfileManager;
class CScene;
class CMotionManager;
class CModelManager;
class CJsonManager;

//*********************************************************
// マネージャークラスを定義
//*********************************************************
class CManager
{
public:

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CScene::MODE GetScene(void);
	void SetScene(std::unique_ptr<CScene> pNewScene);

	CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard.get(); }
	CMotionManager* GetMotionManager(void) { return m_pMotionManager.get(); }
	CModelManager* GetModelManagere(void) { return m_pModelManager.get(); }
	CJsonManager* GetJsonManager(void) { return m_pJsonManager.get(); }
	CXfileManager* GetXManager(void) { return m_pXfileManager.get(); }
	CRenderer* GetRenderer(void) { return m_pRenderer.get(); }
	CInputMouse* GetMouse(void) { return m_pInputMouse.get(); }
	CTexture* GetTexture(void) { return m_pTexture.get(); }
	CJoyPad* GetJoyPad(void) { return m_pJoyPad.get(); }
	CCamera* GetCamera(void) { return m_pCamera.get(); }
	CScene* GetSceneRaw(void) { return m_pScene.get(); }
	CSound* GetSound(void) { return m_pSound.get(); }
	CLight* GetLight(void) { return m_pLight.get(); }
	CFade* GetFade(void) { return m_pFade.get(); }

	static CManager* GetInstance(void);

private:

	CManager();
	~CManager();

	std::unique_ptr<CRenderer>m_pRenderer;						// レンダラークラスのポインタ
	std::unique_ptr<CInputKeyboard>m_pInputKeyboard;			// キーボードクラスのポインタ
	std::unique_ptr<CJoyPad>m_pJoyPad;							// ジョイパッドクラスのポインタ
	std::unique_ptr<CSound>m_pSound;							// サウンドオブジェクトのポインタ
	std::unique_ptr<CInputMouse>m_pInputMouse;					// マウスクラスのポインタ
	std::unique_ptr<CTexture>m_pTexture;						// テクスチャクラスのポインタ
	std::unique_ptr<CCamera>m_pCamera;							// カメラクラスのポインタ
	std::unique_ptr<CLight>m_pLight;							// ライトクラスのポインタ	
	std::unique_ptr<CScene>m_pScene;;							// シーン管理クラスのポインタ
	std::unique_ptr<CFade>m_pFade;								// フェードクラスのポインタ
	std::unique_ptr<CXfileManager>m_pXfileManager;				// Xファイルマネージャークラス
	std::unique_ptr<CMotionManager>m_pMotionManager;			// モーションマネージャークラス
	std::unique_ptr<CModelManager>m_pModelManager;				// キャラクターモデル管理クラス
	std::unique_ptr<CJsonManager>m_pJsonManager;				// json管理クラスの定義
};