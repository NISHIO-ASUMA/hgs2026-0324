//=========================================================
//
// 入力処理関数 [ input.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 入力親クラスを定義
//*********************************************************
class CInput
{
public:

	CInput();
	~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

	LPDIRECTINPUTDEVICE8 GetInputDevice(void);

protected:

	static LPDIRECTINPUT8 m_pInput; // 入力変数
	LPDIRECTINPUTDEVICE8 m_pDevice; // 入力デバイス
};

//*********************************************************
// キーボードクラスを定義
//*********************************************************
class CInputKeyboard : public CInput
{
public:

	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey,int nMaxTime);

private:

	static inline constexpr int KEY_MAX = 256; // 最大キー数

	BYTE m_aKeystate[KEY_MAX];	// キー入力状態
	BYTE m_aOldState[KEY_MAX];	// 過去の入力内容を保存
	int m_nKeyPressCount;		// キー入力カウント
};

//*********************************************************
// ゲームパッドクラスを定義
//*********************************************************
class CJoyPad : public CInput
{
public:

	//*********************************
	// ジョイパッドのキーの列挙型
	//*********************************
	enum JOYKEY
	{
		JOYKEY_UP,			 // 十字キー(上)
		JOYKEY_DOWN,		 // 十字キー(下)
		JOYKEY_LEFT,		 // 十字キー(左)
		JOYKEY_RIGHT,		 // 十字キー(右)
		JOYKEY_START,		 // スタートキー
		JOYKEY_BACK,		 // バックキー
		JOYKEY_LS,			 // L3(Lスティック)
		JOYKEY_RS,			 // R3(Rスティック)
		JOYKEY_LEFT_B,		 // LBキー
		JOYKEY_RIGHT_B,		 // RBキー
		JOYKEY_LEFT_TRIGGER, // LTキー
		JOYKEY_RIGHT_TRIGGER,// RTキー
		JOYKEY_A,			 // Aボタン
		JOYKEY_B,			 // Bボタン
		JOYKEY_X,			 // Xボタン
		JOYKEY_Y,			 // Yボタン
		JOYKEY_MAX			 // パッドの最大数
	};

	CJoyPad();
	~CJoyPad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void UpdateVibration(void);

	bool GetPress(JOYKEY Key);
	bool GetTrigger(JOYKEY Key);
	bool GetRelease(JOYKEY Key);
	bool GetRepeat(JOYKEY Key, int nMaXTime);
	bool GetTriggerLT(void);
	bool GetTriggerRT(void);
	bool GetPressLT(void);
	bool GetPressRT(void);
	bool GetLeftStick(void);

	inline XINPUT_STATE* GetStickAngle(void) { return &m_joyKeyState; }

	/// <summary>
	/// コントローラーの振動処理
	/// </summary>
	/// <param name="leftMotor">左側の振動の強さ</param>
	/// <param name="rightMotor">右側の振動の強さ</param>
	/// <param name="durationMs">振動フレーム時間</param>
	void SetVibration(int leftMotor, int rightMotor, int durationMs);

private:

	XINPUT_STATE m_joyKeyState;			// ジョイパッドのプレス情報
	XINPUT_STATE m_OldKeyState;			// 過去のジョイパッド入力情報
	XINPUT_STATE m_joyKeyStateTrigger;	// ジョイパッドのトリガー情報

	int m_nPressCount;					// 入力カウント
	int m_leftMotor;					// 左モーターの強さ
	int m_rightMotor;					// 右モーターの強さ
	DWORD m_VibrationEndTime;			// 振動終了時刻
	bool m_isVibration;					// 振動中かどうか
};

//*********************************************************
// マウスクラスを定義
//*********************************************************
class CInputMouse : public CInput
{
public:

	//**************************
	// ボタンキー種類
	//**************************
	enum MOUSE
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_MAX
	};

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	void SetCursorVisibility(bool visible);
	void GetRay(D3DXVECTOR3* pOutOrigin, D3DXVECTOR3* pOutDir);

	bool GetTriggerDown(int button_type);
	bool GetTriggerUp(int button_type);
	bool GetPress(int button_type);
	bool GetState(DIMOUSESTATE* mouseState);

	inline int GetMouseX(void) const { return m_CurrentMouseState.lX; }
	inline int GetMouseY(void) const { return m_CurrentMouseState.lY; }
	inline D3DXVECTOR2 GetMouseVelocity() { return D3DXVECTOR2((float)m_CurrentMouseState.lX, (float)m_CurrentMouseState.lY); }
	inline D3DXVECTOR2 GetMouseOldVelocity() { return D3DXVECTOR2((float)m_PrevState.lX, (float)m_PrevState.lY); }
	inline BOOL IsMouseWheelPresent(void) { return (GetSystemMetrics(SM_MOUSEWHEELPRESENT) != 0); }


private:
	DIMOUSESTATE m_PrevState;			// マウス状態
	DIMOUSESTATE m_CurrentMouseState;	// 現在のマウス状態
};