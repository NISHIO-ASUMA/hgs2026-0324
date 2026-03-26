//=========================================================
//
// プリコンパイルヘッダー [ pch.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル
//*********************************************************
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "d3dx9.h"
#include "d3d9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"
#include <Shlwapi.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <crtdbg.h>

//*********************************************************
// ライブラリのリンク宣言
//*********************************************************
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"shlwapi.lib")

//*********************************************************
// マクロ定義
//*********************************************************
#define SCREEN_WIDTH (1280)				// ウインドウの幅
#define SCREEN_HEIGHT (720)				// ウインドウの高さ
#define CLASS_NAME "WindowClass"		// ウインドウクラスの名前
#define WINDOW_NAME "FlyFlog"			// ウインドウの名前

#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					// [2D]頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1)		// [3D]頂点フォーマット
#define FVF_VERTEX_3D_MULTI (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2) // [3Dマルチ]頂点フォーマット

#define VECTOR3_NULL (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// VECTOR3座標初期化
#define VECTOR4_NULL (D3DXVECTOR4(0.0f, 0.0f, 0.0f,0.0f))	// VECTOR4座標初期化
#define INITSCALE (D3DXVECTOR3(1.0f, 1.0f, 1.0f))			// 初期拡大率
#define CENTERPOS (D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// 画面の中心座標
#define HALFWIDTH (SCREEN_WIDTH * 0.5f)						// スクリーンの横幅の半分
#define HALFHEIGHT (SCREEN_HEIGHT * 0.5f)					// スクリーンの高さの半分
#define HALF (0.5f)											// 半分の値

#define COLOR_BLACK (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// 黒
#define COLOR_CLEAR (D3DXCOLOR(1.0f,1.0f,1.0f,0.0f))		// 透明
#define COLOR_WHITE (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 白
#define COLOR_RED	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))		// 赤
#define COLOR_GREEN (D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.75f))	// 緑
#define COLOR_YERROW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	// 黄色
#define COLOR_BLUE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	// 青
#define COLOR_PURPLE (D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f))	// 紫
#define COLOR_GLAY	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f))	// グレー
#define LASER		 (D3DXCOLOR(0.05f, 0.05f, 1.0f, 1.0f))	// レーザーカラー
#define COLOR_NULL	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// 完全透明
#define SCORECOLOR	 (D3DXCOLOR(0.0f, 0.75f, 0.9f, 1.0f))	// ランキングスコアの色

#define V_COLOR_WHITE (D3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f))	// モデルのカラー白
#define V_COLOR_RED (D3DCOLORVALUE(1.0f, 0.0f, 0.0f, 1.0f))		// モデルのカラー赤
#define OUTLINE_COLOR (D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f))		// アウトライン基準カラー

#define BASEVERTEX (4)		// 基準頂点数
#define MAX_WORD (256)		// 最大文字列数

//*********************************************************
// 頂点情報[2D]の構造体
//*********************************************************
struct VERTEX_2D
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
};

//*********************************************************
// 頂点情報[3D]の構造体
//*********************************************************
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
};

//*********************************************************
// 頂点情報[3D_マルチ]の構造体
//*********************************************************
struct VERTEX_3D_MULTI
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
	D3DXVECTOR2 texM;	// マルチテクスチャ
};

//*********************************************************
// 3Dモデル構造体
//*********************************************************
struct MODEL_3D
{
	D3DXVECTOR3 pos;	// 座標
	D3DXVECTOR2	tex;	// テクスチャ
	D3DXVECTOR3 normal;	// 法線
};