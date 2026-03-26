//=========================================================
//
// サウンド処理 [ sound.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// サウンドクラスを定義
//*********************************************************
class CSound
{
public:

	//***************************
	// サウンドの種類列挙型
	//***************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_TITLE_BGM,		// タイトルBGM
		SOUND_LABEL_GAMEBGM,		// ゲームBGM
		SOUND_LABEL_RESULTBGM,		// リザルトBGM
		SOUND_LABEL_TUTORIALBGM,	// チュートリアルBGM
		SOUND_LABEL_LOSERESULTBGM,	// 敗北リザルト
		SOUND_LABEL_RANKING,		// ランキングBGM
		SOUND_LABEL_SELECT,			// キー選択SE
		SOUND_LABEL_RETURN,			// 決定キーSE
		SOUND_LABEL_ITEM,			// アイテムSE
		SOUND_LABEL_ALART,			// 警告音SE
		SOUND_LABEL_BULLET,			// 通常弾SE
		SOUND_LABEL_LASER,			// レーザーSE
		SOUND_LABEL_LIFE,			// 回復SE
		SOUND_LABEL_DAMAGE,			// ダメージSE
		SOUND_LABEL_CIRCLE,			// メッシュ攻撃SE
		SOUND_LABEL_CLEAR,			// タスク完了SE
		SOUND_LABEL_GUARD,			// ガード音SE
		SOUND_LABEL_EXPLOSION,		// 衝撃音SE
		SOUND_LABEL_BOSS,			// ボス叫びSE
		SOUND_LABEL_CHANGEWEPON,	// 武器切り替え
		SOUND_LABEL_AVOID,			// 回避SE
		SOUND_LABEL_TITLEENTER,		// タイトルキー
		SOUND_LABEL_PAUSESTART,		// 開始音
		SOUND_LABEL_QUEEN,			// 女王ヒット
		SOUND_LABEL_START,			// ゲーム開始音
		SOUND_LABEL_EAT,			// 食べる音
		SOUND_LABEL_FLOG,			// 声
		SOUND_LABEL_JUMP,			// ジャンプ
		SOUND_LABEL_MAX
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:

	//*****************************
	// サウンド情報の構造体定義
	//*****************************
	struct SOUNDINFO
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
	};

	//******************************
	// サウンドの情報列挙型宣言
	//******************************
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data/BGM/titlebgm.wav", -1},		 // タイトルBGM
		{"data/BGM/gamebgm.wav",-1},		 // ゲームBGM
		{"data/BGM/resultbgm.wav",-1},		 // リザルトBGM
		{"data/BGM/tutorialbgm.wav",-1},	 // チュートリアルBGM
		{"data/BGM/LoseResult.wav",-1},		 // 敗北リザルトBGM
		{"data/BGM/rankingbgm.wav",-1},		 // ランキングBGM

		{"data/SE/keyselect.wav",0},		 // 選択キーSE
		{"data/SE/keyenter.wav",0},			 // 決定キーSE
		{"data/SE/Itemse.wav",0},			 // アイテムSE
		{"data/SE/Alart.wav",0},			 // 警告音SE
		{"data/SE/BulletSe.wav",0},			 // 通常弾
		{"data/SE/LaserBullet.wav",0},		 // レーザー弾
		{"data/SE/LifeSe.wav",0},			 // 回復SE
		{"data/SE/Damage.wav",0},			 // ダメージSE
		{"data/SE/MeshCircle.wav",0},		 // メッシュ攻撃SE
		{"data/SE/TutoClear.wav",0},		 // タスク完了SE
		{"data/SE/Guard.wav",0},			 // ガード音SE
		{"data/SE/Explosion.wav",0},		 // 衝撃音SE
		{"data/SE/BossShout.wav",0},		 // 叫び声SE
		{"data/SE/Change.wav",0},			 // 武器切り替えSE
		{"data/SE/Avoid.wav",0},			 // 回避SE
		{"data/SE/TitleEnter.wav",0},		 // タイトルキー
		{"data/SE/pausekey.wav",0},			 // ポーズキー
		{"data/SE/QueenHit.wav",0},			 // 女王ヒット
		{"data/SE/Startbgm.wav",0},			 // ゲーム開始画面音
		{"data/SE/Laser.wav",0},			 // 食べる音
		{"data/SE/frogvoice.wav",0},			 // カエル
		{"data/SE/jump.wav",0}
	};

	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	int m_SoundCount;										// サウンドのカウント
	int m_SoundInterval;									// インターバルカウント

};