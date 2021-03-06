#include "stdafx.h"
#include "ShadowModel.h"

//コンストラクタ
CShadowModel::CShadowModel()
{
}
//デストラクタ
CShadowModel::~CShadowModel()
{
	RemoveShadowModel();
}

/// <summary>
/// 初期化関数
/// </summary>
void CShadowModel::Init()
{
	m_model.reset(new Model);
	g_graphicsEngine->GetShadowMap().AddShadowModel(*m_model);
	m_isValid = true;
}

/// <summary>
/// モデルの初期化関数
/// </summary>
/// <param name="initData">モデルの初期化データ</param>
void CShadowModel::InitModel(ModelInitData& initData)
{
	m_model->Init(initData);
}

/// <summary>
/// モデルのパラメータ更新
/// </summary>
/// <param name="pos">場所</param>
/// <param name="rot">回転</param>
/// <param name="scale">拡大</param>
void CShadowModel::UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_model->UpdateWorldMatrix(pos, rot, scale);
}

/// <summary>
/// シャドウ用のモデルの登録を消去する
/// </summary>
void CShadowModel::RemoveShadowModel()
{
	if (m_isValid)
	{
		g_graphicsEngine->GetShadowMap().RemoveShadowModel(*m_model);
		m_isValid = false;
	}
}