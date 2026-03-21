//=========================================================
//
// 3Dモデルインスタンシング [ Instancing.hlsl ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// シェーダー内グローバル宣言空間
//*********************************************************
float4x4 g_mtxview;         // ビューマトリックス
float4x4 g_mtxprojection;   // プロジェクションマトリックス

float4 g_MatColor;          // マテリアルカラー
texture2D g_TexCharactor;   // テクスチャポインタ

sampler2D CharactorSampler = sampler_state // テクスチャサンプラー
{
    Texture = <g_TexCharactor>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

//*********************************************************
// 頂点シェーダー入力構造体
//*********************************************************
struct VS_INPUT
{
    float4 pos : POSITION; // 座標
    float2 uv : TEXCOORD;  // UV値
    float3 normal : NORMAL; // 法線

    float4 world0 : TEXCOORD1; // ワールド行列
    float4 world1 : TEXCOORD2; // ワールド行列
    float4 world2 : TEXCOORD3; // ワールド行列
    float4 world3 : TEXCOORD4; // ワールド行列
};
//*********************************************************
// 頂点シェーダー出力構造体
//*********************************************************
struct VS_OUT
{
    float4 pos : POSITION; // 座標
    float2 uv : TEXCOORD; // UV値
    float3 normal : NORMAL; // 法線
};

//=========================================================
// 頂点シェーダーメインエントリーポイント関数
//=========================================================
VS_OUT VS_Main(VS_INPUT input)
{
	// 出力構造体変数
    VS_OUT output;

	// ワールドマトリックスを設定する
    float4x4 mtxWorld = float4x4
	(
		input.world0,
		input.world1,
		input.world2,
		input.world3
	);

	// ワールド座標を計算
    float4 worldPos = mul(input.pos, mtxWorld);
	
	// ビュー変換座標を計算
    float4 viewPos = mul(worldPos, g_mtxview);
	
	// 出力座標に変換
    output.pos = mul(viewPos, g_mtxprojection);
	
	// uv設定
    output.uv = input.uv;
	
	// 法線設定
    output.normal = input.normal;
	
	// 最終出力変数を返す
    return output;
}

//=========================================================
// テクスチャピクセルシェーダーメインエントリーポイント関数
//=========================================================
float4 PS_TexMain(VS_OUT output) : COLOR
{
	// テクスチャを返す
    return tex2D(CharactorSampler, output.uv);
}
//=========================================================
// デフォルトカラーピクセルシェーダーメインエントリーポイント関数
//=========================================================
float4 PS_Main(VS_OUT output) : COLOR
{
	// カラーそのままを返す
    return g_MatColor;
}

//=========================================================
// テクニック宣言
//=========================================================
technique Instancing
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }

    pass P1
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_TexMain();
    }
}