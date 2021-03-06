///////////////////////////////////////
// ZPrepass
///////////////////////////////////////


///////////////////////////////////////
// 構造体。
///////////////////////////////////////


//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
    int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos 		: POSITION;		//モデルの頂点座標。
    float3 normal	: NORMAL;		//モデルの法線ベクトル。
    float3 tangent  : TANGENT;		//接ベクトル
    float3 biNormal : BINORMAL;		//従法線
    float2 uv 		: TEXCOORD0;	//UV座標。
    SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       //座標。
    float3 depth : TEXCOORD0;       //深度値。xにはプロジェクション空間、yにはカメラ空間での正規化されたZ値、zにはカメラ空間でのZ値
    float3 normal	: TEXCOORD1;		//スクリーン空間でのモデルの法線ベクトル。

};

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。


///////////////////////////////////////
// 定数バッファ。
///////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};




///////////////////////////////////////
// 関数
///////////////////////////////////////
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);
float4x4 CalcSkinMatrix(SSkinVSIn skinVert);

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}


/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}


/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	//ピクセルシェーダに渡すデータ
	SPSIn psIn;

	//ワールド行列
	float4x4 m;

	if (hasSkin)
	{
		//スキンあり
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else
	{
		//スキンなし
		m = mWorld;
	}
    //深度値。xにはプロジェクション空間、yにはカメラ空間での正規化されたZ値、zにはカメラ空間でのZ値
    psIn.pos = mul(m, vsIn.pos); // モデルの頂点をワールド座標系に変換
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.depth.z = psIn.pos.z;      //zにはカメラ空間でのZ値
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.depth.x = psIn.pos.z / psIn.pos.w; //xにはプロジェクション空間
    psIn.depth.y = saturate(psIn.pos.w / 1000.0f);  //yにはカメラ空間での正規化されたZ値

    //頂点法線
    psIn.normal = vsIn.normal;
	return psIn;
}


// モデル用のピクセルシェーダーのエントリーポイント
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //xにプロジェクション空間での深度値。
    //y,z,wには頂点の法線情報が入る。
    return float4( psIn.depth.x, psIn.normal.x, psIn.normal.y, psIn.normal.z);
}

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }

    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

    return skinning;
}