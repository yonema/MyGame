#include "stdafx.h"
#include "ModelRender.h"

//モデルレンダラーの定数データを使えるようにする
using namespace modelRenderConstData;

/// <summary>
/// デフォルトの初期化処理関数
/// 最初に呼んで！
/// </summary>
/// <param name="filePath">tkmのファイルパス</param>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
/// <param name="modelUpAxis">モデルのUP軸</param>
void CModelRender::Init(
	const char* filePath,
	D3D12_CULL_MODE cullMode,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnModelUpAxis modelUpAxis)
{
	//tkmのファイルパスを保持
	m_tkmFilePath = filePath;
	//スケルトンのデータの読み込み。
	InitSkeleton(filePath);
	//モデルを初期化
	InitModel(filePath, cullMode, modelUpAxis);
	//アニメーションを初期化
	InitAnimation(animationClips, numAnimationClips);
	SetShadowCasterFlag(true);
	SetShadowReceiverFlag(true);

	InitZPrepassModel();
	//初期化完了
	m_isInited = true;

	return;
}


/// <summary>
/// カスタム用の初期化関数
/// 最初に呼んで！
/// デフォルトのModelInitDataとは違う内容で初期化するときに使う
/// </summary>
/// <param name="initData">初期化用のデータ</param>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
void CModelRender::Init(
	ModelInitData initData,
	AnimationClip* animationClips,
	int numAnimationClips)
{
	//tkmのファイルパスを保持
	m_tkmFilePath = initData.m_tkmFilePath;
	//スケルトンのデータの読み込み。
	InitSkeleton(initData.m_tkmFilePath);
	//モデルを初期化
	m_model.Init(initData);
	//アニメーションを初期化
	InitAnimation(animationClips, numAnimationClips);

	InitZPrepassModel();


	//初期化完了
	m_isInited = true;

	return;
}


/// <summary>
/// スケルトンを初期化。
/// tkmファイルパスをtksファイルパスに変換する
/// </summary>
/// <param name="filePath">tkmファイルパス</param>
/// <returns>初期化が成功したか？</returns>
bool CModelRender::InitSkeleton(const char* filePath)
{
	//tkmファイルをtksファイルに変換する
	std::string skeletonFilePath = filePath;
	//ファイルパスの文字列から、拡張子の場所を探す
	int pos = (int)skeletonFilePath.find(FILE_EXTENSION_TKM);
	//tkmのファイルパスからtksのファイルパスに入れ替える
	skeletonFilePath.replace(pos, FILE_EXTENSION_LENGHT, FILE_EXTENSION_TKS);

	//スケルトンのリソースの確保
	m_skeletonPtr.reset(new Skeleton);
	//スケルトンのデータを読み込み。
	bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

	//初期化に成功したか？
	if (isInited)
		return true;	//成功したらtrueを返す
	else
	{
		//失敗したらスケルトンのリソースを開放して
		//falseを返す
		m_skeletonPtr.reset();
		return false;
	}
}


/// <summary>
/// モデルを初期化。
/// </summary>
/// <param name="filePath">tkmファイルパス</param>
/// <param name="modelUpAxis">モデルのUP軸</param>
void CModelRender::InitModel(const char* filePath, D3D12_CULL_MODE cullMode, EnModelUpAxis modelUpAxis)
{
	//初期化データを作成する
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = filePath;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = SHADER_FILEPATH_DEFAULT;
	//スケルトンを指定する。
	if (m_skeletonPtr)	//スケルトンが初期化されていたら
		initData.m_skeleton = m_skeletonPtr.get();
	//モデルの上方向を指定する。
	initData.m_modelUpAxis = modelUpAxis;

	//シェーダに渡すコンスタントバッファの設定

	//ライト共通のデータの登録
	initData.m_expandConstantBuffer[0] =
		CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize[0] =
		sizeof(*CLightManager::GetInstance()->GetLightParam());

	//ディレクションライト達のデータの登録
	initData.m_expandConstantBuffer[1] =
		CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize[1] =
		sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) *
		CLightManager::GetMax_DirectionLight();

	//ポイントライト達のデータの登録
	initData.m_expandConstantBuffer[2] =
		CLightManager::GetInstance()->GetPointLigData();
	initData.m_expandConstantBufferSize[2] =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_PointLight();

	//スポットライト達のデータの登録
	initData.m_expandConstantBuffer[3] =
		CLightManager::GetInstance()->GetSpotLigData();
	initData.m_expandConstantBufferSize[3] =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_SpotLight();

	//シャドウのデータの登録
	initData.m_expandConstantBuffer[4] =
		g_graphicsEngine->GetShadowMap().GetShadowParam();
	initData.m_expandConstantBufferSize[4] =
		sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam()) * g_MAX_SHADOW_NUM;

	//シェーダーリソースビューの設定

	//シャドウマップの登録
	initData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();
	//ZPrepassで作成された深度テクスチャの登録
	initData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
	//スカイキューブの登録
	initData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();

	//カリングモードを設定する
	initData.m_cullMode = cullMode;

	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);

	return;
}


/// <summary>
/// アニメーションを初期化。
/// </summary>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
void CModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
{
	//アニメーションクリップが登録されているか
	if (animationClips == nullptr)
		return;	//されていない場合は何もしない

	//アニメーションのリソースの確保
	m_animationPtr.reset(new Animation);
	//アニメーションを初期化。
	m_animationPtr->Init(
		*m_skeletonPtr,			//アニメーションを流し込むスケルトン。
		animationClips,		//アニメーションクリップ。
		numAnimationClips	//アニメーションの数。
	);

	return;
}

/// <summary>
/// シャドウ用のモデルを初期化
/// </summary>
void CModelRender::InitShadowModel()
{
	//シャドウ用のモデルクラスの初期化
	m_shadowModel.Init();

	//シャドウ用のモデルの初期化データ
	ModelInitData initShadowModelData;
	initShadowModelData.m_fxFilePath = SHADER_FILTPATH_SHADOW_MAP;
	//シャドウのパラーメータを定数バッファに渡す
	initShadowModelData.m_expandConstantBuffer[0] = 
		(void*)g_graphicsEngine->GetShadowMap().GetShadowParam();
	initShadowModelData.m_expandConstantBufferSize[0] = 
		sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam());

	initShadowModelData.m_colorBufferFormat = DXGI_FORMAT_R32G32_FLOAT;

	initShadowModelData.m_tkmFilePath = m_tkmFilePath;

	//シャドウ用のモデルの初期化
	m_shadowModel.InitModel(initShadowModelData);
	//パラメータを更新する
	m_shadowModel.UpdateModel(
		m_position,
		m_rotation,
		m_scale
	);

	return;
}

/// <summary>
/// Zプリパス用のモデルを初期化
/// </summary>
void CModelRender::InitZPrepassModel()
{
	//Zプリパス用モデルの初期化データ
	ModelInitData modelInitData;
	modelInitData.m_tkmFilePath = m_tkmFilePath;
	modelInitData.m_fxFilePath = SHADER_GILEPATH_Z_PREPASS;
	modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	modelInitData.m_trans = false;
	modelInitData.m_cullMode = D3D12_CULL_MODE_NONE;

	//Zプリパス用のモデルの初期化
	m_zprepassModel.Init(modelInitData);

	return;
}


//アップデート関数
void CModelRender::Update()
{
	//初期化されているか？
	if (!m_isInited)
		return;	//されていない場合は何もしない


	//スケルトンを更新。
	if (m_skeletonPtr)	//スケルトンが初期化されていたら
		m_skeletonPtr->Update(m_model.GetWorldMatrix());
	//アニメーションを進める。
	if (m_animationPtr)	//アニメーションが初期化されていたら
		m_animationPtr->Progress(GameTime().GetFrameDeltaTime());

	//モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	//シャドウ用のモデルが初期化されていたら
	if (m_shadowModel.IsValid())
		//パラメータ更新
		m_shadowModel.UpdateModel(m_position, m_rotation, m_scale);

	//ZPrepass用モデルの更新
	m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	return;
}

//常に呼ばれるアップデート関数
void CModelRender::AlwaysUpdate()
{
	//輪郭線を描画するか？
	if (m_drawOutLineFlag)
	{
		// ZPrepassの描画パスにモデルを追加
		g_graphicsEngine->Add3DModelToZPrepass(m_zprepassModel);
	}

	return;
}


//描画用関数
void CModelRender::Render(RenderContext& rc)
{
	//初期化されているか？
	if (!m_isInited)
		return;	//されていない場合は何もしない



	//モデルを描画
	m_model.Draw(rc);

	return;
}